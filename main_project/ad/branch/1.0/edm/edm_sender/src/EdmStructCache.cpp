/*
 * EdmStructCache.cpp
 *
 *  Created on: 2012-2-21
 *      Author: sunzz
 */
#include "EdmStructCache.h"
#include "EdmInvertedIndex.h"
#include "LogWrapper.h"
#include <bitset>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/tokenizer.hpp>
#include "Date.h"
#include "ParametersController.h"
using namespace std;
using namespace boost;
using namespace xce::ad;
/****************************************************************************************/
AdMemberPtr AdMemberPool::GetAdMember(Ice::Long member_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdMemberPtr ans = NULL;
  map<Ice::Long, AdMemberPtr>::iterator it = member_pool_.find(member_id);
  if (it != member_pool_.end()) {
    ans = it->second;
  } else {
    // MCE_INFO("AdMemberPool::GetAdMember --> miss member_id = " << member_id);
  }
  return ans;
}

void AdMemberPool::Init() {
  MCE_DEBUG("AdMemberPool::Init");
  Statement sql;
  map<Ice::Long, AdMemberPtr> member_pool;
  sql << "select * from member";
  const string DB_INSTANCE = EngineConfig::instance().db_source_main();

  mysqlpp::StoreQueryResult res =
      QueryRunner(DB_INSTANCE, CDbRServer,  "member").store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      AdMemberPtr member = new AdMember;
      mysqlpp::Row row = res.at(i);
      member->set_member_id((Ice::Long) row["member_id"]);
      member->set_is_formal((int) row["is_formal"]);
      member->set_validity((int) row["validity"]);
      member->set_have_money((int) row["have_money"]);
      member->set_no_exceed_max((int) row["no_exceed_max"]);
      member->set_am_online_status((int) row["am_online_status"]);
      member->set_ma_online_status((int) row["ma_online_status"]);
      member->set_delete_flag((int) row["delete_flag"]);
      member->set_category((int) row["category"]);
      member->set_industry((int) row["industry"]);
      if (member->Available()) {
        member_pool[member->member_id()] = member;
      }
    }
    MCE_INFO("AdMemberPool::Init --> AFTER INIT member_pool.size = " << member_pool.size());
    {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      member_pool_.swap(member_pool);
    }
  } else {
    MCE_INFO("AdMemberPool::Init --> no data in ad_member");
  }
}

void AdMemberPool::UpdateCheckTimer::handle() {
  AdMemberPool::instance().Init();
}

/*******************************************************************************************/
AdCampaignPtr AdCampaignPool::GetAdCampaign(Ice::Long campaign_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdCampaignPtr ans = NULL;
  map<Ice::Long, AdCampaignPtr>::const_iterator it = campaign_pool_.find(
      campaign_id);
  if (it != campaign_pool_.end()) {
    ans = it->second;
  } else {
    // MCE_INFO("AdCampaignPool::GetAdCampaign --> miss campaign_id = " << campaign_id);
  }
  return ans;
}

