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

#include "db.h"
#include "Date.h"
#include "ad/indexService/TIndex/src/adTranslator.h"

using namespace xce::ad;
using namespace mysqlpp;
typedef mysqlpp::Null<string, mysqlpp::NullIsZero> MYSQL_ZERO;
typedef mysqlpp::Null<string, mysqlpp::NullIsBlank> MYSQL_BLANK;

void DBSelector::outputDBConfig()
{
	MCE_INFO("                                                                 ");
	MCE_INFO("###################### BEGIN DB CONFIG ##############################");
  MCE_INFO("AD SOURCE ADS: " << ad_db_source_ads_ );
  MCE_INFO("AD SOURCE MAIN: " << ad_db_source_main_ );
	MCE_INFO("CONFIG SOURCE: " << config_db_source_);
	MCE_INFO("###################### END DB CONFIG ##############################");
	MCE_INFO("                                                               ");
}


DBSelector::~DBSelector()
{
	//conn_.disconnect();
}

/*  
int DBSelector::init()
{
	return connectToConfigDB();
}
*/


int DBSelector::excuteQuery(const com::xiaonei::xce::Statement &sql, 
								const std::string &table,
								mysqlpp::StoreQueryResult & res,
                const std::string &db_source)
{
	try
	{
		res = com::xiaonei::xce::QueryRunner(db_source.c_str(), com::xiaonei::xce::CDbRServer, table.c_str()).store(sql);
	}
	catch(const std::exception &e)
	{
		MCE_WARN("excuteQuery catch std::exception: " << e.what());
		return -1;
	}
	catch(...)
	{
		MCE_WARN("excuteQuery catch unknown exception!");
		return -1;
	}

	return 0;

}

int DBSelector::loadAdMembers(std::map<long, AdMemberPtr> &member_pool)
{

	com::xiaonei::xce::Statement sql;
	sql << MEMBER_QUERY.c_str();
	mysqlpp::StoreQueryResult res;
  int ret = excuteQuery(sql, "admember", res, ad_db_source_main_);
	if(ret < 0)
	{
		MCE_WARN("DBSelector::loadMembers failed");
		return -1;
	}
        
  try{
    if (res.num_rows() > 0) 
    {
      for (size_t i = 0; i < res.num_rows(); ++i) 
      {
        AdMemberPtr member = new AdMember;
        mysqlpp::Row row = res.at(i);
                  try{
        member->member_id_ = ((long)row["member_id"]);
        member->category_ = ((int) row["category"]);
        member->industry_ = ((int) row["industry"]);
                   } catch(...){
                        continue;
                   }
        member_pool[member->member_id_] = member;
      }

      MCE_INFO("TRIGGER member_pool.size = " << member_pool.size());
    } 
    else 
    {
      MCE_INFO("AdMemberPool::Init --> AFTER loadAdMembers no data in ad_member");
    }
  }catch(const std::exception &e){
    MCE_WARN("DBSelector::loadAdMembers caught exception:" << e.what());
    return -1;
  }

	return 0;

}

int DBSelector::loadAdCampaigns(std::map<long, AdCampaignPtr> &campaign_pool)
{

  com::xiaonei::xce::Statement sql;
  sql << CAMPAIGN_QUERY.c_str();
  mysqlpp::StoreQueryResult res;

  int ret = excuteQuery(sql, "adcampaign", res, ad_db_source_ads_);
  if(ret < 0)
  {
	  MCE_WARN("DBSelector::loadAdCampaigns excuteQuery failed");
	  return -1;
  }

  try{
    if (res.num_rows() > 0) 
    {
      for (size_t i = 0; i < res.num_rows(); ++i) 
      {
        AdCampaignPtr campaign = new AdCampaign;
        mysqlpp::Row row = res.at(i);

        campaign->campaign_id_ = (long) row["campaign_id"];
        campaign->member_id_ = (long) row["member_id"];

        campaign_pool[campaign->campaign_id_] = campaign;
      }

      MCE_INFO("TRIGGER campaign_pool_size=" << campaign_pool.size());

    } 
    else 
    {
      MCE_INFO("AdCampaignPool::Init --> AFTER DBSelector::loadAdCampaigns no data in ad_campaign");
    }
  }catch(const std::exception &e){
    MCE_WARN("DBSelector::loadAdCampaigns caught exception:" << e.what());
    return -1;
  }

  return 0;

}

