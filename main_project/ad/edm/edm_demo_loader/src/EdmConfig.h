/*
 * AdConfig.h
 *
 *  Created on: 2012-09-20
 *      Author: yu.fu
 */

#ifndef EDM_CONFIG_H_
#define EDM_CONFIG_H_

#include "Util.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "Evictor.h"
#include "IceLogger.h"
#include <map>

namespace xce {
namespace ad {

#define  EDM_NO_GENDER_PV_TH 					"edm_no_gender_pv_th"
#define  EDM_NO_GENDER_CLICK_TH 			"edm_no_gender_click_th"
#define  EDM_NO_STAGE_PV_TH 					"edm_no_stage_pv_th"
#define  EDM_NO_STATE_CLICK_TH 				"edm_no_stage_click_th"
#define  EDM_SPID_PV_TH 							"edm_spid_pv_th"
#define  EDM_SPID_CLICK_TH 						"edm_spid_click_th"
#define  EDM_ALL_PV_TH 								"edm_all_pv_th"
#define  EDM_ALL_CLICK_TH 						"edm_all_click_th"
#define  EDM_TWO_FEATRUE_PV_TH 				"edm_two_feature_pv_th"
#define  EDM_TWO_FEATURE_CLICK_TH 		"edm_two_feature_click_th"

#define  AD_EDM_NO_GENDER_PV_TH				"ad_edm_no_gender_pv_th"
#define  AD_EDM_NO_GENDER_CLICK_TH		"ad_edm_no_gender_click_th"
#define  AD_EDM_NO_STAGE_PV_TH				"ad_edm_no_stage_pv_th"
#define  AD_EDM_NO_STAGE_CLICK_TH			"ad_edm_no_stage_click_th"
#define  AD_EDM_SPID_PV_TH						"ad_edm_spid_pv_th"
#define  AD_EDM_SPID_CLICK_TH					"ad_edm_spid_click_th"
#define  AD_EDM_ALL_PV_TH							"ad_edm_all_pv_th"
#define  AD_EDM_ALL_CLICK_TH					"ad_edm_all_click_th"
#define  AD_EDM_TWO_FEATURE_PV_TH			"ad_edm_two_feature_pv_th"
#define  AD_EDM_TWO_FEATURE_CLICK_TH	"ad_edm_two_feature_click_th"

class EdmDemoConfig : public MyUtil::Singleton<EdmDemoConfig> {
	public:
		EdmDemoConfig():st_ad_db_("ad_st"),	st_edm_db_("statdb.edb"){
		}

		bool LoadConfig();
		void SetAdDBST(string DBInstance) {
			st_ad_db_ = DBInstance;
		}
		string GetAdDBST(){
			return st_ad_db_;
		}
		void SetEdmDBST(string DBInstance) {
			st_edm_db_ = DBInstance;
		}
		string GetEdmDBST() {
			return st_edm_db_;
		}

		int GetEdmConfig(string key){
			IceUtil::RWRecMutex::RLock lock(mutex_);
			map<string,int>::iterator it = edm_pv_click_th_.find(key);
			if(it != edm_pv_click_th_.end()){
				return it->second;
			}
			MCE_WARN("[EdmConfig::GetEdmCOnfig] Not Config The threshold :" << key);
			return 100;
		}

	private:
		//TODO add for edm
		string st_ad_db_;
		string st_edm_db_; 
		map<string,int> edm_pv_click_th_;
		IceUtil::RWRecMutex mutex_;
	public:
		class ConfigLoadTimer : public MyUtil::Timer{
			public:
				ConfigLoadTimer(int time = 0):Timer(time){}
				virtual void handle() {
					MyUtil::TaskManager::instance().schedule(new ConfigLoadTimer(3 * 60 * 1000));//3分钟重新加载一次
					EdmDemoConfig::instance().LoadConfig();
				}
		};
};

} // end namespace ad
} // end namespace xce
#endif /* EDM_CONFIG_H_ */
