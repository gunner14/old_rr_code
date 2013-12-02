/*
 * =====================================================================================
 *
 *       Filename:  db.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月18日 09时42分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "Date.h"
#include "index_load.h"
#include "ad/indexService/TIndex/src/adTranslator.h"
#include "ad/indexService/TIndex/src/targetingIndex.h"

using namespace xce::ad;
using namespace MyUtil;

void IndexServiceLoader::ShowVectorContent(const std::string& title,const IndexItem& item,int type){
	std::ostringstream outvector;
	copy(item.fields.begin(), item.fields.end(), std::ostream_iterator<std::string>(outvector, "|"));

	if(type == 0){
		MCE_INFO(title << "ItemContent table:" <<  item.table  << ", content:"<< outvector.str());
	}else if(type == 1){
		MCE_WARN(title << "ItemContent table:" <<  item.table  << ", content:"<< outvector.str());
	}
}

void IndexServiceLoader::LoadAdMembers(const std::vector<IndexItem> &items){
	int cnt = 0;
	std::vector<IndexItem>::const_iterator it = items.begin();
	for(; it != items.end(); ++it){
		//ShowVectorContent("IndexServiceLoader::Load Base AdMember, ",*it,0);
		std::vector<std::string> v = it->fields;
		try{
			long member_id = boost::lexical_cast<long>(v[0]);
			int category = boost::lexical_cast<int>(v[1]);
			int industry = boost::lexical_cast<int>(v[2]);
			IceUtil::RWRecMutex::WLock wlock(lock_);
			AdMemberPtr member = new AdMember;
			member->member_id_ = member_id;
			member->category_ = category;
			member->industry_ = industry;
			member_pool_[member->member_id_] = member;
			ShowVectorContent("IndexServiceLoader::Load Base Valid Member, ",*it,0);
			++cnt;
		}catch(const std::exception &e){
			ShowVectorContent("IndexServiceLoader::Load Base Member exception, ",*it,1);
		}
	}
	MCE_INFO("IndexServiceLoader::Load Base Members End,Valid Size:" << cnt);
}


void IndexServiceLoader::LoadAdCampaigns(const std::vector<IndexItem> &items){
	int cnt = 0;
	std::vector<IndexItem>::const_iterator it = items.begin();
	for(; it != items.end(); ++it){
		std::vector<std::string> v = it->fields;
		try{
			long campaign_id = boost::lexical_cast<long>(v[0]);
			long member_id = boost::lexical_cast<long>(v[1]);
			AdCampaignPtr campaign = new AdCampaign;
			campaign->campaign_id_ = campaign_id;
			campaign->member_id_ = member_id;
			IceUtil::RWRecMutex::WLock wlock(lock_);
			campaign_pool_[campaign->campaign_id_] = campaign;	
			ShowVectorContent("IndexServiceLoader::Load Base Valid Campaigns, ",*it,0);
			++cnt;
		}catch(const std::exception &e){
			ShowVectorContent("IndexServiceLoader::Load Base Campaigns exception, ",*it,1);
		}
	}
	MCE_INFO("IndexServiceLoader::Load Base AdCampaigns End, Valid Size:" << cnt);
}

void IndexServiceLoader::LoadAdGroups(const std::vector<IndexItem> &items){
	int cnt = 0;
	std::vector<IndexItem>::const_iterator it = items.begin();
	for(; it != items.end(); ++it){
		std::vector<std::string> v = it->fields;
		try{
			long group_id = boost::lexical_cast<long>(v[0]);
			long campaign_id = boost::lexical_cast<long>(v[1]);
			long member_id = boost::lexical_cast<long>(v[2]);
			int trans_type = boost::lexical_cast<int>(v[4]);
			AdGroupPtr group = new AdGroup;
			group->group_id_ = group_id;
			group->campaign_id_ = campaign_id;
			group->member_id_ = member_id;
			group->trans_type_ = trans_type;
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdCampaignPtr>::iterator itcap = campaign_pool_.find(campaign_id);
			if(itcap == campaign_pool_.end()){
			//	MCE_WARN("IndexServiceLoader::Load Base AdGroup ,AdGroupID:" << group_id << " ,Campaign_Pool No CampaignID:" << campaign_id);
				continue;
			}
			__gnu_cxx::hash_map<long, AdMemberPtr>::iterator itmember = member_pool_.find(member_id);
			if(itmember != member_pool_.end()){
				group->member_category_ = itmember->second->category_;
				group->member_industry_ = itmember->second->industry_;
			}else{
			//	MCE_WARN("IndexServiceLoader::Load Base AdGroup ,AdGroupID:" << group_id << " ,Member_Pool No MemberID:" << member_id);
				continue;
			}
			group_pool_[group->group_id_] = group;
			ShowVectorContent("IndexServiceLoader::Load Base Valid AdGroups, ",*it,0);
			++cnt;
		}catch(const std::exception &e){
		//	MCE_WARN("IndexServiceLoader::LoadAdGroups  exception," << e.what() << ", adgroupId:" << group_id << ", item size:" << v.size());
			ShowVectorContent("IndexServiceLoader::Load Base AdGroups exception, ",*it,1);
		}
	}
	MCE_INFO("IndexServiceLoader::Load Base AdGroups End, Valid Size:"<< cnt);
}

void IndexServiceLoader::LoadAdCreatives(const std::vector<IndexItem> &items){
	int cnt = 0;
	std::vector<IndexItem>::const_iterator it = items.begin();
	for(; it != items.end(); ++it){
		std::vector<std::string> v = it->fields;
		try{
			long creative_id = boost::lexical_cast<long>(v[0]);
			long group_id = boost::lexical_cast<long>(v[1]);
			long widget_id = boost::lexical_cast<long>(v[2]);

			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdGroupPtr>::iterator iter = group_pool_.find(group_id);
			if(iter != group_pool_.end()){
				AdGroupPtr group = iter->second;
				group->creative_id_ = creative_id;
				group->widget_id_ = widget_id;
				++cnt;
		 		ShowVectorContent("IndexServiceLoader::Load Base Valid Creatives, ",*it,0);
			}else{
			//	MCE_WARN("IndexServiceLoader::Load Base Creatives,CreativeID:" << creative_id << " ,Group_Pool No GroupID:" << group_id);
			}
		}catch(const std::exception &e){
			ShowVectorContent("IndexServiceLoader::Load Base Creatives exception, ",*it,1);
		}
	}
	MCE_INFO("IndexServiceLoader::Load Base Creatives End, Valid Size:" << cnt);
}

void IndexServiceLoader::LoadAdZoneRef(const std::vector<IndexItem> &items){
	int cnt = 0;
	std::vector<IndexItem>::const_iterator it = items.begin();
	for(; it != items.end(); ++it){
		std::vector<std::string> v = it->fields;
		try{
			//该表主键是id，所以从1取
			long zone_id = boost::lexical_cast<long>(v[1]);
			long group_id = boost::lexical_cast<long>(v[2]);
			double max_price = boost::lexical_cast<double>(v[3]);

			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdGroupPtr>::const_iterator itzef = group_pool_.find(group_id);
			if(itzef != group_pool_.end()){
				AdGroupPtr group = itzef->second;
				group->zone_price_[zone_id] = max_price;
				++cnt;
				ShowVectorContent("IndexServiceLoader::Load Base Valid AdZoneRef, ",*it,0);
			}else{
			//	MCE_WARN("IndexServiceLoader::Load Base AdZoneRef,zone_id:" << zone_id << ",Group_Pool No GroupID:" << group_id);
			}
		}catch(const std::exception &e){
			ShowVectorContent("IndexServiceLoader::Load Base AdZoneRef exception, ",*it,1);
		}
	}
	MCE_INFO("IndexServiceLoader::Load Base AdZoneRef End, Valid Size:" << cnt);
}

void IndexServiceLoader::LoadBidUnits(const std::vector<IndexItem> &items){
	int cnt = 0;
	std::vector<IndexItem>::const_iterator it = items.begin();
	for(; it != items.end(); ++it){
		struct BidUnit bidunit; 
		std::vector<std::string> v = it->fields;
		try{
			bidunit.bid_id_ = boost::lexical_cast<long>(v[0]);
			bidunit.group_id_ = boost::lexical_cast<long>(v[1]);
			bidunit.gender_ = boost::lexical_cast<int>(v[2]);
			bidunit.stage_ = boost::lexical_cast<int>(v[3]);
			bidunit.grade_ = v[4];
			bidunit.age_ = v[5];
			bidunit.school_ = v[6];
			bidunit.area_ = v[7];
			bidunit.company_ = v[8];
			bidunit.interest_ = v[9];
			IceUtil::RWRecMutex::WLock wlock(lock_);
			bidunit_pool_[bidunit.bid_id_] = bidunit;
			++cnt;
			ShowVectorContent("IndexServiceLoader::Load Base Valid BidUnits, ",*it,0);
		}catch(const std::exception &e){
			ShowVectorContent("IndexServiceLoader::Load Base BidUnits exception, ",*it,1);
		}
	}
	if(cnt > 0){
		bidunit_changed_ = true;
	}
	MCE_INFO("IndexServiceLoader::Load Base BidUnits End, Valid Size:" << cnt);
}

void IndexServiceLoader::CreateIndex(){
	if(!bidunit_changed_){ //之前有变化的时候才重新创建索引进行替换
		MCE_INFO("IndexServiceLoader::CreateIndex ,No Inc Data,Do Nothing");
		return;
	}
	bidunit_changed_ = false;
	MCE_INFO("IndexServiceLoader::CreateIndex ,,BidUnit:size:" << bidunit_pool_.size() << " ,AdGroup_Pool Size:" << group_pool_.size());
	int cnt = 0;
	const int STAGE_COLLEGE = 2;
	const int STAGE_WORK = 4;
	const int STAGE_ALL = 6;
	std::map<long,TargetingIndex::FeatureIndexPtr> zone_index;

	IceUtil::RWRecMutex::RLock rlock(lock_);
	__gnu_cxx::hash_map<long, BidUnit>::iterator itbu =  bidunit_pool_.begin();
	for(; itbu != bidunit_pool_.end(); ++itbu){
		struct BidUnit bidunit = itbu->second;
		__gnu_cxx::hash_map<long, AdGroupPtr>::iterator itgrp = group_pool_.find(bidunit.group_id_);
		if(itgrp == group_pool_.end()){
			continue;
		}
		AdGroupPtr group = itgrp->second; 
		if(!group){
			continue;
		}
		++cnt;
		vector<short> short_keys;
		vector<uint64_t> uint_keys;
		group->bid_unit_id_ = bidunit.bid_id_;

		std::map<long, double>::iterator zit = group->zone_price_.begin();
		for(; zit != group->zone_price_.end(); ++zit){
			TargetingIndex::FeatureIndexPtr fidx = NULL;
			long zone_id = zit->first;
			if(zone_index.find(zone_id) == zone_index.end()){
				fidx = new TargetingIndex::FeatureIndex;
				zone_index.insert(make_pair(zone_id, fidx));
			}else{
				fidx = zone_index[zone_id];
			}
			if(Translator::processGender(bidunit.gender_, short_keys)) {
				fidx->gender_index_->add(short_keys, group);
				short_keys.clear();
			}
			if(Translator::processStage(bidunit.stage_, short_keys)){
				fidx->stage_index_->add(short_keys, group);
				short_keys.clear();
			}
			if(Translator::processAge(bidunit.age_, short_keys)){
				fidx->age_index_->add(short_keys, group);
				short_keys.clear();
			}
			vector<uint64_t>   uint64_keys;
			if (Translator::processCompany(bidunit.company_, uint64_keys)){
				fidx->company_index_->add(uint64_keys, group);
				uint64_keys.clear();
			}
			if (Translator::processInterest(bidunit.interest_, uint64_keys)){
				fidx->interest_index_->add(uint64_keys, group);
				uint64_keys.clear();
			}
			if(bidunit.stage_ == STAGE_COLLEGE) {
				vector<uint64_t> uint_keys2;
				if(Translator::processSchool(bidunit.school_, uint_keys, uint_keys2)){
					fidx->school_area_index_->add(uint_keys, group);
					fidx->school_index_->add(uint_keys2, group);
					uint_keys.clear();
					uint_keys2.clear();
				}
				if(Translator::processGrade(bidunit.grade_, short_keys)) {
					fidx->grade_index_->add(short_keys, group);
					short_keys.clear();
				}
			}
			else if((bidunit.stage_ == STAGE_WORK) && Translator::processArea(bidunit.area_, uint_keys)){
				fidx->area_index_->add(uint_keys, group);
				uint_keys.clear();
			}
			else if(bidunit.stage_ == STAGE_ALL && Translator::processArea(bidunit.area_, uint_keys)){
				fidx->area_index_->add(uint_keys, group);
				uint_keys.clear();

				short_keys.push_back(-1);
				fidx->grade_index_->add(short_keys, group);
				short_keys.clear();
			}
		}
	}
	TargetingIndex::instance().UpdateZoneIndex(zone_index);
}

void IndexServiceLoader::UpdateData(){
	//TODO 更新AdMember 
	__gnu_cxx::hash_map<long, AdGroupPtr> group_pool = group_pool_;
	__gnu_cxx::hash_map<long, AdCampaignPtr> campaign_pool = campaign_pool_;
	__gnu_cxx::hash_map<long, AdMemberPtr> member_pool = member_pool_;
	MCE_INFO("IndexServiceLoader::UpdateData, member_pool.size:" << member_pool.size() << ", campaign_pool.size:" << campaign_pool.size() << ", group_pool.size:" << group_pool.size() << " ,bidunit_pool.size:" << bidunit_pool_.size());
	AdPool::instance().UpdatePool(group_pool,campaign_pool,member_pool);
}

int IndexServiceLoader::LoadBase(const std::string &level, const std::vector<IndexItem> &items){
	if(items.empty()){
		return -1;
	}
	if(level == "jebe_ads.adgroup"){
		LoadAdGroups(items);
	}else if(level == "jebe_ads.campaign"){
		LoadAdCampaigns(items);
	}else if(level == "jebe_main.member"){
		LoadAdMembers(items);
	}else if(level == "jebe_ads.creative"){
		LoadAdCreatives(items);
	}else if(level == "jebe_ads.adgroup_adzone_ref"){
		LoadAdZoneRef(items);    
	}else if(level == "jebe_ads.bid_unit"){
		LoadBidUnits(items);
	}else{
		MCE_WARN("IndexServiceLoader::LoadBase, unknown level:" << level);
		return -1;
	}
  return 0;
}

int IndexServiceLoader::LoadInc(const std::vector<IndexItem> & items){
	if( 0 == items.size()){
		return -1;
	}
	std::vector<IndexItem>::const_iterator it = items.begin();
	for(; it != items.end(); ++it){
		std::string level = it->table;
		if(level == "jebe_ads.adgroup"){
			LoadIncItemToGroups(*it);
		}else if(level == "jebe_main.member"){
			LoadIncItemToMember(*it);
		}	else if(level == "jebe_ads.campaign"){
			LoadIncItemToCampain(*it);
		} else if(level == "jebe_ads.creative"){
			LoadIncItemToCreatives(*it);
		} else if(level == "jebe_ads.adgroup_adzone_ref"){
			LoadIncItemToZone(*it);
		} else if(level == "jebe_ads.bid_unit"){
			LoadIncItemToBidUnit(*it);
		} else {
			MCE_WARN("IndexServiceLoader::LoadInc , Unknown Level:"<< level);
			return -1;
		}
	}
	return 0;
}

void IndexServiceLoader::LoadIncItemToMember(const IndexItem & item){
	try{
		std::vector<std::string> v = item.fields;
		long member_id = boost::lexical_cast<long>(v[0]);
		int category = boost::lexical_cast<int>(v[1]);
		int industry = boost::lexical_cast<int>(v[2]);
		int is_formal = boost::lexical_cast<int>(v[3]);
		int no_exceed_max = boost::lexical_cast<int>(v[4]);
		int am_online_status = boost::lexical_cast<int>(v[5]);
		int have_money = boost::lexical_cast<int>(v[6]);
		int ma_online_status = boost::lexical_cast<int>(v[7]);
		int delete_flag = boost::lexical_cast<int>(v[8]);
		int audit_status = boost::lexical_cast<int>(v[9]);
		int validity = boost::lexical_cast<int>(v[10]);
		bool valid = is_formal && (validity == 1) && no_exceed_max
			&& am_online_status && ma_online_status &&
			(audit_status == 1) && delete_flag && have_money;

		if(valid){   //有效，替换或者添加
			AdMemberPtr member = new AdMember;
			member->member_id_ = member_id;
			member->category_ = category;
			member->industry_ = industry;
			member_pool_[member->member_id_] = member;
			ShowVectorContent("IndexServiceLoader::Load Inc Valid Member, ", item,0);
		}else{ //无效，删除
			__gnu_cxx::hash_map<long, AdMemberPtr>::iterator itmember = member_pool_.find(member_id);
			if(itmember != member_pool_.end()){
				member_pool_.erase(itmember);
				ShowVectorContent("IndexServiceLoader::Load Inc InValid Member, ", item,1);
			}else{
				ShowVectorContent("IndexServiceLoader::Load Inc InValid Member,No Source",item,1);		
			}
		}
	}	catch(const std::exception &e){
		ShowVectorContent("IndexServiceLoader::Load Inc Member exception, ",item,1);
	}
}

void IndexServiceLoader::LoadIncItemToGroups(const IndexItem & item){
	try{
		std::vector<std::string> v = item.fields;
		long group_id = boost::lexical_cast<long>(v[0]);
		long campaign_id = boost::lexical_cast<long>(v[1]);
		long member_id = boost::lexical_cast<long>(v[2]);
		int trans_type = boost::lexical_cast<int>(v[3]);
		//int max_price = boost::lexical_cast<int>(v[4]);
		std::string day_points = v[5];
		std::string hour_points = v[6];
		int member_status = boost::lexical_cast<int>(v[7]);
		int am_status = boost::lexical_cast<int>(v[8]);
		int audit_status = boost::lexical_cast<int>(v[9]);
		int delete_flag = boost::lexical_cast<int>(v[10]);
		int daily_sys_status = boost::lexical_cast<int>(v[11]);

		bool point_valid = PointsChecker::instance().check(day_points, hour_points);
		bool valid = (member_status == 1) && (am_status == 1) && (audit_status == 1) && (delete_flag == 1)
			&& (daily_sys_status == 1)  && point_valid;

		if(valid){  //增量数据有效，增加或者替换
			AdGroupPtr group = new AdGroup;
			group->group_id_ = group_id;
			group->campaign_id_ = campaign_id;
			group->member_id_ = member_id;
			group->trans_type_ = trans_type;
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdCampaignPtr>::iterator itcap = campaign_pool_.find(campaign_id);
			if(itcap == campaign_pool_.end()){
	//			MCE_WARN("IndexServiceLoader::Load Inc AdGroup ,AdGroupID:" << group_id << " ,Campaign_Pool No CampaignID:" << campaign_id);
				return;
			}
			__gnu_cxx::hash_map<long, AdMemberPtr>::iterator itmember = member_pool_.find(member_id);
			if(itmember != member_pool_.end()){
				group->member_category_ = itmember->second->category_;
				group->member_industry_ = itmember->second->industry_;
			}else{
	//			MCE_WARN("IndexServiceLoader::Load Inc AdGroup ,AdGroupID:" << group_id << " ,Member_Pool No MemberID:" << member_id);
				return;
			}
			group_pool_[group->group_id_] = group;
			ShowVectorContent("IndexServiceLoader::Load Inc Valid AdGroups, ",item,0);
		}else{  //增量数据无效，删除
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdGroupPtr>::iterator itgrp = group_pool_.find(group_id);
			if(itgrp != group_pool_.end()){
				group_pool_.erase(itgrp);
				MCE_INFO("IndexServiceLoader::Load Inc InValid AdGroups,Delete, member_id:"<< member_id << ", campaign_id:" << campaign_id << ", group_id:"<< group_id);
			}else{
				ShowVectorContent("IndexServiceLoader::Load Inc InValid AdGroups,No Source",item,1);		
			}
		}
	}catch(const std::exception &e){
		ShowVectorContent("IndexServiceLoader::Load Inc AdGroups exception, " ,item,1);
	}
}

void IndexServiceLoader::LoadIncItemToCampain(const IndexItem & item){
	try{
		std::vector<std::string> v = item.fields;
		long campaign_id = boost::lexical_cast<int>(v[0]);
		long member_id = boost::lexical_cast<int>(v[1]);
		int delete_flag = boost::lexical_cast<int>(v[2]);
		int audit_status = boost::lexical_cast<int>(v[3]);
		int am_status = boost::lexical_cast<int>(v[4]);
		int sys_status = boost::lexical_cast<int>(v[5]);
		int member_status =boost::lexical_cast<int>(v[6]);
		std::string s_begin_time = v[7];
		std::string s_end_time = v[8];

		Date date_begin_time = Date(s_begin_time);
		Date date_end_time = Date(s_end_time);
		time_t begin_time = date_begin_time.time();
		time_t end_time = date_end_time.time();

		Date date = Date(time(NULL));
		time_t timestamp = date.time();
		bool time_check = timestamp >= begin_time && (!end_time || timestamp <= end_time);
		bool valid = delete_flag && (audit_status == 1) && time_check
			&& (member_status == 1) && (am_status == 1) && (sys_status == 1);
		if(valid){ //增量数据有效，替换或者增加
			AdCampaignPtr campaign = new AdCampaign;
			campaign->campaign_id_ = campaign_id;
			campaign->member_id_ = member_id;
			IceUtil::RWRecMutex::WLock wlock(lock_);
			campaign_pool_[campaign->campaign_id_] = campaign;
			ShowVectorContent("IndexServiceLoader::Load Inc Campain, ",item,0);
		}else{  //增量数据无效，删除
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdCampaignPtr>::iterator itcap = campaign_pool_.find(campaign_id);
			if(itcap != campaign_pool_.end()){
				campaign_pool_.erase(itcap);
				MCE_INFO("IndexServiceLoader::Load Inc InValid Campain,Delete, member_id:"<< member_id << ", campaign_id:" << campaign_id);
			}else{
				ShowVectorContent("IndexServiceLoader::Load Inc InValid Campain,No Source",item,0);		
			}
		}
	}catch(const std::exception &e){
		ShowVectorContent("IndexServiceLoader::Load Inc Campain exception, ",item,1);
	}
}
void IndexServiceLoader::LoadIncItemToCreatives(const IndexItem & item){
	try{
		std::vector<std::string> v = item.fields;
		long creative_id = boost::lexical_cast<long>(v[0]);
		long group_id = boost::lexical_cast<long>(v[1]);
		long widget_id = boost::lexical_cast<long>(v[2]);
		int member_status = boost::lexical_cast<int>(v[3]);
		int am_status = boost::lexical_cast<int>(v[4]);
		int audit_status = boost::lexical_cast<int>(v[5]);
		int delete_flag = boost::lexical_cast<int>(v[6]);
		if(member_status != 1 || am_status != 1 || audit_status != 1 || delete_flag != 1){
			//增量数据无效，删除
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdGroupPtr>::iterator iter = group_pool_.find(group_id);
			if(iter != group_pool_.end()){
				group_pool_.erase(iter);
				MCE_INFO("IndexServiceLoader::Load Inc InValid Creatives,Delete, creative_id:"<< creative_id << ", group_id:" << group_id);
			}else{
				ShowVectorContent("IndexServiceLoader::Load Inc InValid Creatives,No Source",item,0);		
			}
		}else{  //增量数据有效，增加或者替换
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdGroupPtr>::iterator iter = group_pool_.find(group_id);
			if(iter != group_pool_.end()){
				AdGroupPtr group = iter->second;
				group->creative_id_ = creative_id;
				group->widget_id_ = widget_id;
				ShowVectorContent("IndexServiceLoader::Load Inc Valid Creatives, ",item,0);
			}
		}
	}catch(const std::exception &e){
		ShowVectorContent("IndexServiceLoader::Load Inc Creatives exception, ",item,1);
	}
}

void IndexServiceLoader::LoadIncItemToZone(const IndexItem & item){
	try{
		std::vector<std::string> v = item.fields;
		long zone_id = boost::lexical_cast<long>(v[1]);
		long group_id = boost::lexical_cast<long>(v[2]);
		double max_price = boost::lexical_cast<double>(v[3]);
		int member_status = boost::lexical_cast<int>(v[4]);
		int am_status = boost::lexical_cast<int>(v[5]);
		int delete_flag = boost::lexical_cast<int>(v[6]);

		bool valid = member_status != 1 || am_status != 1 || delete_flag != 1;
		if(valid){ //增量数据有效，修改或者增加
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdGroupPtr>::const_iterator it = group_pool_.find(group_id);
			if(it != group_pool_.end()){
				AdGroupPtr group = it->second;
				group->zone_price_[zone_id] = max_price;
				ShowVectorContent("IndexServiceLoader::Load Inc Valid AdZoneRef:",item,0);
			}else{
				//MCE_WARN("IndexServiceLoader::Load Inc AdZoneRef,zone_id:" << zone_id << ",Group_Pool No GroupID:" << group_id);
			}
		}else{  //增量数据无效，删除
			IceUtil::RWRecMutex::WLock wlock(lock_);
			__gnu_cxx::hash_map<long, AdGroupPtr>::const_iterator it = group_pool_.find(group_id);
			if(it != group_pool_.end()){
				AdGroupPtr group = it->second;
				std::map<long,double>::iterator itz2p = group->zone_price_.find(zone_id);
				if( itz2p != group->zone_price_.end()){
					group->zone_price_.erase(itz2p);
					ShowVectorContent("IndexServiceLoader::Load Inc InValid AdZoneRef:",item,0);
				}
			}else{
				ShowVectorContent("IndexServiceLoader::Load Inc InValid AdZoneRef,No Source,",item,0);		
			}
		}
	}catch(const std::exception &e){
		ShowVectorContent("IndexServiceLoader::LoadIncItemToZone exception, ",item,1);
	}
}
void IndexServiceLoader::LoadIncItemToBidUnit(const IndexItem & item){
	try{
		std::vector<std::string> v = item.fields;
		struct BidUnit bidunit;
		bidunit.bid_id_ = boost::lexical_cast<long>(v[0]);
		bidunit.group_id_ = boost::lexical_cast<long>(v[1]);
		bidunit.gender_ = boost::lexical_cast<int>(v[2]);
		bidunit.stage_ = boost::lexical_cast<int>(v[3]);
		bidunit.grade_ = boost::lexical_cast<std::string>(v[4]);
		bidunit.age_ = boost::lexical_cast<std::string>(v[5]);
		bidunit.school_ = boost::lexical_cast<std::string>(v[6]);
		bidunit.area_ = boost::lexical_cast<std::string>(v[7]);
		bidunit.company_ = boost::lexical_cast<std::string>(v[8]);
		bidunit.interest_ = boost::lexical_cast<std::string>(v[9]);
		int delete_flag = boost::lexical_cast<int>(v[10]);

	  IceUtil::RWRecMutex::WLock wlock(lock_);
		if(delete_flag != 1){  //增量数据无效 ,删除
			__gnu_cxx::hash_map<long, BidUnit>::iterator it = bidunit_pool_.find(bidunit.bid_id_);
			if(it != bidunit_pool_.end()){
				bidunit_pool_.erase(it);
				ShowVectorContent("IndexServiceLoader::Load Inc InValid BidUnit :",item,0);
			}else{
				ShowVectorContent("IndexServiceLoader::Load Inc InValid BidUnit,No Source:",item,0);
			}

		}else{ //增量数据有效，增加或者替换
			bidunit_pool_[bidunit.bid_id_] = bidunit;
			ShowVectorContent("IndexServiceLoader::Load Inc Valid BidUnit :",item,0);
			bidunit_changed_ = true;
		}
	}catch(const std::exception &e){
		ShowVectorContent("IndexServiceLoader::Load Inc BidUnit exception, ",item,1);
	}
}
