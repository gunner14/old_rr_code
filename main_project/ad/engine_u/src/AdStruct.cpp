/*
 * AdStruct.cpp
 *
 *  Created on: Sep 3, 2010
 *      Author: antonio
 */

#include "AdStruct.h"
#include "Date.h"
#include <bitset>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "AdConfig.h"
#include "UserRankHandler.h"
namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;
using namespace boost;

DictNodePtr DictNode::insert(const char& c, bool wordend) {
  Char2NodeMap::iterator it = node_.find(c);
  if (it == node_.end()) {
    DictNodePtr new_node = new DictNode();
    new_node->wordend_ = wordend;
    node_.insert(make_pair<char, DictNodePtr> (c, new_node));
    return new_node;
  }
  return it->second;
}

void DictNode::GetNext2(const char& c, DictNodePtr& ptr, bool lastisinteger, bool search_end){
  bool match = true;
  if(match){
  //每次来的时候，应该按照字母优先级去匹配，优先级比×去匹配数字的优先级高，不然会出bug
  //正常的匹配逻辑，看是否有要匹配的字符串， 有就将Ptr指向其所对应的位置，没有就让ptr指向NULL
  ////////////////////////////////////////////////////////////////
    Char2NodeMap::iterator it;
    it = ptr->node_.find(c);
    if(it != ptr->node_.end()){
      ptr = it->second;
      if(search_end){
        if(!it->second->wordend_){
          ptr = NULL;
        }
      }
      //正常匹配，如果找到了，那么一定会return
    }
    else{
      //如果没有匹配上来的字母的话，那么要寻找×这个匹配逻辑
      Char2NodeMap::iterator it;
      it = node_.find('*');
      //首先要找×这个字符，看看是不是模糊匹配
      if(it != node_.end()){
        //如果找到了。
        if(c >= '0' && c <= '9'){
          //而且这次来的是数字，那么我们就会进行模糊匹配
          if(search_end){
            //如果这次请求的是结尾
            if(!it->second->wordend_){
              //但是urlmaping里面不是结尾，那么是错误的 ，不会匹配上的。
              ptr = NULL;
            }
            else{
              //如果是搜索到结尾，要让ptr移动到树里面的结尾， 用以能够最后取到referer_zones_
              ptr = it->second;
            }
          }
          return;
        }
        else{
          //如果不是数字的话
          if(lastisinteger){
            //如果上一次是模糊的匹配，也就是上一次匹配成功的是数字
            ptr = it->second;
            //那么就把指针向后移动一个
            it = ptr->node_.find(c);
            //去查有没有这个字符，进行二次匹配
            if(it != ptr->node_.end()){
              //
              ptr = it->second;
              if(search_end){
                if(!it->second->wordend_){
                  ptr = NULL;
                  return;
                }
              }
              return;
            }
          }
        }
      }
      ptr = NULL;
    }
  }
}

void DictNode::GetNext(const char& c, DictNodePtr& ptr, bool search_end){
  bool match = true;
  Char2NodeMap::iterator it;
  it = node_.find('*');
  if(it != node_.end()){
    if(c >= '0' && c <= '9'){   // url_adzone_maping中的*表示一个整数
      if(search_end){
        if(!it->second->wordend_){
          ptr = NULL;
        }
        else{
          //如果是搜索到结尾，要让ptr移动到树里面的结尾， 用以能够最后取到referer_zones_
          ptr = it->second;
        }
      }
      return;
    }
    ptr = it->second;
  }
  if(match){
    //正常的匹配逻辑，看是否有要匹配的字符串， 有就将Ptr指向其所对应的位置，没有就让ptr指向NULL
    it = ptr->node_.find(c);
    if(it != ptr->node_.end()){
      ptr = it->second;
      if(search_end){
        if(!it->second->wordend_){
          ptr = NULL;
        }
      }
    }
    else{
      ptr = NULL;
    }
  }
}

void DictTree::insert(const string& str, const AdZonePtr& zone_ptr) {
  int len = str.length();
  DictNodePtr index = root_;
  for (int i = 0; i < len; ++i) {
    index = index->insert(str.at(i), (i == len - 1));
  }
  index->referer_zones_.push_back(zone_ptr);
}