int DBSelector::loadAdGroups(__gnu_cxx::hash_map<long, AdGroupPtr> &group_pool)
{
	com::xiaonei::xce::Statement sql;
	mysqlpp::StoreQueryResult res;
	sql << GROUP_QUERY.c_str();

	int ret = excuteQuery(sql, "adgroup", res, ad_db_source_ads_);
	if(ret < 0)
	{
		MCE_WARN("DBSelector::loadAdGroups excuteQuery failed");
		return -1;
	}

  try{
    if (res.num_rows() > 0) 
    {
      for (size_t i = 0; i < res.num_rows(); ++i) 
      {
        AdGroupPtr group = new AdGroup;
        mysqlpp::Row row = res.at(i);

        group->group_id_ = (long) row["adgroup_id"];
        group->campaign_id_ = (long) row["campaign_id"];
        group->member_id_ = (long) row["member_id"];
        group->trans_type_ = (int) row["trans_type"];

        string day_points = (MYSQL_BLANK) row["day_points"];
        string hour_points = (MYSQL_BLANK) row["hour_points"];

        AdCampaignPtr campaign_ptr = AdPool::instance().getAdCampaign(group->campaign_id_);
        if (!campaign_ptr) 
        {
          continue;
        }

        AdMemberPtr member_ptr = AdPool::instance().getAdMember(group->member_id_);
        if (member_ptr) 
        {
          group->member_category_ = member_ptr->category_;
          group->member_industry_ = member_ptr->industry_;
        }
        else
        {
          continue;
        }

        bool point_valid = PointsChecker::instance().check(day_points, hour_points);
        if(point_valid)
        {
          group_pool[group->group_id_] = group;
        }

      }
                        


      MCE_INFO("TRIGGER group_pool_size=" << group_pool.size());
    } 
    else 
    {
      MCE_INFO("AdGroupPool::Init --> AFTER DBSelector::loadAdGroups no data in AdGroupPool");
    }
  }catch(const std::exception &e){
    MCE_WARN("DBSelector::loadAdGroups caught exception:" << e.what());
    return -1;
  }

	return 0;
}

int DBSelector::loadAdCreatives(__gnu_cxx::hash_map<long, AdGroupPtr> &group_pool)
{

  int cnt = 0;
	com::xiaonei::xce::Statement sql;
	sql << CREATIVE_QUERY.c_str();
	mysqlpp::StoreQueryResult res;

	int ret = excuteQuery(sql, "creative", res, ad_db_source_ads_);
	if(ret < 0)
	{
		MCE_WARN("DBSelector::loadCreatives failed");
		return -1;
	}

  try{
    if (res.num_rows() > 0) 
    {
      for (size_t i = 0; i < res.num_rows(); ++i) 
      {
        AdMemberPtr member = new AdMember;
        mysqlpp::Row row = res.at(i);

        long creative_id = (long) row["creative_id"];
        long group_id = (long) row["adgroup_id"];
        long widget_id = (long) row["widget_id"];

        __gnu_cxx::hash_map<long, AdGroupPtr>::iterator it = group_pool.find(group_id);	

        if (it != group_pool.end())
        {
          AdGroupPtr group = it->second;
          group->creative_id_ = creative_id;
          group->widget_id_ = widget_id;
          ++cnt;
        }

      }

      MCE_INFO("TRIGGER creative_pool_size=" << cnt);

    } 
    else 
    {
      MCE_INFO("AdMemberPool::Init --> AFTER DBSelector::loadAdCreatives no data in ad_member");
    }
  }catch(const std::exception &e){
    MCE_WARN("DBSelector::loadAdCreatives caught exception:" << e.what());
    return -1;
  }

	return 0;

}

int DBSelector::loadAdZoneRef(const __gnu_cxx::hash_map<long, AdGroupPtr> &group_pool)
{

  int cnt = 0;
	com::xiaonei::xce::Statement sql;
	sql << ADZONE_GROUP_REF_QUERY.c_str();
	mysqlpp::StoreQueryResult res;

	int ret = excuteQuery(sql, "adgroup_adzone_ref", res, ad_db_source_ads_);
	if(ret < 0)
	{
		MCE_WARN("DBSelector::LoadAdZone excuteQuery failed");
		return -1;
	}

  try{
    if (res.num_rows() > 0) 
    {
      size_t i = 0;
      for (; i < res.num_rows(); ++i) 
      {
        mysqlpp::Row row = res.at(i);
        long group_id = (long) row["adgroup_id"];
        long zone_id = (long) row["adzone_id"];
        double max_price = (double) row["max_price"];

        __gnu_cxx::hash_map<long, AdGroupPtr>::const_iterator it = group_pool.find(group_id);
        if(it != group_pool.end())
        {
          AdGroupPtr group = it->second;
          group->zone_price_[zone_id] = max_price;
          ++cnt;
        }
      }

      MCE_INFO("TRIGGER adzone_pool_size=" << cnt);
    }
    else
    {
      MCE_INFO("AFTER DBSelector::LoadAdZone AdZonePool has no data");
    }
  }catch(const std::exception &e){
    MCE_WARN("DBSelector::loadAdZoneRef caught exception:" << e.what());
    return -1;
  }

	return 0;
	
}

