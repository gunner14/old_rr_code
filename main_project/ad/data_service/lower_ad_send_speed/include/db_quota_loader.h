/*
 * =====================================================================================
 *
 *       Filename:  DBQuotaLoader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年08月24日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lei.yu1 
 *        Company:  renren
 *
 * =====================================================================================
 */

#ifndef __XCE_AD_DB_QUOTA_LOADER__
#define __XCE_AD_DB_QUOTA_LAODER__
#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include "ServiceI.h"
#include "Singleton.h"
#include "../common/struct.h"
#include <Ice/Config.h>
#include "compute_ratio.h"
#include "LogWrapper.h"
#include <QueryRunner.h>
#include <DbDescriptor.h>

namespace xce{
namespace ad{

class DBQuotaLoader: public MyUtil::Singleton<DBQuotaLoader> {
public:
  bool Load(std::map<Ice::Long, DummyRecordPtr>& records, const std::vector<std::string>& node_name, const com::xiaonei::xce::Statement& sql, ComputeRatioPtr computer, time_t& last_update_time, const std::string& table_name);
  void SetDB(const std::string& db) { db_instance_ = db; } 
protected:
  bool GetDataFromDB(const com::xiaonei::xce::Statement& sql, mysqlpp::StoreQueryResult& res, const std::string& table_name);
  bool Parse(const mysqlpp::StoreQueryResult& res, const std::vector<std::string>& node_name, std::map<Ice::Long, DummyRecordPtr>& records, ComputeRatioPtr computer, time_t& last_update_time);

private:
  std::string db_instance_; 
};//end of class DBQuotaLoader 

}//end of namespace ad
}//end of namespace xce
#endif