bool DictTree::search(const string& str, vector<AdZonePtr>& zones) {
  bool ans = true;
  int len = str.length();
  bool lastisinteger = false;
  DictNodePtr index = root_;
  for (int i = 0; i < len; ++i) {
    //index->GetNext(str.at(i), index, (i == len - 1));
    if(i){
      lastisinteger = (str.at(i - 1) >= '0' && str.at(i - 1) <= '9');
    }
    index->GetNext2(str.at(i), index, lastisinteger, (i == len - 1));
    if (!index) {
      ans = false;
      break;
    }
  }
  if (ans) {
    zones = index->referer_zones_;
  }
  return ans;
}

/***********************************************************************************************************************/
bool AdZone::Available() {
  bool result = false;
  result = (member_online_status_ == 1) && (am_online_status_ == 1)
      && (audit_status_ == 1) && (delete_flag_ == 1);
  return result;
}



Ice::Long AdZone::id() {
  return id_;
}


int AdZone::weight() {
  return weight_;
}

set<string> AdZone::referers() {
  return referer_;
}

void AdZone::set_id(Ice::Long id) {
  id_ = id;
}

//void AdZone::set_ad_count(int ad_count) {
//  ad_count_ = ad_count;
//}

void AdZone::add_stage(int stage) {
  stage_.insert(stage);
}

bool AdZone::FindStage(int stage) {
	return stage_.find(stage) != stage_.end();
}

void AdZone::set_weight(int weight) {
  weight_ = weight;
}

void AdZone::set_referer(const string& ref) {
  referer_.insert(ref);
}

void AdZone::set_member_online_status(int member_online_status) {
  member_online_status_ = member_online_status;
}
void AdZone::set_am_online_status(int am_online_status) {
  am_online_status_ = am_online_status;
}
void AdZone::set_audit_status(int audit_status) {
  audit_status_ = audit_status;
}
void AdZone::set_delete_flag(int delete_flag) {
  delete_flag_ = delete_flag;
}


void AdZonePool::LoadAdzone(map<Ice::Long, AdZonePtr> & zones) {
  Statement sql;
  sql << "select adzone_id, member_online_status, audit_status, delete_flag, "
      "am_online_status, count, bp_count, member_type, priority, rotation_number "
      "from adzone ";
  if (EngineConfig::instance().IsForBrand()) {
    sql << " where member_type = 0 or member_type = 1";
  } else {
    sql << " where member_type = 0 or member_type = 2";
  }
  const string DB_INSTANCE = EngineConfig::instance().DB_Main();
  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, "adzone").store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      AdZonePtr zone = new AdZone();
      zone->set_id((Ice::Long) row["adzone_id"]);
      zone->set_member_online_status((int) row["member_online_status"]);
      zone->set_am_online_status((int) row["am_online_status"]);
      zone->set_audit_status((int) row["audit_status"]);
      zone->set_delete_flag((int) row["delete_flag"]);
      zone->set_brand_count((int) res[i]["count"]);
      zone->set_bp_count((int) res[i]["bp_count"]);
      zone->set_weight((int) res[i]["priority"]);
      zone->set_member_type( (int) row["member_type"] );

      if (zone->Available()) {
        zones.insert(make_pair<Ice::Long, AdZonePtr> (zone->id(), zone));
      }
    }
  } else {
    MCE_INFO("AdZonePool::init --> no data in table adzone");
  }
}


void AdZonePool::LoadUrlAdzoneMaping(map<Ice::Long, AdZonePtr> & zones,
    DictTreePtr & trie, multimap<string, AdZonePtr> & ref_zone_map) {
  Statement sql;
  sql << "select url, stage, adzone_id from url_adzone_maping";
  const string DB_INSTANCE = EngineConfig::instance().DB_Main();
  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, "url_adzone_maping").store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      string referer = (mysqlpp::Null<string, mysqlpp::NullIsBlank>) res[i]["url"];
      Ice::Long zone_id = (Ice::Long) res[i]["adzone_id"];
      map<Ice::Long, AdZonePtr>::iterator it = zones.find(zone_id);
      if (it != zones.end()) {
        it->second->set_referer(referer);
        it->second->add_stage((int) res[i]["stage"]);
        ref_zone_map.insert(make_pair<string, AdZonePtr> (referer, it->second));
        trie->insert(referer, it->second);
      } else {
      }
    }
  } else {
    MCE_INFO("AdZonePool::init --> no data in table url_adzone_maping");
  }

  MCE_INFO("AdZonePool::Init --> AFTER INIT zones.size = " << zones.size()
      << " ref_zone_map.size = " << ref_zone_map.size());
}

