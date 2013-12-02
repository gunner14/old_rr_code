/*
 *  AdConfig.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description: 实验数据cache的配置更新 
 *
 */

#ifndef AD_CONFIG_H_
#define AD_CONFIG_H_

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "Util.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "Evictor.h"
#include "IceLogger.h"
#include <map>

namespace xce {
namespace ad {


class AdExperimentConfig : public MyUtil::Singleton<AdExperimentConfig> {
public:
  AdExperimentConfig() {
    pv_threshold_ratio_ = 0.055;
    real_time_weight_limit_ = 0.2;
    real_time_window_ = 3;
    max_ad_class_num_ = 5;
    ad_class_list_path_ = "../data/ad_class_list";
    ctr_online_model_path_ = "../data/ctr_online_model";
    refresh_ctr_ratio_path_ = "../data/refresh_ctr_ratio";
    ad_extra_data_path_ = "../data/ad_extra_data";
    db_name_ = "ad_strategy";
    db_host_ = "10.3.23.69";\
    jebe_db_host_ = "reportbi.db.jebe.d.xiaonei.com";
    db_sds_basic_ = "sds_basic";
    zone_ids_.push_back(0);
    zone_ids_.push_back(100000000001);
    zone_ids_.push_back(100000000063);
    zone_ids_.push_back(100000000070);
    zone_ids_.push_back(100000000072);
  }
  void LoadConfig();
  
  string GetValue(string key);
   
  template<typename  AdExperConfigValueT>
    AdExperConfigValueT GetValue(string& key, AdExperConfigValueT default_value){
      AdExperConfigValueT res = default_value;
      string value = GetValue(key);
      if (!value.empty()) {
      try {
        res = boost::lexical_cast<AdExperConfigValueT>(value); 
      } catch(...) {
        MCE_WARN("AdExperimentConfig::GetValue --> parser config value error.");
      } 
    }
    return res;
  }

  string ConfigDbHost(){
    return db_host_;
  }
  string ConfigDbName(){
    return db_name_;
  }
  string JebeDbHost(){
    return jebe_db_host_;
  }
  string DbSdsBasic(){
    return db_sds_basic_;
  }
  string RefreshCtrRatioPath(){
    return refresh_ctr_ratio_path_;
  }
  string CtrOnlineModelPath(){
    return ctr_online_model_path_;
  }
  string AdClassListPath(){
    return ad_class_list_path_;
  }
  string AdExtraDataPath(){
    return ad_extra_data_path_;
  }
  int MaxAdClassNum(){
    return max_ad_class_num_;
  }
  int RealTimeWindow(){
    string key = "real_time_window";
    return GetValue(key, real_time_window_);
  }
  int LrMaxModelId();
  double RealTimeWeightLimit();
  double ThresholdRatio(int hour);
  vector<Ice::Long> GetZoneIds(){
    return zone_ids_;
  }
  class ConfigLoadTimer : public MyUtil::Timer{
  public:
    ConfigLoadTimer(int time = 0):Timer(time){}
    virtual void handle() {
      MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(3 * 60 * 1000));//3min Load一次配置
      AdExperimentConfig::instance().LoadConfig();
    }
  };

private:
  string db_host_;
  string db_name_;
  string db_sds_basic_;
  string jebe_db_host_;
  string ctr_online_model_path_;
  string refresh_ctr_ratio_path_;
  string ad_class_list_path_;
  string ad_extra_data_path_;
  int max_ad_class_num_;
  int real_time_window_;
  double pv_threshold_ratio_;
  double real_time_weight_limit_;
  vector<Ice::Long> zone_ids_;
  map<string, string> map_;
  IceUtil::RWRecMutex mutex_;
};

} // end namespace ad
} // end namespace xce
#endif /* AD_CONFIG_H_ */