int DBSelector::loadBidUnits(std::map<long, TargetingIndex::FeatureIndexPtr> &zone_index)
{
  int cnt = 0;
	const int STAGE_COLLEGE = 2;
	const int STAGE_WORK = 4;
	const int STAGE_ALL = 6;
	com::xiaonei::xce::Statement sql;
	sql << BID_UNIT_QUERY.c_str();
	mysqlpp::StoreQueryResult res;
	int ret = excuteQuery(sql, "bid_unit", res, ad_db_source_ads_);
	if(ret < 0)
	{
		MCE_WARN("DBSelector::loadBidUnit excuteQuery failed");
		return -1;
	}
/*        stringstream sql;
	sql << BID_UNIT_QUERY.c_str();
        const char* DB_HOST = "10.3.18.145";
        const char* DB_NAME = "jebe_ads";
        const char* DB_USER = "ad";
        const char* DB_PASSWORD = "adjb###";

        Connection conn(DB_NAME, DB_HOST, DB_USER, DB_PASSWORD);
        Query query(&conn, true, sql.str().c_str());
        StoreQueryResult res = query.store(); 
*/
	if(res.num_rows() <= 0)
	{
		MCE_WARN("DBSelector::loadBidUnit return no data");
		return -1;
	}

  try{
    size_t i = 0;
    for (; i < res.num_rows(); ++i) 
    {
      mysqlpp::Row row = res.at(i);

      long group_id = boost::lexical_cast<long> ((MYSQL_ZERO) res.at(i)["adgroup_id"]);
      long bid_id = boost::lexical_cast<long> ((MYSQL_ZERO) res.at(i)["bid_unit_id"]);
      int gender = boost::lexical_cast<int> ((MYSQL_ZERO) res.at(i)["cast_gender"]);
      int stage = boost::lexical_cast<int> ((MYSQL_ZERO) res.at(i)["cast_stage"]);
      string grade = (MYSQL_BLANK) (res.at(i)["cast_grade"]);
      string age = (MYSQL_BLANK) (res.at(i)["cast_age"]);
      string school = (MYSQL_BLANK) (res.at(i)["cast_school"]);
      string area = (MYSQL_BLANK) (res.at(i)["cast_area"]);
                string company = (MYSQL_BLANK)(res.at(i)["cast_company"]);
                string interest = (MYSQL_BLANK)(res.at(i)["cast_interest"]);  //add 2012.07.05
      AdGroupPtr group = AdPool::instance().getAdGroup(group_id);
      if(!group)
        continue;

      //MCE_DEBUG("loadBidUnits load group: " << group->group_id_ << " bid_id: " << bid_id << " gender: " << gender
      //		 << " stage: " << stage << " grade: " << grade << " age: " << age << " school: " << school << " area: " << area);

      ++cnt;
      group->bid_unit_id_ = bid_id;

		vector<short> short_keys;
		vector<uint64_t> uint_keys;

      std::map<long, double>::iterator zit = group->zone_price_.begin();
      for(; zit != group->zone_price_.end(); ++zit)
      {
        TargetingIndex::FeatureIndexPtr fidx = NULL;

        long zone_id = zit->first;
        if(zone_index.find(zone_id) == zone_index.end())
        {
          fidx = new TargetingIndex::FeatureIndex;
          zone_index.insert(make_pair(zone_id, fidx));
        }
        else
        {
          fidx = zone_index[zone_id];
        }

        if(Translator::processGender(gender, short_keys)) 
        {
          fidx->gender_index_->add(short_keys, group);
          short_keys.clear();
        }

        if(Translator::processStage(stage, short_keys)) 
        {
          fidx->stage_index_->add(short_keys, group);
          short_keys.clear();
        }

        if(Translator::processAge(age, short_keys)) 
        {
          fidx->age_index_->add(short_keys, group);
          short_keys.clear();
        }

      vector<uint64_t>   uint64_keys;
      if (Translator::processCompany(company, uint64_keys))
      {
        fidx->company_index_->add(uint64_keys, group);
        uint64_keys.clear();
      }
      if (Translator::processInterest(interest, uint64_keys))       //add 2012.07.05
      {
        fidx->interest_index_->add(uint64_keys, group);
        uint64_keys.clear();
      }

      
			if(stage == STAGE_COLLEGE) 
			{
				vector<uint64_t> uint_keys2;
				if(Translator::processSchool(school, uint_keys, uint_keys2)) 
				{
					fidx->school_area_index_->add(uint_keys, group);
					fidx->school_index_->add(uint_keys2, group);
					uint_keys.clear();
					uint_keys2.clear();
				}
				if(Translator::processGrade(grade, short_keys)) {
					fidx->grade_index_->add(short_keys, group);
					short_keys.clear();
				}
			}
			else if((stage == STAGE_WORK) && Translator::processArea(area, uint_keys)) 
			{
				fidx->area_index_->add(uint_keys, group);
				uint_keys.clear();
			} 
			else if(stage == STAGE_ALL && Translator::processArea(area, uint_keys)) 
			{

          fidx->area_index_->add(uint_keys, group);
          uint_keys.clear();

          short_keys.push_back(-1);
          fidx->grade_index_->add(short_keys, group);
          short_keys.clear();
        }
      }
    }
  }catch(const std::exception &e){
    MCE_WARN("DBSelector::loadBidUnits caught exception:" << e.what());
    return -1;
  }


	return 0;

}