void AdCampaignPool::Init() {
  MCE_DEBUG("AdCampaignPool::Init");
  Statement sql;
  map<Ice::Long, AdCampaignPtr> campaign_pool;
  sql
      << "select sys_status, member_status, am_status, campaign_id, member_id, begin_time, end_time, cast_calender, cast_hour, delete_flag, audit_status from campaign";
  const string DB_INSTANCE = EngineConfig::instance().db_source_ads();
  mysqlpp::StoreQueryResult res;
  try{
  res = QueryRunner(DB_INSTANCE, CDbRServer, "campaign").store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      AdCampaignPtr obj = new AdCampaign;
      mysqlpp::Row row = res.at(i);
      Ice::Long campaign_id = (Ice::Long) row["campaign_id"];
      Ice::Long member_id = (Ice::Long) row["member_id"];
      string begin_time =
          (mysqlpp::Null<string, mysqlpp::NullIsBlank>) row["begin_time"];
      string end_time =
          (mysqlpp::Null<string, mysqlpp::NullIsBlank>) row["end_time"];
      bool delete_flag = (bool) row["delete_flag"];
      int audit_status = (int) row["audit_status"];
      int sys_status = (int) row["sys_status"];
      int member_status = (int) row["member_status"];
      int am_status = (int) row["am_status"];

      obj->set_campaign_id(campaign_id);
      obj->set_member_id(member_id);
      obj->set_delete_flag(delete_flag);
      obj->set_audit_status(audit_status);
      obj->set_sys_status(sys_status);
      obj->set_member_status(member_status);
      obj->set_am_status(am_status);

      if (begin_time.empty()) {
        obj->set_begin_time(0);
      } else {
        Date date_begin_time = Date(begin_time);
        obj->set_begin_time(date_begin_time.time());
      }
      if (end_time.empty()) {
        obj->set_end_time(0);
      } else {
        Date date_end_time = Date(end_time);
        obj->set_end_time(date_end_time.time());
      }

      //MCE_INFO("AdAnalysisManagerI::Campaign --> campaign_id_ = " << campaign_id << " member_id_ = " << member_id << " begin_time_ = " << begin_time
      //    << " end_time_ = " << end_time << " delete_flag_ = " << delete_flag << " audit_status_ = " << audit_status <<
      //    " cast_calender = " << cast_calender << " running_area_day_ = " << running_area_day << " cast_hour = " << cast_hour << " running_area_hour = " << running_area_hour);
      if (obj->Available()) {
        campaign_pool[campaign_id] = obj;
      }
    }

    MCE_INFO("AdCampaignPool::Init --> AFTER INIT campaign_pool.size = " << campaign_pool.size());

    {
      IceUtil::RWRecMutex::WLock lock(mutex_);
      campaign_pool_.swap(campaign_pool);
    }
  } else {
    MCE_INFO("AdCampaignPool::Init --> no data in ad_campaign");
  }
    } catch(std::exception& e) {
              MCE_WARN("CampainPool::Load std::exception error : " << e.what());
    }  catch(...) {
    MCE_WARN("LoadCampain--> err:" <<",line:" << __LINE__)
  }
}
void AdCampaignPool::UpdateCheckTimer::handle() {
  AdCampaignPool::instance().Init();
}