void AdZonePool::Init() {
  MCE_INFO("AdZonePool::init");
  map<Ice::Long, AdZonePtr> zones;
  LoadAdzone(zones);

  DictTreePtr trie = new DictTree();
  multimap<string, AdZonePtr> ref_zone_map;
  LoadUrlAdzoneMaping(zones, trie, ref_zone_map);

  map<Ice::Long, vector<AdGroupPtr> > zone_groups;
  LoadDefaultAdGroup(zone_groups);

  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    zone_pool_.swap(zones);
    ref_zone_map_.swap(ref_zone_map);
    trie_ = trie;
	default_group_pool_.swap(zone_groups);
  }
}

vector<AdGroupPtr> AdZonePool::GetDefaultGroups(Ice::Long zone_id)
{
	return default_group_pool_[zone_id];
}

/*void AdZonePool::GetDefaultGroups(Ice::Long zone_id, int count, set<AdGroupPtr> &ans) {

	const vector<AdGroupPtr> &groups = default_group_pool_[zone_id];
	int default_size = groups.size();
	if(default_size <= count){
		ans.insert(groups.begin(), groups.end());
	}
	else{
		int now_index = rand() % default_size;
		for (int i = 0; i < count; i++){
			ans.insert(groups[now_index%default_size]);
			now_index++;
		}
	}

}
*/
/*void AdZonePool::GetDefaultGroupsForSA(Ice::Long zone_id, int count, set<AdGroupPtr> &ans) {

	vector<AdGroupPtr> local_default_list;
	const vector<AdGroupPtr> &groups = default_group_pool_[zone_id];
	for(vector<AdGroupPtr>::const_iterator it = groups.begin(); it != groups.end(); it++){
		Ice::Long widget_id = (*it)->widget_id();
		if(Widget2TypeCache::instance().GetWidgetType(widget_id) == 1){
			local_default_list.push_back(*it);
		}
	}
	//set<AdGroupPtr> ans;
	int default_size = local_default_list.size();
	if (default_size <= count){
		ans.insert(local_default_list.begin(), local_default_list.end());
	}
	else{
		int now_index = rand() % default_size;
		for (int i = 0; i < count; i++) {
			ans.insert(local_default_list[now_index%default_size]);
			now_index++;
		}
	}

}
*/
AdZoneDict AdZonePool::GetAdZoneOrdered(const string& referer, const UserProfile& userprofile, bool hide) {
  AdZoneDict ans;
  int user_stage = userprofile.stage();
  if(referer.empty()){
    return ans;
  }
  //MCE_DEBUG("AdZonePool::GetAdZoneOrdered -->  referer = " << referer);
  vector<string> maybe;
  vector<AdZonePtr> trie_zones;
  int len = referer.length();
  bool trie_ans = false;
  string ok_referer = "";
  maybe.push_back(referer);
  for (int i = len - 1; i; --i) {
    if (referer.at(i) == '?' || referer.at(i) == '/' || referer.at(i) == '#') {
      maybe.push_back(referer.substr(0, i));
    }
  }
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    for (vector<string>::iterator it = maybe.begin(); it != maybe.end(); ++it) {
      //MCE_DEBUG("AdZonePool::GetAdZoneOrdered --> trie.search url = " << (*it));
      try {
        trie_ans = trie_->search(*it, trie_zones);
      } catch (std::exception& e) {
        MCE_WARN("AdZonePool::GetAdZoneOrdered --> trie.search ERR  line:"
            << __LINE__ << " err:" << e.what());
      }
      if (trie_ans) {
        ok_referer = *it;
        //MCE_DEBUG("AdZonePool::GetAdZoneOrdered --> trie.search : " << *it
        //    << " OK trie_zones.size = " << trie_zones.size());
        bool is_match = false;
        for(vector<AdZonePtr>::iterator trie_zone_it = trie_zones.begin(); trie_zone_it != trie_zones.end(); ++trie_zone_it){
          //MCE_INFO("AdZonePool::GetAdZoneOrdered --> trie_zones.id = " << (*trie_zone_it)->id());
          if((*trie_zone_it)->Available() && (*trie_zone_it)->FindStage(user_stage)) {
            is_match = true;
          }
        }
        //break;
        if(is_match) {
          break; //匹配到maybe中某一个url，跳出循环
        } else {
          continue; //没有匹配到该url，继续循环
        }
      } else {
        //MCE_INFO("AdZonePool::GetAdZoneOrdered --> trie.search : " << *it << " FAILED");
      }
    }
  }

  for (vector<AdZonePtr>::iterator it = trie_zones.begin(); it != trie_zones.end(); ++it) {
    bool available = (*it)->Available();
    if (available && (*it)->FindStage(user_stage)) {
      //MCE_DEBUG("AdZonePool::GetAdZoneOrdered --> after equal_range referer = "
      //    << ok_referer << " find userstage = " << user_stage << " OK");
      if(!hide || (*it)->id() != 100000000003 && (*it)->id() != 100000000004) {
        ans.insert(make_pair<int, AdZonePtr> ((*it)->weight(), (*it)));
      }
    } else {
      //MCE_DEBUG("AdZonePool::GetAdZoneOrdered --> after equal_range referer = "
      //    << ok_referer << " find userstage = " << user_stage << " NOT MATCH");
    }
  }

  if (!ans.empty()) {
    /*MCE_DEBUG("AdZonePool::GetAdZoneOrdered --> ans.size = " << ans.size()
        << " referer = " << referer << " okreferer = " << ok_referer
        << " user.id = " << userprofile.id() << " user.stage = "
        << userprofile.stage() << " user.grade = " << userprofile.grade()
        << " user.gender = " << userprofile.gender() << " user.age = "
        << userprofile.age() << " user.school = " << userprofile.school()
        << " user.home_area = " << userprofile.home_area()
        << " user.current_area = " << userprofile.current_area());*/
    return ans;
  } else {
    MCE_INFO("AdZonePool::GetAdZoneOrdered --> myreferer = " << referer
        << " could not find any adzone");
  }

  return ans;
}

