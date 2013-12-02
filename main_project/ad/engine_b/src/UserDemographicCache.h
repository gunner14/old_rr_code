/*
 * UserDemographicCache.h
 *
 *  Created on: Apr 19, 2011
 *      Author: kingsfield
 */

#ifndef USERDEMOGRAPHICCACHE_H_
#define USERDEMOGRAPHICCACHE_H_

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
#include "AdConfig.h"
#include "AdDemoQuality.h"
#include "IceLogger.h"
#include "engineTimer.h"

using namespace xce::ad;
using namespace std;

class UserDemographicCache : public MyUtil::Singleton<UserDemographicCache> {
public:
  pair<double,int> GetDemoCtr(Ice::Long zone_id,Ice::Long sponserID, Ice::Long adGroupID, int pos, Ice::Long creative_id, int gender,int stage);
  Ice::Long  GetRandomFreshCreative(const set<Ice::Long>& creative_ids);
  void Load();
	void LoadFresh();
private:
  static const int NO_USE = -1;
  static const int FLAG_GENDER = 1;
  static const int FLAG_STAGE = 2;
    
  string GetKey(string zone_id, Ice::Long sp_id ,Ice::Long group_id,int pos , Ice::Long creative_id, int gender,int stage){
    sp_id = sp_id * 10 + pos;
    group_id = group_id * 10 + pos;
    ostringstream sin;
    sin << zone_id <<"_"<< sp_id <<"_"<< group_id <<"_" << creative_id << "_"  <<gender<<"_"<<stage;
    return sin.str();
  }  

  string GetKey(Ice::Long zone_id, Ice::Long sp_id, Ice::Long group_id, int pos, Ice::Long creative_id, int gender,int stage){
    sp_id = sp_id * 10 + pos;
    group_id = group_id * 10 + pos;
    ostringstream sin;
    sin << zone_id <<"_"<< sp_id <<"_"<< group_id <<"_" << creative_id << "_" <<gender<<"_"<<stage;
    return sin.str();
  }  

  double GetCtr(string zone_id, Ice::Long sp_id ,Ice::Long group_id, int pos , Ice::Long creative_id, int gender,int stage){
    string key = GetKey(zone_id,sp_id,group_id,pos, creative_id, gender,stage);
    AdQualityMap::iterator iter;
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    iter = demographTable.find(key);
    if(iter != demographTable.end()){
			MCE_DEBUG("key = " << key << " CTR = " << iter->second);
			return iter->second;
    }else{
			MCE_DEBUG("key = " << key << " CTR = " << -1.0);
			return -1.0;
    }
  }

private:
	IceUtil::RWRecMutex mutex_;
	AdQualityMap demographTable;
	FreshAdQualityMap  freshCreativeMap_;
};

class UserDemographicCacheTimer : public EngineTimer{
public:
	UserDemographicCacheTimer(void *instance): EngineTimer(instance){}

	virtual void process(){
		UserDemographicCache *cache = (UserDemographicCache*)instance_;
		if(cache == NULL){
			MCE_WARN("UserDemographicCacheTimer::process get a NULL pointer!");
			return;
		}
		cache->Load();
	}

	virtual int interval() const {
		return 180;
	}
};

class FreshCreativeLoadTimer : public EngineTimer{
public:
	FreshCreativeLoadTimer(void *instance): EngineTimer(instance){}
  virtual void process(){
		UserDemographicCache *cache = (UserDemographicCache*)instance_;
		if(cache == NULL){
			MCE_WARN("UserDemographicCacheTimer::process get a NULL pointer!");
			return;
		}   
		cache->LoadFresh();
	}

  virtual int interval() const {
		return 180;
	}
};

#endif /* USERDEMOGRAPHICCACHE_H_ */