int DBSelector::loadCommonConfig(std::map<std::string, std::string> &para_map){

  com::xiaonei::xce::Statement sql;
  mysqlpp::StoreQueryResult res;
  sql << "select * from common_para;";

  int ret = excuteQuery(sql, "common_para", res, config_db_source_);
  if(ret < 0){
    MCE_WARN("DBSelector::loadCommonConfig excuteQuery FAILED");
    return -1;
  }

  if (res && res.num_rows() > 0){
    for (size_t i = 0; i < res.num_rows(); ++i){
      std::string key = (MYSQL_BLANK) (res.at(i)["key"]);
      std::string value = (MYSQL_BLANK) (res.at(i)["value"]);
      para_map[key] = value;
    }
  }

  return 0;
}


int DBSelector::LoadClosedAd(__gnu_cxx::hash_map<int, std::set<CloseItem> > &closed_ad_pool, int minutes, long &max_close_id){

  int ad_num = 0;
  max_close_id = 0;
  com::xiaonei::xce::Statement sql;
	mysqlpp::StoreQueryResult res;
  time_t now = time(NULL);
  time_t old_time = now - minutes * 60;  
  sql << "select * from ad_close where close_time > " << old_time;

	int ret = excuteQuery(sql, "ad_close", res, "db_adwr");
  if(ret < 0){
    MCE_WARN("DBSelector::LoadClosedAd excuteQuery FAILED");
    return -1;
  }

  if (res && res.num_rows() > 0){
   for (size_t i = 0; i < res.num_rows(); ++i){
     CloseItem item;
     mysqlpp::Row row = res.at(i);
     int close_id = (int) row["id"];
     if(close_id > max_close_id){
       max_close_id = close_id;
     }

     item.uid_ = (int) row["uid"]; 
     item.creative_id_ = (long) row["creativeid"]; 
     item.close_time_ = (long) row["close_time"]; 
     closed_ad_pool[item.uid_].insert(item);
     ++ad_num;
   } 
  }

  MCE_INFO("DBSelector::LoadClosedAd load " << ad_num << " ads, " << closed_ad_pool.size() << " users" 
              << ",max_close_id:" << max_close_id);
  
  return 0;
}

int DBSelector::ReloadClosedAd(std::map<int, std::list<CloseItem> > &closed_items, long &max_close_id){

  int num = 0;
  com::xiaonei::xce::Statement sql;
  mysqlpp::StoreQueryResult res;
  sql << "select * from ad_close where id > " << max_close_id;

  int ret = excuteQuery(sql, "ad_close", res, "db_adwr");
  if(ret < 0){
    MCE_WARN("AdCloseManager::ReloadClosedAd FAILED");
    return -1;
  }

  if (res.num_rows() > 0){
    for (size_t i = 0; i < res.num_rows(); ++i){
      CloseItem item;
      mysqlpp::Row row = res.at(i);
      int close_id = (int) row["id"];
      if(close_id > max_close_id){
        max_close_id = close_id;
      }

      item.uid_ = (int) row["uid"]; 
      item.creative_id_ = (long) row["creativeid"]; 
      item.close_time_ = (long) row["close_time"]; 
      closed_items[item.uid_].push_back(item);
      ++num;
    } 
  }

  MCE_INFO("DBSelector::ReloadClosedAd reload " << num << " ads," << closed_items.size() << " users" 
                << ",max_close_id:" << max_close_id);

  return 0;
}

int DBSelector::InsertClosedAd(const CloseItem &item){

  com::xiaonei::xce::Statement sql;
  sql << "INSERT INTO ad_close(uid, creativeid, close_time) VALUES (" << item.uid_ 
    << "," << item.creative_id_ << "," << item.close_time_  << ")";

  MCE_DEBUG("DBSelector::InsertClosedAd UID: " << item.uid_ << "creative_id: " << item.creative_id_);

  try {
    com::xiaonei::xce::QueryRunner("db_adwr", com::xiaonei::xce::CDbWServer,"ad_close").store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" AdCloseManager::InsertToDB--> error:" << e);
    return -1;
  } catch (std::exception& e) {
    MCE_WARN(" AdCloseManager::InsertToDB--> error:" << e.what());
    return -1;
  }

  return 0;
}
