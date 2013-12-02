/*
 * AdEdmUserTargetCacheI.h
 *
 *  Created on: 2012-2-23
 *      Author: yongjun.yin
 */

#ifndef ADEDMUSERTARGETCACHEI_H_
#define ADEDMUSERTARGETCACHEI_H_

#include "TaskManager.h"
#include "Singleton.h"
#include <map>
#include <vector>
#include "AdEdmUserTargetCache.h"
#include "AdEdmUserIDIndex.h"

namespace xce {
namespace ad{

using namespace MyUtil;
using namespace std;

typedef IceUtil::Handle<AdEdmUserIDIndex> UserIDIndexPtr;

class AdEdmUserTargetCacheI: public AdEdmUserTargetCache, public MyUtil::Singleton<AdEdmUserTargetCacheI> {
public:
  AdEdmUserTargetCacheI();
  ~AdEdmUserTargetCacheI(){}

  virtual AdgroupSeq GetAdSeqByUserID(const int userID, const Ice::Current& = Ice::Current());                  //ICE接口 获得Edm广告
  virtual bool RmAdgroupUserID(const Ice::Long adgroupID, const int userID, const Ice::Current& = Ice::Current());    //ICE接口 删除某Edm广告的用户群中指定用户

  void SetDB(const string DBInstance) {
    DBInstance_ = DBInstance;
  }
  string GetDB() {
    return DBInstance_;
  }

  void Init();
  void Reload();
  void InitOK(){
    initOK_ = true;
  }
private:
  void TotalLoadAdGroup();          //全量加载数据库
  void IncLoadAdGroup();            //增量加载数据库
  string DBInstance_;
  bool initOK_;
  map<Ice::Long, UserIDIndexPtr> adgroupUserIDMap_;

  IceUtil::RWRecMutex mutex_;
};//end of class AdEdmUserTargetCacheI

class InitTask : virtual public MyUtil::Task{
public:
  InitTask(){}
  virtual void handle();
};//end of class InitTask

class ReloadTask : virtual public MyUtil::Task{
public:
  ReloadTask(){}
  virtual void handle();
};//end of class ReloadTask

}//end of namespace ad
}//end of namespace xce

#endif