/**********************************************************************************************/
AdGroupPtr AdGroupPool::GetAdGroup(Ice::Long group_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  AdGroupPtr ans = NULL;
  hash_map<Ice::Long, AdGroupPtr>::iterator it = group_pool_.find(group_id);
  if (it != group_pool_.end()) {
    ans = it->second;
  }
  return ans;
}
set<AdGroupPtr> AdGroupPool::GetAdGroupsByIds(const vector<Ice::Long>& gids){
  IceUtil::RWRecMutex::RLock lock(mutex_);
  set<AdGroupPtr> ans;
  for(vector<Ice::Long>::const_iterator itg = gids.begin(); itg != gids.end(); ++itg) {
    hash_map<Ice::Long, AdGroupPtr>::iterator it = group_pool_.find(*itg);
    if (it != group_pool_.end()) {
      ans.insert(it->second);
    }
  }
  return ans;
}
void AdGroupPool::LoadAdGroup(hash_map<Ice::Long, AdGroupPtr> & group_pool) {
  Statement sql;
  sql
      << "select total_sys_status,daily_sys_status, display_type, trans_type, adgroup_id, campaign_id, member_id, max_price, member_status, am_status, audit_status, delete_flag,day_points,hour_points,  edm_send_level, edm_cast_type from adgroup where display_type = 8";
  const string DB_INSTANCE = EngineConfig::instance().db_source_ads();
  MCE_DEBUG("AdGroupPool::LoadAdGroup-->db:" << DB_INSTANCE);

  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, 
      "adgroup").store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      Ice::Long group_id = 0;
      try{
        AdGroupPtr group = new AdGroup;
        mysqlpp::Row row = res.at(i);
        //Ice::Long group_id = (Ice::Long) row["adgroup_id"];
        group_id = (Ice::Long) row["adgroup_id"];
        Ice::Long campaign_id = (Ice::Long) row["campaign_id"];
        Ice::Long member_id = (Ice::Long) row["member_id"];
        int member_status = (int) row["member_status"];
        int am_status = (int) row["am_status"];
        int audit_status = (int) row["audit_status"];
        int delete_flag = (int) row["delete_flag"];
        int max_price = (int) row["max_price"];
        int display_type = (int) row["display_type"];
        int trans_type = (int) row["trans_type"];
        int daily_sys_status = (int) row["daily_sys_status"];
        int total_sys_status = (int) row["total_sys_status"];


        int edm_send_level = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)row["edm_send_level"]);
        int edm_cast_type = lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>) row["edm_cast_type"]);
        string day_points =
          (mysqlpp::Null<string, mysqlpp::NullIsBlank>) row["day_points"];
        string hour_points =
          (mysqlpp::Null<string, mysqlpp::NullIsBlank>) row["hour_points"];
        bool campaign_status = false;
        AdCampaignPtr campaign_ptr = AdCampaignPool::instance().GetAdCampaign(
            campaign_id);
        if (campaign_ptr) {
          campaign_status = campaign_ptr->Available();
        }

        bool member_table_status = false;
        AdMemberPtr member_ptr = AdMemberPool::instance().GetAdMember(member_id);
        if (member_ptr) {
          member_table_status = member_ptr->Available(display_type);
          group->set_member_category(member_ptr->category());
          group->set_member_industry(member_ptr->industry());
        }

        bool point_valid = PointsChecker::instance().Check(day_points,
            hour_points);

        group->set_campaign_id(campaign_id);
        group->set_group_id(group_id);
        group->set_member_id(member_id);
        group->set_member_status(member_status);
        group->set_am_status(am_status);
        group->set_audit_status(audit_status);
        group->set_delete_flag(delete_flag);
        group->set_campaign_status(campaign_status);
        group->set_member_table_status(member_table_status);
        group->set_max_price(max_price);
        group->set_display_type(display_type);
        group->set_trans_type(trans_type);
        group->set_point_valid(point_valid);
        group->set_daily_sys_status(daily_sys_status);
        group->set_total_sys_status(total_sys_status);
        group->set_edm_send_level(edm_send_level);
        group->set_edm_cast_type(edm_cast_type);

        //group->set_rotation_number(rotation_number);
        if (group->Available()) {
          //if(group_id == 10000344890001) {
          //MCE_DEBUG("LoadAdgroup--> valid groupid:" << group_id<<",line:" << __LINE__);
          //}
          group_pool[group_id] = group;
        } else {
          MCE_DEBUG("LoadAdgroup--> invalid groupid:" << group_id<<",line:" << __LINE__);
        }
      } catch(std::exception& e) {
        MCE_WARN("AdGroupPool::LoadAdGroup std::exception error : " << e.what()<<",group_id:" <<group_id );
      } catch(...) {
        MCE_WARN("AdGroupPool::LoadAdGroup unknown error.");
      }
    }

  } else {
    MCE_INFO("AdGroupPool::Init --> no data in adgroup");
  }
}
void AdGroupPool::LoadCreative(hash_map<Ice::Long, AdGroupPtr> & group_pool) {
  Statement sql;
  sql
      << "select creative_id, adgroup_id, widget_id, widget,member_status, am_status, audit_status, delete_flag from creative "
          " where member_status = 1 and am_status = 1 and audit_status = 1 and delete_flag = 1";
  const string DB_INSTANCE = EngineConfig::instance().db_source_ads();

  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, 
      "creative").store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      Ice::Long creative_id = (Ice::Long) row["creative_id"];
      Ice::Long group_id = (Ice::Long) row["adgroup_id"];
      Ice::Long widget_id = (Ice::Long) row["widget_id"];
      string widget =
          (mysqlpp::Null<string, mysqlpp::NullIsBlank>) row["widget"];

      hash_map<Ice::Long, AdGroupPtr>::iterator it = group_pool.find(group_id);
      if (it != group_pool.end()) {
        MCE_DEBUG("LoadCreative--> valid groupid:" << group_id<<",line:" << __LINE__ <<",creative_id" <<creative_id);
        AdCreativePtr cpt = new AdCreative(creative_id, widget_id, widget);
        it->second->add_creative_seq(cpt);
        //MCE_DEBUG("[WIDGET_DEBUG] adgroup_id = " << it->second->group_id() << ", creative_id = " << creative_id << ", widget_id = " << widget_id);
        it->second->add_creative_id(creative_id);
        it->second->add_widget_id(creative_id, widget_id);
      } else {
      }

    }
  }
}
void AdGroupPool::CheckIfHaveCreative(
    hash_map<Ice::Long, AdGroupPtr> & group_pool) {
  for (hash_map<Ice::Long, AdGroupPtr>::iterator git = group_pool.begin();
      git != group_pool.end();) {
    if (!git->second->HasCreatives()) {
      MCE_DEBUG("AdGroupPool::Init --> because obj->HasCreatives is false so erase it  groupid = " << git->first);
      group_pool.erase(git++);
    } else {
      ++git;
    }
  }
}

