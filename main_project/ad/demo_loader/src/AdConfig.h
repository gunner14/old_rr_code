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
  two_feature_pv_th_(50000),two_feature_click_th_(10),fresh_ad_722_th_(0.0),fresh_ad_721_th_(0.0),
  fresh_ad_632_th_(0.0),fresh_ad_631_th_(0.0),fresh_ad_702_th_(0.0),fresh_ad_701_th_(0.0),
  test_base_impr_(100000),test_real_impr_(110000),test_impr_ratio_(0.04),
  state_pv_th_(100000),state_click_th_(10),state1_test_impr_ratio_(0.1),
  state2_test_impr_ratio_(0.05),state3_test_impr_ratio_(0.05),fresh_pv_thr_(2000),fresh_click_thr_(5),fresh_ad_test_day_(4),ecpm_ratio_(1.2),app_all_pv_th_(100000),app_all_click_th_(20),app_no_gender_pv_th_(60000),app_no_gender_click_th_(15),
  app_no_stage_pv_th_(60000),app_no_stage_click_th_(15),app_spid_pv_th_(100000),app_spid_click_th_(20),
  app_two_feature_pv_th_(100000),app_two_feature_click_th_(20),
  db_source_ad_test_("ad_test"), db_source_audit_("ad_jebeaudit"),ctr_discount_type_(1),quality_db_name_("ad_quality"),
  db_source_stat_("ad_stat"),db_source_ads_("jebe_ads_load"),st_db_("ad_st"),db_source_statbigt_("ad_statbigt"){
  }
  
	bool LoadConfig();
  
  void SetDBSource_Test(string DBInstance){
    db_source_ad_test_ = DBInstance;
  }
  string GetDBSource_Test(){
    return db_source_ad_test_;
  }

  void SetDBSource_Audit(string DBInstance){
    db_source_audit_ = DBInstance;
  }
  string GetDBSource_Audit(){
    return db_source_audit_;
  }

  void SetDBSource_Stat(string DBInstance){
    db_source_stat_ = DBInstance;
  }
  string GetDBSource_Stat(){
    return db_source_stat_;
  }
  
  void SetDBSource_StatBigt(string DBInstance){
    db_source_statbigt_ = DBInstance;
  }
  string GetDBSource_StatBigt(){
    return db_source_statbigt_;
  }
  
  void SetDBSource_Ads(string DBInstance){
    db_source_ads_ = DBInstance;
  }
  string GetDBSource_Ads(){
    return db_source_ads_;
  }

	string GetValue(string key) {
    map<string, string>::iterator it = map_.find(key);
    if (it != map_.end()) {
      return it->second;
    } else {
      return string();
    }
  }

  void SetDBST(string DBInstance) {
    st_db_ = DBInstance;
  }
  
	string GetDBST(){
	  return st_db_;
	}

  void SetDBQuality(string DBInstance) {
		quality_db_ = DBInstance;
	}

	string GetDBQuality() {
		return quality_db_;
	}

  int GetAllPvTh() {
    return all_pv_th_;
  }
  int GetAllClickTh() {
    return all_click_th_;
  }
  int GetNoGenderPvTh() {
    return no_gender_pv_th_;
  }
  int GetNoGenderClickTh() {
    return no_gender_click_th_;
  }
  int GetNoStagePvTh() {
    return no_stage_pv_th_;
  }
  int GetNoStageClickTh() {
    return no_stage_click_th_;
  }
  int GetSpidPvTh() {
    return spid_pv_th_;
  }
  int GetSpidClickTh() {
    return spid_click_th_;
  }
  int GetTwoFeaturePvTh() {
    return two_feature_pv_th_;
  }
  int GetTwoFeatureClickTh() {
    return two_feature_click_th_;
  }
  Ice::Long GetBaseTestImpr() {
    return test_base_impr_;
  }
  Ice::Long GetRealTestImpr() {
    return test_real_impr_;
  }
  double GetTestImprRatio() {
    return test_impr_ratio_;
  }
 
  double GetFreshAdCtrThr(int state){
    if (state == 1){
      return fresh_ad_722_th_;
    }
    else if (state == 2){
      return fresh_ad_721_th_;
    }
    else if (state == 3){
      return fresh_ad_632_th_;
    }
    else if (state == 4){
      return fresh_ad_631_th_;
    }
    else if (state == 5){
      return fresh_ad_702_th_;
    }
     else if (state == 6){
      return fresh_ad_701_th_;
    }
  }
  int GetStatePvTh() {
    return state_pv_th_;
  }
  int GetStateClickPvTh() {
    return state_click_th_;
  }
  double GetStateTestImprRatio(int state) {
    if(state == 1) {
      return state1_test_impr_ratio_;
    } else if(state == 2) {
      return state2_test_impr_ratio_;
    } else if(state == 3) {
      return state3_test_impr_ratio_;
    }
    return 0.05;
  }

  int GetFreshPvThr() {
    return fresh_pv_thr_;
  }
 
  int GetFreshClickThr() {
    return fresh_click_thr_;
  }

  int GetFreshTestDay() {
    return fresh_ad_test_day_;
  }  
  double GetFreshEcpmRatio() {
    return ecpm_ratio_;
  }
  int GetAppAllPvTh() {
    return app_all_pv_th_;
  }
  int GetAppAllClickTh() {
    return app_all_click_th_;
  }
  int GetAppNoGenderPvTh() {
    return app_no_gender_pv_th_;
  }
  int GetAppNoGenderClickTh() {
    return app_no_gender_click_th_;
  }
  int GetAppNoStagePvTh() {
    return app_no_stage_pv_th_;
  }
  int GetAppNoStageClickTh() {
    return app_no_stage_click_th_;
  }
  int GetAppSpidPvTh() {
    return app_spid_pv_th_;
  }
  int GetAppSpidClickTh() {
    return app_spid_click_th_;
  }
  int GetAppTwoFeaturePvTh() {
    return app_two_feature_pv_th_;
  }
  int GetAppTwoFeatureClickTh() {
    return app_two_feature_click_th_;
  }
  string quality_db_name(){
    return quality_db_name_;
  }
  int ctr_discount_type(){
    return ctr_discount_type_;
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
  int app_all_pv_th_;
  int app_all_click_th_;
  int app_no_gender_pv_th_;
  int app_no_gender_click_th_;
  int app_no_stage_pv_th_;
  int app_no_stage_click_th_;
  int app_spid_pv_th_;
  int app_spid_click_th_;
  int app_two_feature_pv_th_;
  int app_two_feature_click_th_;


  double fresh_ad_722_th_;
  double fresh_ad_721_th_;
  double fresh_ad_632_th_;
  double fresh_ad_631_th_;
  double fresh_ad_702_th_;
  double fresh_ad_701_th_;
  Ice::Long test_base_impr_;
  Ice::Long test_real_impr_;
  double test_impr_ratio_;
  string fresh_db_name_;
  
  int state_pv_th_;
  int state_click_th_;
  int fresh_pv_thr_;
  int fresh_click_thr_;
  int fresh_ad_test_day_;
  double state1_test_impr_ratio_;
  double state2_test_impr_ratio_;
  double state3_test_impr_ratio_;
  double ecpm_ratio_;
  string quality_db_name_;
  int ctr_discount_type_;

	string st_db_;
  string quality_db_; 
  string db_source_stat_;
  string db_source_statbigt_;
  string db_source_ads_;
  string db_source_ad_test_;
  string db_source_audit_;
	map<string, string> map_;
public:
  class ConfigLoadTimer : public MyUtil::Timer{
  public:
    ConfigLoadTimer(int time = 0):Timer(time){}
    virtual void handle() {
      MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(60 * 1000));//1鍒嗛挓鏇存敼涓�
      AdDemoConfig::instance().LoadConfig();
    }
  };
};

} // end namespace ad
} // end namespace xce
#endif /* AD_CONFIG_H_ */
