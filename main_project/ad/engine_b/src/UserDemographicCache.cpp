/*
 * UserDemographicCache.cpp
 *
 *  Created on: Apr 19, 2011
 *      Author: kingsfield
 */

#include "UserDemographicCache.h"
#include "AdCreativeQualityReplicaAdapter.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <connection.h>
#include <query.h>
#include <string>
#include <algorithm> 
#include <cmath>
#include <sstream>

using namespace mysqlpp;
using namespace std;

void UserDemographicCache::Load() {
  AdQualityMap demo_pool;
  int server_index = EngineConfig::instance().getServerIndex();
  try{
    demo_pool = AdCreativeQualityReplicaAdapter::instance().GetDemoPool(server_index);
  }catch (Ice::Exception& e) {
    MCE_WARN("UserDemoGraphicCache::Load --> call AdCreativeQualityReplicaAdapter ERR  line:"
        << __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("UserDemoGrahpicCache::load --> call AdCreativeQualityReplicaAdapter ERR  line:"
        << __LINE__ << " err:" << e.what());
  } 
  if (demo_pool.size() > 0){
    IceUtil::RWRecMutex::WLock lock(mutex_);
    string befor_swap_max_time = boost::lexical_cast<string> ((Ice::Long) demographTable["max_time"]);
    demographTable.swap(demo_pool);
    string after_swap_max_time = boost::lexical_cast<string> ((Ice::Long) demographTable["max_time"]);
    MCE_INFO("[UserDemographicCache]: Load from GetDemoPool. size is "  << demographTable.size()
    <<",befor update,max_time:" <<befor_swap_max_time<< ",after update,max_time:" << after_swap_max_time);          
  }else{
    MCE_WARN("[UserDemographicCache]: Load from GetDemoPool. DemoPool  is empty DemoPoolsize = " << demo_pool.size()); 
  }
}


void UserDemographicCache::LoadFresh() {
  FreshAdQualityMap fresh_pool;
  int server_index = EngineConfig::instance().getServerIndex();
  try{
    fresh_pool = AdCreativeQualityReplicaAdapter::instance().GetFreshAdMap(server_index);
  }catch (Ice::Exception& e) {
    MCE_WARN("UserDemoGraphicCache::Load --> call AdCreativeQualityReplicaAdapter ERR  line:"
        << __LINE__ << " err:" << e); 
  } catch (std::exception& e) {
    MCE_WARN("UserDemoGrahpicCache::load --> call AdCreativeQualityReplicaAdapter ERR  line:"
        << __LINE__ << " err:" << e.what());
  } 
  if (fresh_pool.size() > 0){
    IceUtil::RWRecMutex::WLock lock(mutex_);
    freshCreativeMap_.swap(fresh_pool);
  }else{
    MCE_WARN("[UserDemographicCache]: Load from GetDemoPool. DemoPool  is empty DemoPoolsize = " << fresh_pool.size()); 
  }
}

pair<double,int> UserDemographicCache::GetDemoCtr(Ice::Long zone_id, Ice::Long sponserID, Ice::Long adGroupID, int pos_id, Ice::Long creative_id, int gender,int stage){
  ostringstream   oss;
  oss << zone_id;
	double ctr = GetCtr(oss.str(),sponserID,adGroupID,pos_id, creative_id,  NO_USE,NO_USE);
	MCE_DEBUG("creative_id:" << creative_id << " ctr" << ctr);
	if(ctr > 0.0) {
		return pair<double,int> (ctr,2);
	}
	return pair<double,int> (0.000001,3);
}

Ice::Long  UserDemographicCache::GetRandomFreshCreative(const set<Ice::Long>& creative_ids){
	vector<Ice::Long>   fresh_cids;
	for(set<Ice::Long>::const_iterator it = creative_ids.begin(); it != creative_ids.end(); ++it){
		if(freshCreativeMap_["reserved"].find(*it) != freshCreativeMap_["reserved"].end()){
			fresh_cids.push_back(*it);
		}
	}
	if(fresh_cids.empty()){
		return 0;
	}else{
		return fresh_cids[rand()%fresh_cids.size()];
	}
}

 
