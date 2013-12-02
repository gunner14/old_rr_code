#include "CachePool.h"
#include "InvertedIndex.h"
#include "Monitor.h"
#include "Config.h"
#include "Handler.h"

using namespace xce::ad;

AdMobMonitorI::AdMobMonitorI() {
	cache_dict_[AdMobPool::MEMBER] = &MemberPool::instance();
	cache_dict_[AdMobPool::CAMPAIGN] = &CampaignPool::instance();
	cache_dict_[AdMobPool::GROUP] = &GroupPool::instance();
	cache_dict_[AdMobPool::ZONE] = &ZonePool::instance();
	cache_dict_[AdMobPool::USERBIND] = &UserBindPool::instance();
	cache_dict_[AdMobPool::PLATFORM] = &PlatformMap::instance();
	cache_dict_[AdMobPool::BRAND3G] = &Brand3GMap::instance();
	cache_dict_[AdMobPool::RESOLUTION] = &ResolutionMap::instance();
	cache_dict_[AdMobPool::RESOLUTION_WIDGET] = &ResolutionMatcher::instance();
}

string AdMobMonitorI::GetPoolValue(const int pool_name, const long id, const Ice::Current&) {
	string result;
	if ( pool_name == AdMobPool::MEMBER ) {
		AdMemberPtr member = MemberPool::instance().GetAdMember( id );
		if (member) {
			result = member->ToString();
		}
	} else if ( pool_name == AdMobPool::CAMPAIGN ) {
		AdCampaignPtr campaign = CampaignPool::instance().GetAdCampaign( id );
		if (campaign) {
			result = campaign->ToString();
		}
	} else if ( pool_name == AdMobPool::GROUP ) {
		AdGroupPtr group = GroupPool::instance().GetAdGroup( id );
		if (group) {
			result = group->ToString();
		}
	} else {
		result = "The pool is not exist";
	}
	if( true == result.empty() ) {
		result = "Cannot find the id in the pool";
	}

	return result;
}

int AdMobMonitorI::GetPoolSize(const int pool_name, const Ice::Current&) {
	map<int, CachePool*>::iterator it = cache_dict_.begin();
	for (; it != cache_dict_.end(); ++it) {
		if ( (*it).first == pool_name ) {
			return (*it).second->Size();
		}
	}
	return 0;
}

string AdMobMonitorI::GetPoolAll(const int pool_name, const int size, const Ice::Current&) {
	map<int, CachePool*>::iterator it = cache_dict_.begin();
	string result;
	for (; it != cache_dict_.end(); ++it) {
		if ( (*it).first == pool_name ) {
			result = (*it).second->Print(size);
		}
	}
	if ( true == result.empty() ) {
		result = "The Pool is not Exist";
	}
	return result;
}

string AdMobMonitorI::GetBrandCandidateGroups(const xce::ad::AdMobReqPara& request,const int index_flag, const Ice::Current&) {
	MCE_INFO("AdMobMonitorI::GetBrandCandidateGroups --> Get a index test request");
	ostringstream result;
	HandlerPara handler_para(request);
	HandlerPtr parachecker = new ParaCheckHandler;
	parachecker->handle(handler_para);
	set<AdGroupPtr> groups;
	InvertedIndex::instance().Get(handler_para, request.adzoneId, BRAND_INDEX, groups);
	for( set<AdGroupPtr>::iterator it = groups.begin();it != groups.end(); ++it) {
		result << (*it)->group_id() << "  ";
	}
	return result.str();
}

string AdMobMonitorI::GetSelfCandidateGroups(const xce::ad::AdMobReqPara& request,const int index_flag, const Ice::Current&) {
	MCE_INFO("AdMobMonitorI::GetSelfCandidateGroups --> Get a index test request");
	ostringstream result;
	HandlerPara handler_para(request);
	HandlerPtr parachecker = new ParaCheckHandler;
	parachecker->handle(handler_para);
	set<AdGroupPtr> groups;
	InvertedIndex::instance().Get(handler_para, request.adzoneId, SELF_INDEX, groups);
	for( set<AdGroupPtr>::iterator it = groups.begin();it != groups.end(); ++it) {
		result << (*it)->group_id() << "  ";
	}
	return result.str();
}

/*
 *     Function: AdMobMonitorI::GetRotateGroups
 *  Description: 获取指定广告位的所有轮播广告
 */
