/*
 * AdConfig.h
 *
 *  Created on: 2010-11-2
 *      Author: zhaohui.tang
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

class AdDemoConfig : public MyUtil::Singleton<AdDemoConfig> {
public:
  AdDemoConfig(): all_pv_th_(100000),all_click_th_(20),no_gender_pv_th_(50000),no_gender_click_th_(10),
                  no_stage_pv_th_(50000),no_stage_click_th_(10),spid_pv_th_(50000),spid_click_th_(10),
									two_feature_pv_th_(50000),two_feature_click_th_(10),
                  db_name_("statdb_brand"),db_host_("10.3.19.101"),db_ad_name_("ade_db"){
  }
  bool LoadConfig();
  string GetValue(string key) {
    map<string, string>::iterator it = map_.find(key);
    if (it != map_.end()) {
      return it->second;
    } else {
      return string();
    }
  }
  int GetAllPvTh() {
    return all_pv_th_;
  }
  int GetAllClickTh() {
    return all_click_th_;
  }
  int GetNoGenderPvTh(){
    return no_gender_pv_th_;
  }
  int GetNoGenderClickTh(){
    return no_gender_click_th_;
  }
  int GetNoStagePvTh(){
    return no_stage_pv_th_;
  }
  int GetNoStageClickTh(){
    return no_stage_click_th_;
  }
  int GetSpidPvTh(){
    return spid_pv_th_;
  }
  int GetSpidClickTh(){
    return spid_click_th_;
  }

  int GetTwoFeaturePvTh(){
    return two_feature_pv_th_;
  }
  int GetTwoFeatureClickTh(){
    return two_feature_click_th_;
  }

  string db_host(){
		return db_host_;
	}
	string db_name(){
		return  db_name_;
	}

  string db_ad_name(){
    return db_ad_name_;
  }
 
private:
  int all_pv_th_;
  int all_click_th_;
  int no_gender_pv_th_;
  int no_gender_click_th_;
  int no_stage_pv_th_;
  int no_stage_click_th_;
  int spid_pv_th_;
  int spid_click_th_;
	int two_feature_pv_th_;
	int two_feature_click_th_;
  
  string db_name_;
  string db_host_;
  string db_ad_name_;
  map<string, string> map_;
public:
  class ConfigLoadTimer : public MyUtil::Timer{
  public:
    ConfigLoadTimer(int time = 0):Timer(time){}
    virtual void handle() {
      MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(60 * 1000));
      AdDemoConfig::instance().LoadConfig();
    }
  };
};

} // end namespace ad
} // end namespace xce
#endif /* AD_CONFIG_H_ */