AdZoneDict AdZonePool::GetAdZoneOrdered(const ZoneInfoMap& zone_info_map, const UserProfile& userprofile) {
  AdZoneDict ans;
  //int user_stage = userprofile.stage();
  if(zone_info_map.empty()){
    return ans;
  }
  for(ZoneInfoMap::const_iterator itz = zone_info_map.begin(); itz != zone_info_map.end(); ++itz) {
    AdZonePtr z_ptr = AdZonePool::instance().GetAdZone(itz->first);
    if(z_ptr){
      ans.insert(make_pair<int, AdZonePtr> ((z_ptr)->weight(), z_ptr));
    }
  }
  if (!ans.empty()) {
    /*MCE_DEBUG("AdZonePool::GetAdZoneOrdered --> ans.size = " << ans.size()
        << " referer = " << referer << " okreferer = " << ok_referer
        << " user.id = " << userprofile.id() << " user.stage = "
        << userprofile.stage() << " user.grade = " << userprofile.grade()
        << " user.gender = " << userprofile.gender() << " user.age = "
        << userprofile.age() << " user.school = " << userprofile.school()
        << " user.home_area = " << userprofile.home_area()
        << " user.current_area = " << userprofile.current_area());*/
    return ans;
  } else {
    MCE_INFO("AdZonePool::GetAdZoneOrdered -->  = "  
        << " could not find any adzone");
  }

  return ans;
}

bool AdZonePool::IsCompetedDefaultGroup(AdGroupPtr group, set<AdGroupPtr> &selected_ids) {
  for(set<AdGroupPtr>::iterator it = selected_ids.begin(); it != selected_ids.end(); ++it) {
    if (group->group_id() == (*it)->group_id()) {
      return true;
    }
  }
  return false;
}

void AdZonePool::GetDefaultGroupsCompete(const Ice::Long adzone,const int count, set<AdGroupPtr> selected, const vector<AdGroupPtr> &groups,set<AdGroupPtr> &ans) {
	vector<AdGroupPtr> local_default_list;
	set<AdGroupPtr> selected_ads(selected.begin(),selected.end());
	for(vector<AdGroupPtr>::const_iterator it = groups.begin(); it != groups.end(); it++){
		if(!IsCompetedDefaultGroup((*it), selected_ads)){
			local_default_list.push_back(*it);
			selected_ads.insert(*it);
		}   
	}  
	int default_size = local_default_list.size();
	if (default_size <= count){
		ans.insert(local_default_list.begin(), local_default_list.end());
	}   
	else{
		int now_index = rand() % default_size;
		for (int i = 0; i < count; i++) {
			ans.insert(local_default_list[now_index%default_size]);
			now_index++;
		}   
	}
}

