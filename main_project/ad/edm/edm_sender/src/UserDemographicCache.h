/*
 * UserDemographicCache.h
 *
 *  Created on: sep 22, 2012
 *      Author: yu.fu 
 */

#ifndef EDM_USERDEMOGRAPHICCACHE_H_
#define EDM_USERDEMOGRAPHICCACHE_H_

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
#include "IceLogger.h"

using namespace xce::ad;
using namespace std;

const double ALPHA_THR = 2.5;
const int EDM_CTR = 1;
const int AD_CTR = 2;

class UserDemographicCache : public MyUtil::Singleton<UserDemographicCache> {
	public:
		void Load();
		bool GetAdGroupCtrFromEdm(const UserProfile & user_profile, const Ice::Long& group_id,double& edmctr);
		bool GetAdGroupCtrFromAd(const UserProfile& user_profile,const Ice::Long& group_id, double& adctr);

		//TODO test function
		void ShowAllCtr();

	private:
		static const int NO_USE = -1;
		static const int FLAG_GENDER = 1;
		static const int FLAG_STAGE = 2;

		pair<double,int> GetGroupCtr(const UserProfile& user_profile, Ice::Long adGroupID, int ctrType);
		double GetTwoFeatureCtr(Ice::Long adGroupID, int gender,int stage, int ctrType);
		string GetKey(Ice::Long group_id, int gender, int stage){
			ostringstream sin;
			sin << group_id << "_" << gender << "_" << stage;
			return sin.str();
		}

		double GetCtr(Ice::Long group_id,int gender,int stage,int ctrType){
			string key = GetKey(group_id,gender,stage);
      //MCE_DEBUG("UserDemographicCache::GetCtr key:" << key << " type:" << ctrType << " edmctr size:" << edmgraphtable_.size() << " ad size:" << adgraphtable_.size() );
			EdmQualityMap::iterator iter;
			if(ctrType == EDM_CTR){
       // MCE_DEBUG("UserDemographicCache::GetEdmCtr key:" << key << " type:" << ctrType);
				IceUtil::RWRecMutex::RLock lock(edm_mutex_);    
				iter = edmgraphtable_.find(key);
				if(iter != edmgraphtable_.end()){
    //      MCE_DEBUG("UserDemographicCache::GetEdmCtr key:" << key << " value:"<< iter->second);
					return iter->second;
				}
			}else if(ctrType == AD_CTR){
      //  MCE_DEBUG("UserDemographicCache::GetAdCtr key:" << key << " type:" << ctrType);
				IceUtil::RWRecMutex::RLock lock(ad_mutex_);    
				iter = adgraphtable_.find(key);
				if(iter != adgraphtable_.end()){
     //     MCE_DEBUG("UserDemographicCache::GetAdCtr key:" << key << " value:"<< iter->second);
					return iter->second;
				}
			}
      MCE_DEBUG("UserDemographicCache::GetCtr error  key:" << key);
			return -1.0;
		}

	private:
		EdmQualityMap adgraphtable_;
		EdmQualityMap edmgraphtable_;
		IceUtil::RWRecMutex ad_mutex_;
		IceUtil::RWRecMutex edm_mutex_;

};

class UserDemographicCacheTimer : public MyUtil::Timer {
	public:
		UserDemographicCacheTimer() :
			Timer(3 * 60 * 1000) { //60分钟重新计算一次
			}
		virtual void handle() {
			UserDemographicCache::instance().Load();
		}
};


#endif /* USERDEMOGRAPHICCACHE_H_ */
