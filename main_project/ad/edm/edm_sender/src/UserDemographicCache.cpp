/*
 * UserDemographicCache.cpp
 *
 *  Created on: sep 22, 2012
 *      Author: yu.fu 
 */

#include "UserDemographicCache.h"
#include "EdmDemoQualityReplicaAdapter.h"
#include <connection.h>
#include <query.h>
#include <string>
#include <algorithm> 
#include <cmath>
#include "ParametersController.h"

using namespace mysqlpp;
using namespace std;
bool UserDemographicCache::GetAdGroupCtrFromEdm(const UserProfile & user_profile, const Ice::Long& group_id,double& edmctr){
	pair<double,int> result = GetGroupCtr(user_profile,group_id,EDM_CTR);
	if(result.second == 3){
	 return false;
	}
	edmctr = result.first;
  MCE_DEBUG("UserDemographicCache::GetEdmGroupCtrFromAd return true groupid:" << group_id << "ctr:" << edmctr);
	return true;
}

bool UserDemographicCache::GetAdGroupCtrFromAd(const UserProfile& user_profile,const Ice::Long& group_id, double& adctr){
	pair<double,int> result = GetGroupCtr(user_profile,group_id,AD_CTR);
	if(result.second == 3){
	 return false;
	}
	adctr  = result.first;
  MCE_DEBUG("UserDemographicCache::GetAdGroupCtrFromAd return true groupid:" << group_id << "ctr:" << adctr);
	return true;
}

pair<double,int> UserDemographicCache::GetGroupCtr(const UserProfile& user_profile, Ice::Long adGroupID, int ctrType){
  MCE_DEBUG("UserDemographicCache::GetGroupCtr Type:" << ctrType << "adgroupID:" << adGroupID);
	int gender = user_profile.gender();
	int stage = user_profile.stage();
	for (int stateIDX = 0;stateIDX < 4;stateIDX++){
		if (stateIDX == 0){
			double ctr = GetCtr(adGroupID,gender,stage,ctrType);
			if (ctr >= 0.0){
        MCE_DEBUG("UserDemographicCache::GetGroupCtr Type:" << ctrType << "adgroupID:" << adGroupID << " ,ctr:" << ctr << ",stateIDX" << stateIDX);
				return pair<double,int >(ctr,stateIDX);
			}
		}else if(stateIDX == 1){
			double ctr_gender = GetCtr(adGroupID,gender,NO_USE,ctrType);
			double ctr_stage = GetCtr(adGroupID,NO_USE,stage,ctrType);
			if (ctr_gender >= 0.0 and ctr_stage < 0){
				double ctr_gender_stage = GetTwoFeatureCtr(adGroupID,gender,stage,ctrType);
				if (ctr_gender_stage >= 0.0){
					double alpha_gender = ctr_gender_stage / ctr_gender;
					if(fabs(alpha_gender - 1) < ALPHA_THR) {
						return pair<double,int> (ctr_gender,stateIDX*10 + FLAG_GENDER);
					}
				}
			}else if (ctr_gender < 0 and ctr_stage >= 0.0){
				double ctr_gender_stage = GetTwoFeatureCtr(adGroupID,gender,stage,ctrType);
				if (ctr_gender_stage >= 0.0){
					double alpha_stage = ctr_gender_stage / ctr_stage;
					if(fabs(alpha_stage - 1) < ALPHA_THR) {
						return pair<double,int> (ctr_stage,stateIDX*10 + FLAG_STAGE);
					}
				}
			}else if (ctr_gender >= 0.0 and ctr_stage >= 0.0){
				double ctr_gender_stage = GetTwoFeatureCtr(adGroupID,gender,stage,ctrType);
				if (ctr_gender_stage >= 0.0){
					double alpha_gender = ctr_gender_stage / ctr_gender;
					double alpha_stage = ctr_gender_stage / ctr_stage;
					if (fabs(alpha_gender - 1) <= fabs(alpha_stage - 1)){
						if (fabs(alpha_gender - 1) < ALPHA_THR) {
							return pair<double, int> (ctr_gender,stateIDX*10 + FLAG_GENDER + 2);
						}
					}else{
						if (fabs(alpha_stage - 1) < ALPHA_THR) {
							return pair<double, int> (ctr_stage,stateIDX*10 + FLAG_STAGE + 2);
						}
					}
				}
			}
		}else if (stateIDX == 2){
			double ctr = GetCtr(adGroupID,NO_USE,NO_USE,ctrType);
			if (ctr >= 0.0){  
        MCE_DEBUG("UserDemographicCache::GetGroupCtr Type:" << ctrType << "adgroupID:" << adGroupID << " ,ctr:" << ctr << ",stateIDX" << stateIDX);
				return pair<double,int> (ctr,stateIDX);
			}
		}else {
      MCE_DEBUG("UserDemographicCache::GetGroupCtr Error  Type:" << ctrType << "adgroupID:" << adGroupID );
			return pair<double,int> (0.000001,3);   //默认值 线上  important
		}
	}
	return pair<double,int> (0.000001,3); //返回默认值 线上
}