void AdGroupPool::Init() {
  MCE_DEBUG("AdGroupPool::Init");
  hash_map<Ice::Long, AdGroupPtr> group_pool;

  LoadAdGroup(group_pool);
  MCE_INFO("AdGroupPool::Init --> LoadAdGroup Over!,valid group count:" << group_pool.size());
  LoadCreative(group_pool);
  MCE_INFO("AdGroupPool::Init --> LoadCreative Over!");
  CheckIfHaveCreative(group_pool);
  MCE_INFO("AdGroupPool::Init --> CheckIfHaveCreative Over!");

  EdmInvertedIndex::instance().Load(group_pool);
  MCE_INFO("AdGroupPool::Init --> Make EdmInvertedIndex Over!");

  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    group_pool_.swap(group_pool);
  }
}
/*void AdGroupPool::UpdateCampainStatus() {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  for (hash_map<Ice::Long, AdGroupPtr>::iterator it = group_pool_.begin();
      it != group_pool_.end(); ++it) {
    it->second->UpdateCampainStatus();
  }
}*/

void AdGroupPool::UpdateCheckTimer::handle() {
  AdGroupPool::instance().Init();
}
/*void AdGroupPool::UpdateCampaignStatusTimer::handle() {
  AdGroupPool::instance().UpdateCampainStatus();
}*/
/***********************************************************************************************************************/
void SchoolPool::Init() {
  map<Ice::Long, uint64_t> pool;
  Statement sql;
  sql << "select * from school";
  const string DB_INSTANCE = EngineConfig::instance().db_source_main();

  mysqlpp::StoreQueryResult res =
      QueryRunner(DB_INSTANCE, CDbRServer,  "school").store(sql);
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      Ice::Long school = (Ice::Long) row["id"];
      uint64_t area = (uint64_t) row["area_code"];
      pool[school] = area;
    }
  }
  MCE_DEBUG("SchoolPool::Init --> load done , pool size = " << pool.size());
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    pool_.swap(pool);
  }
}

uint64_t SchoolPool::GetSchoolAreaCode(Ice::Long school) {
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    map<Ice::Long, uint64_t>::const_iterator mit = pool_.find(school);
    if (mit != pool_.end()) {
      return mit->second;
    }
  }
  return 0;
}

/***********************************************************************************************************************/
bool PointsChecker::Check(const string & day_points,
    const string & hour_points) {
  bool valid = CheckDay(day_points) && CheckHour(hour_points);
  return valid;
}

bool PointsChecker::CheckHour(const string & hour_point_str) {
  if (hour_point_str.empty()) {
    return true;
  }
  bitset<24> hour_points;
  try {
    hour_points = lexical_cast<int>(hour_point_str);
  } catch (...) {
    MCE_WARN("PointsChecker::CheckHour --> boost cast hour_point_str = " << hour_point_str << " error");
  }
  Date date = Date(time(NULL));
  int this_hour = date.hour();
  bool result = hour_points.test(this_hour);
  //MCE_DEBUG("PointsChecker::CheckHour --> this_hour = " << this_hour
  //    << ", hours_points = " << hour_points << ",result = " << result);
  return result;
}

bool PointsChecker::CheckDay(const string & day_point_str) {
  if (day_point_str.empty()) {
    return true;
  }

  tokenizer<> tok(day_point_str);
  tokenizer<>::iterator tit = tok.begin();
  int year = lexical_cast<int>((*(tit)).substr(0, 4));
  int month = lexical_cast<int>((*(tit++)).substr(4, 2));
  map<int, bitset<32> > yearmonth_day;
  for (; tit != tok.end(); ++tit) {
    if ((*tit) != "0") {
      yearmonth_day[year * 100 + month] = lexical_cast<int>(*tit);
    }
    if (month == 12) {
      ++year;
      month = 1;
    } else {
      ++month;
    }
  }

  Date date = Date(time(NULL));
  int this_day = date.day();
  int this_month = date.month();
  int this_year = date.year();
  int yearmonth = this_year * 100 + this_month;
  map<int, bitset<32> >::iterator mit = yearmonth_day.find(yearmonth);
  if (mit != yearmonth_day.end()) {
    bool result = mit->second.test(this_day - 1); //0代表1号，以此类推
    return result;
  } else {
    return false;
  }
}

/***********************************************************************************************************************/