void AdZonePool::ReloadTimer::handle() {
  AdZonePool::instance().Init();
}



void AdZonePool::LoadDefaultAdGroup(map<Ice::Long, vector<AdGroupPtr> > &zone_groups) {

  Statement sql;
  sql << "select adgroup_adzone_ref.adzone_id, adgroup_adzone_ref.member_id, adgroup_adzone_ref.campaign_id, "
         "adgroup.adgroup_id, adgroup_adzone_ref.max_price, adgroup.trans_type, adgroup.display_type, creative.creative_id, "
         "creative.widget_id from adgroup_adzone_ref, adgroup, creative, campaign where adgroup_adzone_ref.member_id = 999 "
         "and adgroup_adzone_ref.member_status=1 AND adgroup_adzone_ref.am_status=1 AND adgroup_adzone_ref.delete_flag=1 and "
         "adgroup_adzone_ref.adgroup_id=adgroup.adgroup_id and adgroup.display_type = 4 and adgroup.adgroup_id=creative.adgroup_id "
         "and campaign.campaign_id = adgroup.campaign_id and campaign.begin_time < now() AND (campaign.end_time is NULL OR "
         "campaign.end_time>now()) AND campaign.sys_status = 1 AND campaign.member_status = 1 AND campaign.am_status = 1 AND "
         "campaign.audit_status = 1 AND campaign.delete_flag = 1 and adgroup.member_status = 1 AND adgroup.am_status = 1 AND "
         "adgroup.audit_status = 1 AND adgroup.delete_flag = 1 AND adgroup.daily_sys_status = 1 AND adgroup.total_sys_status "
         "= 1 and creative.member_status = 1 and creative.am_status = 1 and creative.audit_status = 1 and creative.delete_flag = 1";

  const string DB_INSTANCE = EngineConfig::instance().DB_Ads();
  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, "adgroup").store(
      sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      AdGroupPtr group = new AdGroup;
      mysqlpp::Row row = res.at(i);
      Ice::Long zone_id = (Ice::Long) row["adzone_id"];
      Ice::Long group_id = (Ice::Long) row["adgroup_id"];
      Ice::Long campaign_id = (Ice::Long) row["campaign_id"];
      Ice::Long member_id = (Ice::Long) row["member_id"];
      Ice::Long creative_id = (Ice::Long) row["creative_id"];
      Ice::Long widget_id = (Ice::Long) row["widget_id"];
      int max_price = (int) row["max_price"];
      int display_type = (int) row["display_type"];
      int trans_type = (int) row["trans_type"];

      group->set_campaign_id(campaign_id);
      group->set_group_id(group_id);
      group->set_member_id(member_id);
      group->set_max_price(max_price);
      group->set_display_type(display_type);
      group->set_trans_type(trans_type);
      group->creative_id_ = creative_id;
      group->widget_id_ = widget_id;

	  zone_groups[zone_id].push_back(group);

	  }

	for(map<Ice::Long, vector<AdGroupPtr> >::iterator it = zone_groups.begin(); it != zone_groups.end(); ++it)
	{
		long zone_id = it->first;
		int len = it->second.size();

		if(len <= 0)	{
			MCE_WARN("LoadDefaultAdGroup ZONE " << zone_id <<  " has no default ad");
		}
	}

  } else {
    MCE_WARN("AdGroupPool::LoadDefaultAdGroup --> no default groups");
  }
  	
}



vector<int> AdStyle::String2intvec(string str, string reg_sep){
  vector<string> tmps;  
  boost::algorithm::split_regex( tmps,str, regex(reg_sep )) ;
  vector<int> res ;
  for(vector<string>::iterator it = tmps.begin(); it < tmps.end() ; ++ it ) {
     int num = 0;
     string tmp = *it;
     for( size_t i=0; i < tmp.size(); ++i){
       num = num*10+(tmp[i]-'0'); 
     }
     res.push_back(num);
  }
  return res;
}

