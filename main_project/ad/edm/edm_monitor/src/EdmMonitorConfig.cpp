#include <sstream>
#include <log4cplus/logger.h>
#include <boost/lexical_cast.hpp>
#include "../include/EdmMonitorConfig.h"
#include "QueryRunner.h"

USE_NS(std)
USE_NS(mysqlpp)
USE_NS(xce::edm)
using namespace com::xiaonei::xce;

void EdmMonitorConfig::Load(){
  StoreQueryResult res;
	try{
		const string db_instance = "ad_st";
		Statement sql;
		sql << "SELECT  * FROM edm_monitor_parameter";
		res = QueryRunner(db_instance, CDbRServer).store(sql);
  } catch (Ice::Exception& e){
      MCE_WARN("EdmMonitorConfig::Load edm_quota_info Error! " << e.what());
      return;
  } catch (std::exception& e){
      MCE_WARN("EdmMonitorConfig::Load edm_quota_info std error! " << e.what());
      return;
  }

  if (!res){
    MCE_WARN("Query Result Error!");
    return;
  }
  else if (res.empty()){
    MCE_INFO("nothing in db!");
    return;
  }
  MCE_INFO("EdmMonitorCache::Load EdmMonitorConfig Cache,  Size=: " << res.num_rows());
  map<string, double> pool;
  isLoading_ = true;
  for (size_t i = 0; i < res.num_rows(); ++i) {
	mysqlpp::Row row = res.at(i);
	std::string key =   boost::lexical_cast<std::string> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["key"]);
	double value  = boost::lexical_cast<double> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["value"]);
	pool[key] = value;
	MCE_INFO("EdmMonitorConfig::Load "<<key<<"--->"<<value);
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    config_.swap(pool);
  }
  isLoading_ = false;
}

void EdmMonitorConfig::Reset(const string& name, StoreQueryResult& res){
  try{    
    double value = boost::lexical_cast<double> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)res.at(0)[name.c_str()]);
    config_[name] = value;
    MCE_INFO("EdmMonitorConfig::Reset --> name: " << name << " value: " << value);
  } catch (std::exception& e){
  MCE_WARN("EdmMonitorConfig::Reset error! name: " << name); 
    return;
  }
}

double EdmMonitorConfig::GetValue(const string& name){
  if (isLoading_){
    MCE_INFO("EdmMonitorConfig::GetValue Failed --> is Loading from sql.");
    return FAILED;
  }

  IceUtil::RWRecMutex::RLock lock(mutex_);
  if (config_.find(name) != config_.end()){
    MCE_INFO("EdmMonitorConfig::GetValue() --> name: " << name << " value: " << config_[name]);
    return config_[name];
  }
  MCE_WARN("EdmMonitorConfig::GetValue --> name: " << name << " not exist in config_ map!");
  return FAILED;
}
