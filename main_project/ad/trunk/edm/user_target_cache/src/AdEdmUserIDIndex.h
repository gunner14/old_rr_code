/*
 * AdEdmUserIDIndex.h
 *
 *  Created on: 2012-2-27
 *      Author:lei.yu1 
 */

#ifndef AD_EDM_GID_SUBINDEX_H_
#define AD_EDM_GID_SUBINDEX_H_

#include <sstream>
#include <ctime>
#include "LogWrapper.h"
#include "Date.h"
#include "User.pb.h"
#include "client/linux/handler/exception_handler.h"
#include "../../../util/src/AdCommonUtil.h"
#include <QueryRunner.h>
#include <DbDescriptor.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "TaskManager.h"

namespace xce {
namespace ad{

using namespace MyUtil;
using namespace std;

class AdEdmUserIDIndex : public IceUtil::Shared {
public:
  AdEdmUserIDIndex(Ice::Long adgroupID):adgroupID_(adgroupID), updateTime_(0), userCount_(0) {
  }
  
  bool FindUserID(int userID);                 //对外接口 返回指定uid是否在此index中
  void RmUserID(int userID);                   //对外接口 从当前广告投放用户群中删除指定用户 
  
  bool TotalLoadDB();                    //全量加载UserTarget数据 整合Index
  bool IncLoadDB();                      //增量加载UserTarget数据 整合Index
protected:
  void ParseUserIDSet(const mysqlpp::Row::value_type &strUserIDs, set<uint32_t>& userIDSet); 
  
  void SetAdgroupID(const long adgroupID) {
    adgroupID_ = adgroupID;
  }
  long GetAdgroupID() {
    return adgroupID_;
  }
  
protected:
  Ice::Long adgroupID_;                        //广告id
  Ice::Long updateTime_;                 //更新时间
  Ice::Long userCount_;                  //索引中uid的数量
  set<uint32_t> userIDSet_;                 //uid索引

  string DBInstance_;                    //数据源
  IceUtil::RWRecMutex mutex_;            
};// end of class AdEdmUserIDIndex

}// end of namespace ad
}// end of namespace xce

#endif