AdStylePtr AdStylePool::GetAdStyle(Ice::Long style_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdStylePtr ans = NULL;
  map<Ice::Long, AdStylePtr>::iterator it = style_pool_.find(style_id);
  if (it != style_pool_.end()) {
    ans = it->second;
  } else {
    // MCE_INFO("AdStylePool::GetAdStyle --> miss style_id = " << style_id);
  }
  return ans;
}

void AdStylePool::Init() {
	MCE_INFO("AdStylePool::Init Begin");
	map<Ice::Long, AdStylePtr> style_pool;
	try {
		mysqlpp::StoreQueryResult res;
		Statement sql;
		sql << "select * from style";
		res = QueryRunner(EngineConfig::instance().ConfigDB(), CDbRServer, "style").store(sql);
		if (res && res.num_rows() > 0) {
			for (size_t i = 0; i < res.num_rows(); ++i) {
				AdStylePtr style = new AdStyle;
				mysqlpp::Row row = res.at(i);
				style->set_style_id((Ice::Long) row["style_id"]);
				style->set_ad_cnt((int) row["ad_cnt"]);
				style->set_note((string) row["note"]);
				style->set_submit_time((string)row["submit_time"]);
				string bigs = (string) row["big_widgets"];
				string smalls = (string) row["small_widgets"];
				vector<int> bwidgets  = style->String2intvec(bigs,"\\_");
				style->set_big_widgets(bwidgets);
				vector<int> swidgets  = style->String2intvec(smalls,"\\_");
				style->set_small_widgets(swidgets);
				style_pool.insert(make_pair<Ice::Long, AdStylePtr> (style->style_id(), style));
			}
			MCE_INFO("AdStylePool::Init --> AFTER INIT style_pool.size = " << style_pool.size());
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				style_pool_.swap(style_pool);
			}
		} else {
			MCE_INFO("AdStylePool::Init --> no data in ad_style");
		}
	} catch (Ice::Exception& e){
		MCE_WARN("AdStylePool::Init ERROR:" << e);
	} catch (std::exception& e) {
		MCE_WARN("AdStylePool::Init ERROR:" << e.what());
	}
	MCE_INFO("AdStylePool::Init End");
}

void AdStylePool::UpdateCheckTimer::handle() {
  AdStylePool::instance().Init();
}

/***********************************************************************************************************************/
AdExperimentPtr AdExperimentPool::GetAdExperiment(Ice::Long experiment_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdExperimentPtr ans = NULL;
  map<Ice::Long, AdExperimentPtr>::iterator it = experiment_pool_.find(experiment_id);
  if (it != experiment_pool_.end()) {
    ans = it->second;
  } else {
    // MCE_INFO("AdExperimentPool::GetAdExperiment --> miss experiment_id = " << experiment_id);
  }
  return ans;
}

vector<AdExperimentPtr> AdExperimentPool::ListAdExperiment() {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  vector<AdExperimentPtr> ans ;
  map<Ice::Long, AdExperimentPtr>::iterator it;
  for( it = experiment_pool_.begin();it != experiment_pool_.end() ; it ++ ){
     ans.push_back(it->second);
  }
  return ans;
}

void AdExperimentPool::Init() {
  MCE_INFO("AdExperimentPool::Init Begin");
  map<Ice::Long, AdExperimentPtr> experiment_pool;
  try {
    Statement sql;
		sql << "select * from experiment where status=1 or status=0";
		mysqlpp::StoreQueryResult res;
		res = QueryRunner(EngineConfig::instance().ConfigDB(), CDbRServer, "experiment").store(sql);
		if (res && res.num_rows() > 0) {
      for (size_t i = 0; i < res.num_rows(); ++i) {
        //MCE_INFO("load a new experiment ...");
        AdExperimentPtr experiment = new AdExperiment;
        mysqlpp::Row row = res.at(i);
        experiment->set_experiment_id((Ice::Long) row["experiment_id"]);
        //MCE_INFO("id = " << experiment->experiment_id());
        experiment->set_zone_id((long) row["zone_id"]);
        //MCE_INFO("zone_id = " << experiment->zone_id());
        experiment->set_style_id((long) row["style_id"]);
        //MCE_INFO("style_id = " << experiment->style_id());
        experiment->set_traffic((int) row["traffic"]);
        //MCE_INFO("traffic = " << experiment->traffic());
        experiment->set_submit_time((string) row["submit_time"]);
        //MCE_INFO("submit_time = " << experiment->submit_time());
        experiment->set_modify_time((string) row["modify_time"]);
        //MCE_INFO("modify_time = " << experiment->modify_time());
        experiment_pool.insert(make_pair<Ice::Long, AdExperimentPtr> (experiment->experiment_id(), experiment));
        //MCE_INFO("load a new experiment !");
      }
      MCE_INFO("AdExperimentPool::Init --> AFTER INIT experiment_pool.size = " << experiment_pool.size());
      {
        IceUtil::RWRecMutex::WLock lock(mutex_);
        experiment_pool_.swap(experiment_pool);
      }
    } else {
      MCE_INFO("AdExperimentPool::Init --> no data in ad_experiment");
    }
  } catch (Ice::Exception& e){
    MCE_WARN("AdExperimentPool::Init ERROR:" << e);
  } catch (std::exception& e) {
    MCE_WARN("AdExperimentPool::Init ERROR:" << e.what());
  }
  MCE_INFO("AdExperimentPool::Init End");
}

