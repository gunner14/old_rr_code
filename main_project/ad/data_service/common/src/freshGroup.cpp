/*
 * =====================================================================================
 *
 *       Filename:  freshGroup.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月03日 10时05分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "freshGroup.h"
#include "indexConfig.h"

using namespace std;
using namespace xce::ad;

FreshAdQualityMap FreshAdGroupsCache::GetFreshGroups(){
	FreshAdQualityMap result;
	{
		IceUtil::RWRecMutex::RLock lock(mutex_);    
		result = fresh_groups_;
	}
	return result;
}

std::string FreshAdGroupsCache::GetFreshKey(Ice::Long zone_id, int pos) {
	Ice::Long key = zone_id * 10 + pos;
	ostringstream sin;
	sin << key;
	return sin.str();

}

void FreshAdGroupsCache::getFreshGroupsByAdzone(long adzone, int pos, std::map<long,double> &gids){

	//std::string key = GetFreshKey(adzone, pos);
	std::string key = "reserved";
	FreshAdQualityMap::iterator it = fresh_groups_.find(key);	
	if(it != fresh_groups_.end()){
		//FreshAdQualitySet& f_groups = it->second;
    gids = it->second;
		//for(FreshAdQualitySet::iterator it2 = f_groups.begin(); it2 != f_groups.end(); ++it2){
		//	gids.push_back(it2->first);
		//}
	}
}

void FreshAdGroupsCache::Load() {
	MCE_INFO("[Fresh:],FreshAdGroupsCache::Load start");
	FreshAdQualityMap pool;
	try {
		int server_index = IndexConfig::instance().getServerIndex();
		pool = AdDemoQualityAdapter::instance().GetFreshAdMap(server_index);
	}catch (Ice::Exception& e) {
		MCE_WARN("FreshAdGroupsCache::Load --> call AdDemoQualityNAdapter ERR  line:"
				<< __LINE__ << " err:" << e); 
	} catch (std::exception& e) {
		MCE_WARN("FreshAdGroupsCache::load --> call AdDemoQualityNAdapter ERR  line:"
				<< __LINE__ << " err:" << e.what());
	}

	IceUtil::RWRecMutex::WLock lock(mutex_);
	fresh_groups_.swap(pool);
	MCE_INFO("[Fresh:]: Load stop, GetFreshAdMap size is:"  << fresh_groups_.size());          
}

