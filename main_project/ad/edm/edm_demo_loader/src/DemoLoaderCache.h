/*
 * DemoLoader.h
 *
 *  Created on: sep 20, 2012
 *      Author: yu.fu 
 */

#ifndef EDMDEMOLOADERCACHE_H_
#define EDMDEMOLOADERCACHE_H_

#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <Ice/Ice.h>
#include "Date.h"
#include "Singleton.h"
#include "FeedMemcProxy/client/user_profile_client.h"
#include "EdmDemoQuality.h"
#include "TaskManager.h"
#include "IceLogger.h"
#include "ServiceI.h"
#include "Date.h"
#include "client/linux/handler/exception_handler.h"
#include "EdmConfig.h"
#include "Util.h"
#include <netdb.h>
#include <connection.h>
#include <query.h>

namespace xce{
namespace ad{
using namespace com::xiaonei::xce;
using namespace std;
using namespace MyUtil;
using namespace mysqlpp;

//TODO add for edm
const int edm_no_gender = 1;
const int edm_no_stage = 2;
const int edm_spid_type = 3;
const int edm_all_type = 4;
const int edm_two_feature = 5;
//TODO add for edm to use Ad
const int ad_edm_no_gender = 11;
const int ad_edm_no_stage = 12;
const int ad_edm_spid_type = 13;
const int ad_edm_all_type = 14;
const int ad_edm_two_feature = 15;

const int cpc_type = 0;
const int cpm_type = 1;
const int cpa_type = 2;

//TODO 计算广告在Ad各个广告位的点击率时，只统计如下广告位上的数据
const string ad_zone_list = "(100000000001,100000000063,100000000065,100000000093,100000000070,100000000071,100000000072,100000000073,100000000075)";

class DemoLoaderCache :public EdmDemoQualityManager, public MyUtil::Singleton<DemoLoaderCache> {
	public:
		DemoLoaderCache(){
		}
		virtual EdmQualityMap GetEdmPool(const ::Ice::Current& = ::Ice::Current());
		virtual EdmQualityMap GetAdEdmPool(const ::Ice::Current& = ::Ice::Current());
		void LoadEdmPool();
		void LoadAdEdmPool();

	private:
		void DataValidityCheck(EdmQualityMap& res_pool);
		void LoadPool(const string & dbsource, const string& sql, const int& type, EdmQualityMap& res_pool);

		string GetKey(Ice::Long group_id,int gender,int stage){
			ostringstream sin;
			sin << group_id << "_" << gender << "_" << stage;
			return sin.str();
		}

		bool IsCredible(int pv, int click, int type) {
			int pv_th = 10000;
			int click_th = 10;
			switch(type) {
				case edm_no_gender:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(EDM_NO_GENDER_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(EDM_NO_GENDER_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;
				case edm_no_stage:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(EDM_NO_STAGE_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(EDM_NO_STATE_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;
				case edm_spid_type:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(EDM_SPID_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(EDM_SPID_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;
				case edm_all_type:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(EDM_ALL_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(EDM_ALL_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;
				case edm_two_feature:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(EDM_TWO_FEATRUE_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(EDM_TWO_FEATURE_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;
				case ad_edm_no_gender:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_NO_GENDER_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_NO_GENDER_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;  
				case ad_edm_no_stage:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_NO_STAGE_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_NO_STAGE_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;  
				case ad_edm_spid_type:
					pv_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_SPID_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_SPID_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;                                    
				case ad_edm_all_type:                     
					pv_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_ALL_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_ALL_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;                                    
				case ad_edm_two_feature:                  
					pv_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_TWO_FEATURE_PV_TH);
					click_th = EdmDemoConfig::instance().GetEdmConfig(AD_EDM_TWO_FEATURE_CLICK_TH);
					//MCE_DEBUG("IsCredbile,type:" << type << "EdmType,pv:" << pv_th  <<",click:" << click_th);
					break;
				default:
					MCE_WARN("IsCredblie,unknown type:" << type);
					pv_th = 10000;
					click_th = 10;
			}
			return (pv > pv_th) || (click > click_th);
		}

	private:
		EdmQualityMap edm_pool_;
		EdmQualityMap ad_edm_pool_;
		static const size_t DAYCOUNT = 10;
		static const size_t HOURCOUNT = 24;
		static const int NO_USE = -1;
		IceUtil::RWRecMutex mutex_;
};

class InitTask : virtual public MyUtil::Task{
	public:
		InitTask(){}
		virtual void handle();
};

class ReloadTask : virtual public MyUtil::Task{
	public:
		ReloadTask(){}
		virtual void handle();
};

}
}

#endif /* DEMOLOADERCACHE_H_ */