void AdExperimentPool::UpdateCheckTimer::handle() {
  AdExperimentPool::instance().Init();
}


void CompeteRef::Init() {
  Load();
  is_ready_ = true;
}
bool CompeteRef::is_ready() {
  return is_ready_;
}

void CompeteRef::Load() {

  Statement sql;
  sql << "select * from ad_compete_ref ";
  mysqlpp::StoreQueryResult res;
  try {
    const string DB_INSTANCE = EngineConfig::instance().DB_Main();
    res = QueryRunner(DB_INSTANCE, CDbRServer, "ad_compete_ref").store(sql);//TODO
  } catch (Ice::Exception& e) {
    MCE_WARN("CompeteRef::Init :" << e);
  } catch (std::exception& e) {
    MCE_WARN("CompeteRef::Init :" << e.what());
  }

  if (!res) {
    MCE_INFO("CompeteRef::Init --> StoryQueryResult wrong");
    return;
  }
  if (res.empty()) {
    MCE_INFO("CompeteRef::Init --> nothing in db ");
  }
  map < Ice::Long, vector<Ice::Long> > tmpref;
  for (size_t i = 0; i < res.size(); ++i) {
    Ice::Long cp_a = (Ice::Long)(res.at(i)["first_campaign_id"]);
    Ice::Long cp_b = (Ice::Long)(res.at(i)["second_campaign_id"]);
    if (0 == cp_a && 0 == cp_b) { //按广告商竞品
      Ice::Long member_a = (Ice::Long)(res.at(i)["first_member_id"]);
      Ice::Long member_b = (Ice::Long)(res.at(i)["second_member_id"]);
      tmpref[member_a].push_back(member_b);
      tmpref[member_b].push_back(member_a);
    } else {
      tmpref[cp_a].push_back(cp_b);
      tmpref[cp_b].push_back(cp_a);
    }
  }
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    ref_.swap(tmpref);
    MCE_INFO("CompeteRef::Load --> load done,tmpref size = " << ref_.size());
  }

}

vector<Ice::Long> CompeteRef::Get(Ice::Long key_id) {
  if (is_ready_) {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    map<Ice::Long, vector<Ice::Long> >::const_iterator mit = ref_.find(key_id);
    if (mit != ref_.end()) {
      return mit->second;
    }
  }
  return vector<Ice::Long> ();
}

void CompeteRef::ReloadTimer::handle() {
  if (!CompeteRef::instance().is_ready()) {
    MCE_INFO("CompeteRef::Get --> is not ready");
    return;
  }
  CompeteRef::instance().Load();
}

/***********************************************************************************************************************/
void UserBindPool::Init() {
}

void UserBindPool::BindUser(int user_id, Ice::Long creative_id, Ice::Long zone_id) {
}

void UserBindPool::UnbindUser(int user_id, Ice::Long zone_id) {
}

void UserBindPool::NotifyBindUser(int user_id, Ice::Long creative_id, Ice::Long zone_id) {
}

void UserBindPool::NotifyUnbindUser(int user_id, Ice::Long zone_id) {
}

UserBindPool::iterator UserBindPool::find(UserBind user_bind) {
  return user_bind_cache_.end();
}

UserBindPool::iterator UserBindPool::end() {
  return user_bind_cache_.end();
}

} // end namespace ad
} // end namespace xce