string AdMobMonitorI::GetRotateGroups(const Ice::Long zone_id, const Ice::Current&) {
	MCE_INFO("AdMobMonitorI::GetRotateGroups --> Get a rotate groups test");
	ostringstream result;
	map<AdGroupPtr, int> groups;
	AdZonePtr zone = NULL;
	zone = ZonePool::instance().GetAdZone(zone_id);
	if ( zone ) {
		zone->GetAllRotateGroups(groups);
	}
	result << "Rotate List Size:" << groups.size() << "\n";
	map<AdGroupPtr, int>::const_iterator it = groups.begin();
	for(; it != groups.end(); ++it) {
		if ( it->first ) {
			result << it->first->group_id() << ":" << it->second << " ";
		}
	}
	result << "\n";
	return result.str();
}

/*
 *     Function: AdMobMonitorI::GetSelfIndexInfo
 *  Description: 品牌索引单元测试接口，获取指定索引的全部内容
 */
string AdMobMonitorI::GetBrandIndexInfo(const int index, const Ice::Long zone, const Ice::Current&) {
	//不是索引枚举值，直接返回
	if ( AdMobIndex::MAX <= index ) {
		return "Not a index type";
	}
	map<Ice::Long, DimIndexPtr>::const_iterator it;
	DimIndexPtr dim_index;
	{
		IceUtil::RWRecMutex::RLock lock(InvertedIndex::instance().mutex_);
		map<Ice::Long, DimIndexPtr>& brand_index = InvertedIndex::instance().zone_brand_index_;
		it = brand_index.find(zone);
		if (it != brand_index.end()) {
			dim_index = it->second;
		}
	}
	string result;
	if ( dim_index ) {
		result = dim_index->index_dict_[index]->Print();
	}
	return result;
}

/*
 *     Function: AdMobMonitorI::GetSelfIndexInfo
 *  Description: 中小索引单元测试接口，获取指定索引的全部内容
 */
string AdMobMonitorI::GetSelfIndexInfo(const int index, const Ice::Long zone, const Ice::Current&) {
	//不是索引枚举值，直接返回
	if ( AdMobIndex::MAX <= index ) {
		return "Not a index type";
	}
	map<Ice::Long, DimIndexPtr>::const_iterator it;
	DimIndexPtr dim_index;
	{
		IceUtil::RWRecMutex::RLock lock(InvertedIndex::instance().mutex_);
		map<Ice::Long, DimIndexPtr>& self_index = InvertedIndex::instance().zone_self_index_;
		it = self_index.find(zone);
		if (it != self_index.end()) {
			dim_index = it->second;
		}
	}
	string result;
	if ( dim_index ) {
		result = dim_index->index_dict_[index]->Print();
	}
	return result;
}

/*
 *     Function: AdMobMonitorI::GetTargetGroups
 *  Description: 索引单元测试接口，input中选定用户的定向条件，返回之中返回倒排索引中的候选广告
 */
AdMobTargetOutputSeq AdMobMonitorI::GetTargetGroups(const xce::ad::AdMobTargetInput& input, const Ice::Current&) {
	MCE_INFO("AdMobMonitorI::GetTargetGroups --> Get a Test Request , uid:" << input.uid);
	AdMobTargetOutputSeq results;
	HandlerPara para;
	UserProfile& profile = para.userProfile_;
	AdMobReqPara& request = para.request_;

	request.uid = input.uid;
	request.adzoneId = input.zoneid;
	request.netStatus = input.netStatus;
	request.client.model = input.model;
	request.client.osVersion = input.osVersion;
	request.client.screenSize = input.screenSize;
	profile.set_gender(input.gender);
	profile.set_stage(input.stage);
	profile.set_age(input.age);
	profile.set_ip_area(input.ipArea);
	profile.set_school(atol(input.school.c_str()));
	profile.set_current_area(input.currentArea);
	request.locationRealTime.longitude = input.lbsx;
	request.locationRealTime.latitude = input.lbsy;

	set<AdGroupPtr> candicates;
	if ( input.type == BRAND_INDEX ) {
		InvertedIndex::instance().Get(para, input.zoneid, BRAND_INDEX, candicates);
	} else {
		InvertedIndex::instance().Get(para, input.zoneid, SELF_INDEX, candicates);
	} 
	for (set<AdGroupPtr>::const_iterator it = candicates.begin(); it != candicates.end(); ++it) {
		AdMobTargetOutput group;
		group.groupID = (*it)->group_id();
		group.memberID = (*it)->member_id();
		group.campaignID = (*it)->campaign_id();
		group.bidUnitID = (*it)->bid_unit_id();
		group.transType = (*it)->trans_type();
		group.memberCategory = (*it)->member_category();
		results.push_back(group);
	}
	return results;
}

