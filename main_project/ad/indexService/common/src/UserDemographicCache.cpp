/*
 * UserDemographicCache.cpp
 *
 *  Created on: Apr 19, 2011
 *      Author: kingsfield
 */

#include "UserDemographicCache.h"
#include "AdDemoQualityAdapter.h"
#include <connection.h>
#include <query.h>
#include <string>
#include   <algorithm> 
#include "indexConfig.h"
#include <cmath>

using namespace mysqlpp;
using namespace std;

void UserDemographicCache::Load() {
  AdQualityMap demo_pool;
  AdQualityMap app_pool;
  int server_index = IndexConfig::instance().getServerIndex();
  try{
    demo_pool = AdDemoQualityAdapter::instance().GetDemoPool(server_index);
    app_pool =  AdDemoQualityAdapter::instance().GetAppPool(server_index);
  }catch (Ice::Exception& e) {
    MCE_WARN("UserDemoGraphicCache::Load --> call AdDemoQualityAdapter ERR  line:"
        << __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("UserDemoGrahpicCache::load --> call AdDemoQualityAdapter ERR  line:"
        << __LINE__ << " err:" << e.what());
  } 
  if (demo_pool.size() > 0){
    if (app_pool.size() > 0) {
      demo_pool.insert(app_pool.begin(),app_pool.end());
    } else {    
      MCE_INFO("[UserDemographicCache]: Load from GetDemoPool. AppPool is empty AppPoolSize = " << app_pool.size() << "DemoPoolsize = " << demo_pool.size());          
    }
    IceUtil::RWRecMutex::WLock lock(mutex_);
    string befor_swap_max_time = boost::lexical_cast<string> ((Ice::Long) demographTable["max_time"]);
    demographTable.swap(demo_pool);
    string after_swap_max_time = boost::lexical_cast<string> ((Ice::Long) demographTable["max_time"]);
    
    MCE_INFO("[UserDemographicCache]: Load from GetDemoPool. size is "  << demographTable.size()
    <<",befor update,max_time:" <<befor_swap_max_time<< ",after update,max_time:" << after_swap_max_time);          
  }else{
    MCE_WARN("[UserDemographicCache]: Load from GetDemoPool. DemoPool and AppPool is empty DemoPoolsize = " << demo_pool.size() << "AppPoolSize = " << app_pool.size());          
  }
}

pair<double,int> UserDemographicCache::GetDemoCtr(string zone_id, Ice::Long sponserID , Ice::Long adGroupID , int pos_id , int gender,int stage,int uid){
	//double ALPHA_THR =  EngineConfig::instance().GetAlphaThreshold();
/*	double ALPHA_THR = 2.5;
	//MCE_DEBUG("[UserDemographicCache]: Get Config. alpha_thr is  " << ALPHA_THR);          
	for (int stateIDX = 0;stateIDX < 4;stateIDX ++){
		if (stateIDX == 0){
			double ctr = GetCtr(zone_id,sponserID,adGroupID,pos_id,gender,stage);
			if (ctr >= 0.0){
				//MCE_DEBUG("[DemoDebug]: good case 0: "<< " zoneId=" << zone_id << " adGroupID=" << adGroupID << " sponserID=" << sponserID << " gender=" << gender << " stage=" << stage << " confidend="  << ctr);            
				//MY_MCE_DEBUG(zone_id,sponserID,adGroupID,pos_id,gender,stage,ctr,uid,stateIDX);
				return pair<double,int >(ctr,stateIDX);
			}
		}
		else if (stateIDX == 1){
			double ctr_gender = GetCtr(zone_id,sponserID,adGroupID,pos_id,gender,NO_USE);
			double ctr_stage = GetCtr(zone_id,sponserID,adGroupID,pos_id,NO_USE,stage);
			if (ctr_gender >= 0.0 and ctr_stage < 0){
				//MCE_DEBUG("[DemoDebug]: good case 1.0: "<< " zoneId=" << zone_id << " adGroupID=" << adGroupID << " sponserID=" << sponserID << " gender=" << gender << " stage=" << stage << " confidend="  << confident_gender); 
				double ctr_gender_stage = GetTwoFeatureCtr(zone_id,sponserID,adGroupID,pos_id,gender,stage);
				if (ctr_gender_stage >= 0.0){
					double alpha_gender = ctr_gender_stage / ctr_gender;
					if(abs(alpha_gender - 1) < ALPHA_THR) {
						//MY_MCE_DEBUG(zone_id,sponserID,adGroupID,pos_id,gender,NO_USE,ctr_gender,uid,stateIDX);
						return pair<double,int> (ctr_gender,stateIDX*10 + FLAG_GENDER);
					}
				}
			}
			else if (ctr_gender < 0 and ctr_stage >= 0.0){
				//MCE_DEBUG("[DemoDebug]: good case 1.1: "<< " zoneId=" << zone_id << " adGroupID=" << adGroupID << " sponserID=" << sponserID << " gender=" << gender << " stage=" << stage << " confidend="  << confident_stage); 
				double ctr_gender_stage = GetTwoFeatureCtr(zone_id,sponserID,adGroupID,pos_id,gender,stage);
				if (ctr_gender_stage >= 0.0){
					double alpha_stage = ctr_gender_stage / ctr_stage;
					if(abs(alpha_stage - 1) < ALPHA_THR) {
						//MY_MCE_DEBUG(zone_id,sponserID,adGroupID,pos_id,NO_USE,stage,ctr_stage,uid,stateIDX);
						return pair<double,int> (ctr_stage,stateIDX*10 + FLAG_STAGE);
					}
				}
			}
			else if (ctr_gender >= 0.0 and ctr_stage >= 0.0){
				double ctr_gender_stage = GetTwoFeatureCtr(zone_id,sponserID,adGroupID,pos_id,gender,stage);
				if (ctr_gender_stage >= 0.0){
					double alpha_gender = ctr_gender_stage / ctr_gender;
					double alpha_stage = ctr_gender_stage / ctr_stage;
					if (abs(alpha_gender - 1) <= abs(alpha_stage - 1)){
						if (abs(alpha_gender - 1) < ALPHA_THR) {
							return pair<double, int> (ctr_gender,stateIDX*10 + FLAG_GENDER + 2);
						}
					}else{
						if (abs(alpha_stage - 1) < ALPHA_THR) {
							return pair<double, int> (ctr_stage,stateIDX*10 + FLAG_STAGE + 2);
						}
					}
				}
			}
		}
		else if (stateIDX == 2){
			double ctr = GetCtr(zone_id,sponserID,adGroupID,pos_id,NO_USE,NO_USE);
			if (ctr >= 0.0){
				//MCE_DEBUG("[DemoDebug]: good case 2: "<< " zoneId=" << zone_id << " adGroupID=" << adGroupID << " sponserID=" << sponserID << " gender=" << gender << " stage=" << stage << " confidend="  << confident); 
				//MY_MCE_DEBUG(zone_id,sponserID,adGroupID,pos_id,NO_USE,NO_USE,ctr,uid,stateIDX);
				return pair<double,int> (ctr,stateIDX);
			}
		} else {
			//MCE_DEBUG("[DemoDebug]: bad case: "<< " zoneId=" << zone_id << " adGroupID=" << adGroupID << " sponserID=" << sponserID << " gender=" << gender << " stage=" << stage << " impr=" << value.first << " click" << value.second);
			//MY_MCE_DEBUG(zone_id,sponserID,adGroupID,pos_id,NO_USE,NO_USE,0.0,uid,stateIDX);
			return pair<double,int> (0.000001,3);   //默认值 线上  important
			//return pair<double,int> (0.0001,3);       //默认值 测试  important
		}
	}*/

	double ctr = GetCtr(zone_id,sponserID,adGroupID,pos_id,NO_USE,NO_USE);
	if(ctr > 0.0) {
		return pair<double,int> (ctr,2);
	}
	return pair<double,int> (0.000001,3);
}

void UserDemographicCache::MY_MCE_DEBUG(string zone_id, Ice::Long sponserID, Ice::Long adGroupID, int pos_id ,int gender,int stage, double ctr,int uid,int state){
  //if (uid == 338113787){
    MCE_DEBUG("[DemoDebug]: "<< " zone_id=" << zone_id << " sponser_id=" << sponserID << " gourp_id=" << adGroupID << " pos=" << pos_id  << " gender=" << gender << " stage=" << stage << " state=" << state  << " ctr=" << ctr);
  //}
}

double UserDemographicCache::GetTwoFeatureCtr(string zone_id, Ice::Long sponserID, Ice::Long adGroupID, int pos_id ,int gender,int stage){
  double ctr_feature = GetCtr(zone_id,sponserID,adGroupID,pos_id,gender,stage);
  if (ctr_feature < 0.0){
    ctr_feature = GetCtr(zone_id,sponserID, NO_USE, pos_id ,gender, stage);
    if (ctr_feature < 0.0){
      ctr_feature = -1.0;
    }
  }
  return ctr_feature;
}