void UserDemographicCache::Load() {
	EdmQualityMap edm_pool;
	EdmQualityMap ad_edm_pool;
	try{
		edm_pool = EdmDemoQualityReplicaAdapter::instance().GetEdmPool(EngineConfig::instance().server_index());
		ad_edm_pool =  EdmDemoQualityReplicaAdapter::instance().GetAdEdmPool(EngineConfig::instance().server_index());
	}catch (Ice::Exception& e) {
		MCE_WARN("UserDemoGraphicCache::Load --> call EdmDemoQualityAdapter ERR  line:"
				<< __LINE__ << " err:" << e); 
	} catch (std::exception& e) {
		MCE_WARN("UserDemoGrahpicCache::load --> call EdmDemoQualityAdapter ERR  line:"
				<< __LINE__ << " err:" << e.what());
	} 
	if (!ad_edm_pool.empty()){
		IceUtil::RWRecMutex::WLock lock(ad_mutex_);
		adgraphtable_.swap(ad_edm_pool);	
		MCE_INFO("[UserDemographicCache]: Load from DemoLoader by GetAdEdmPool. size is "  << adgraphtable_.size());
	}else{
		MCE_WARN("[UserDemographicCache]: Load from DemoLoader by GetAdEdmPool. pool is Empty");          
	}

	if(!edm_pool.empty()){
		IceUtil::RWRecMutex::WLock lock(edm_mutex_);
		edmgraphtable_.swap(edm_pool);
		MCE_INFO("[UserDemographicCache]: Load from DemoLoader by GetEdmPool, size:"<<edmgraphtable_.size());
	}else{
		MCE_INFO("[UserDemographicCache]: Load from DemoLoader by GetEdmPool,pool is Empty");
	}
	ShowAllCtr();
}

double UserDemographicCache::GetTwoFeatureCtr(Ice::Long adGroupID ,int gender,int stage,int ctrType){
	double ctr_feature = GetCtr(adGroupID,gender,stage,ctrType);
	if (ctr_feature < 0.0){
		ctr_feature = GetCtr(NO_USE,gender, stage,ctrType);
		if (ctr_feature < 0.0){
			ctr_feature = -1.0;
		}
	}
	return ctr_feature;
}

void UserDemographicCache::ShowAllCtr(){
	EdmQualityMap::iterator it = adgraphtable_.begin();
	for(; it!=adgraphtable_.end();++it){
		MCE_INFO("[ShowAllCtr] AdCtr key:"<< it->first << " value:"<< it->second);
	}
	MCE_INFO("[ShowAllCtr] AdCtr size:"<< adgraphtable_.size());
	it = edmgraphtable_.begin();
	for(; it!=edmgraphtable_.end();++it){
		MCE_INFO("[ShowAllCtr] EdmCtr key:"<< it->first << " value:"<< it->second);
	}
	MCE_INFO("[ShowAllCtr] EdmCtr size:"<< edmgraphtable_.size());
}

