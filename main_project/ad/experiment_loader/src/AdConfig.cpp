/*
 *  AdConfig.cpp
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description: 实验数据cache的配置更新 
 *
 */
#include "AdConfig.h"
#include <fstream>
#include <netdb.h>
#include <arpa/inet.h>
#include <connection.h>
#include <query.h>

namespace xce {
namespace ad {
using namespace mysqlpp;

int AdExperimentConfig::LrMaxModelId(){
  int res = 3; //默认有3个模型，避免错误
  string res_str = GetValue("lr_max_model_id");
  if (!res_str.empty()){
    try {
      res = boost::lexical_cast<int>(res_str); 
    } catch(...){
      MCE_WARN("AdExperimentConfig::LrMaxModelId --> parser lr_max_model_id_ error.");
    }
  }
  return res; 
}

double AdExperimentConfig::RealTimeWeightLimit(){
  double res = real_time_weight_limit_;
  string res_str = GetValue("real_time_weight_limit");
  if (!res_str.empty()){
    try {
      res = boost::lexical_cast<double>(res_str); 
    } catch(...){
      MCE_WARN("AdExperimentConfig::RealTimeWeightLimit --> parser real_time_weight_limit_ error.");
    }
  }
  return res; 
}

double AdExperimentConfig::ThresholdRatio(int hour){
  double res = pv_threshold_ratio_;
  string res_str = GetValue("pv_threshold_ratio");
  if (!res_str.empty()){
    try {
      res = boost::lexical_cast<double>(res_str); 
    } catch(...){
      MCE_WARN("AdExperimentConfig::RealTimeWeightLimit --> parser pv_threshold_ratio_ error.");
    }
  }
  if (hour >= 3 && hour <= 12) {
    res *= 2.718281828459;
  }
  if (hour >= 18 && hour <= 20) {
    res *= 1.5;
  }
  return res; 
}

string AdExperimentConfig::GetValue(string key) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<string, string>::iterator it = map_.find(key);
  if (it != map_.end()) {
    return it->second;
  } else {
    return string();
  }
}

void AdExperimentConfig::LoadConfig() {
  MCE_INFO("AdExperimentConfig::LoadConfig begin");
  map<string,string> map;
  try{ 
    const char* db_host = AdExperimentConfig::instance().ConfigDbHost().c_str();
    const char* db_name = AdExperimentConfig::instance().ConfigDbName().c_str();
    Connection conn(db_name, db_host,"ad","rrad");
    MCE_INFO("AdExperimentConfig::LoadConfig, db host:" << db_host<< ",db_name:" << db_name);
    stringstream sql; 
    sql.str("");
    sql << " select config_key, config_value from experiment_parameter";
    MCE_INFO("try to excute " << sql.str());
    string s= sql.str();
    Query q(&conn,true,s.c_str());
    mysqlpp::StoreQueryResult res = q.store();
    if (!res || res.num_rows() <= 0) { 
      MCE_WARN("AdExperimentConfig::LoadConfig Error " << sql.str() << ".  select  fail!");
    }    
    MCE_DEBUG("AdExperimentConfig::LoadConfig res size : " << res.num_rows());
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      string key = (string) row["config_key"];
      string value = (string) row["config_value"];
      MCE_DEBUG("AdExperimentConfig::LoadConfig key=" << key << ", value=" << value);
      map[key] = value;
    }    
    IceUtil::RWRecMutex::WLock lock(mutex_);
    map_.swap(map);
    MCE_DEBUG("AdExperimentConfig::LoadConfig size is " << map_.size());
  } catch (std::exception& e) { 
    MCE_WARN("AdExperimentConfig::LoadConfig error: " << e.what());
  } catch (...) {
    MCE_WARN("AdExperimentConfig::LoadConfig parameters error: unknown error!");
  }
  
  MCE_INFO("AdExperimentConfig::LoadConfig stop,map size:" << map_.size());
}


}
}
