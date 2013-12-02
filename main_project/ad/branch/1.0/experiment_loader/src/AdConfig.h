/*
 *  AdConfig.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description: 实验数据cache的配置更新 
 *
 */

#ifndef AD_CONFIG_H_
#define AD_CONFIG_H_

#include "Util.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "Evictor.h"
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
    db_name_ = "ad_strategy";
    db_host_ = "10.3.23.69";
  }
  void LoadConfig();
  string GetValue(string key);
  
  string ConfigDbHost(){
    return db_host_;
  }
  string ConfigDbName(){
    return db_name_;
  }
  string CtrOnlineModelPath(){
    return ctr_online_model_path_;
  }
  string AdClassListPath(){
    return ad_class_list_path_;
  }
  int MaxAdClassNum(){
    return max_ad_class_num_;
  }
  int RealTimeWindow(){
    return real_time_window_;
  }
  double RealTimeWeightLimit();
  double ThresholdRatio(int hour);

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
  string ctr_online_model_path_;
  string ad_class_list_path_;
  int max_ad_class_num_;
  int real_time_window_;
  double pv_threshold_ratio_;
  double real_time_weight_limit_;
  map<string, string> map_;
  IceUtil::RWRecMutex mutex_;
};

} // end namespace ad
} // end namespace xce
#endif /* AD_CONFIG_H_ */
