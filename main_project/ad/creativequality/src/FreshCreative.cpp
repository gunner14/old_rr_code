#include <connection.h>
#include <query.h>
#include <string>
#include <algorithm>
#include "AdConfig.h"
#include "QueryRunner.h"
#include "AdQualityUtil.h"
#include "FreshCreative.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/regex.hpp>
using namespace boost;
namespace xce{
namespace ad{
using namespace mysqlpp;


void FreshCreative::init() {
}

void FreshCreative::print_display_count() {
  ostringstream os;
  MCE_INFO("print_display_count creative_id:" <<creative_id_ <<",audit_time:" << create_time_<< ",sponsor_id:" << sponsor_id_);  
}

void FreshCreativeCache::LoadFreshCreativeMap(FreshAdQualityMap& fresh_creative_pool){
  IceUtil::RWRecMutex::RLock lock(mutex_);
  fresh_creative_pool = fresh_creative_pool_;
}

void FreshCreativeCache::LoadFreshCreativePool() {
  MCE_INFO("FreshCreativeCache::LoadFreshData begin");
  map<Ice::Long, FreshCreativePtr> creative_map;
	FreshAdQualityMap fresh_creative_pool;
  //加载审核过的在线新广告
  LoadAuditedOnlineFreshAd(creative_map);
  //加载新广告的每天的历史投放情况
  LoadFreshAdData(creative_map);
  //打印新广告的信息
  PrintDisplayCount(creative_map);
	AssignFreshCreative(fresh_creative_pool, creative_map);
	{
  	IceUtil::RWRecMutex::WLock lock(mutex_);
		fresh_creative_pool_.swap(fresh_creative_pool);
	}
	
	MCE_INFO("DemoLoaderCache::LoadFreshData stop");
}

void FreshCreativeCache::LoadAuditedOnlineFreshAd(map<Ice::Long, FreshCreativePtr>&  creative_map){ 
	MyUtil::Date begin_day = MyUtil::Date::seconds(time(NULL) - duration_day * 24 * 3600);  //10天的数据
  string begin_time = begin_day.str("%Y-%m-%d %H");
  {
    Statement sql;
    stringstream sql_string;//加载广告数据

		sql_string << "select creative_id, creative.adgroup_id AS group_id, creative.member_id, creative.create_time  from creative, adgroup"
							 << " where  adgroup.adgroup_id = creative.adgroup_id"
							 << " and creative.member_status = 1 and creative.am_status = 1 and creative.audit_status = 1"
							 << " and (adgroup.display_type = 1 or adgroup.display_type = 2 or adgroup.display_type = 6 or adgroup.display_type = 7)"
							 << " and creative.create_time >= '" << begin_time <<"'";

    /*    sql_string << "select  creative_id, adgroup_id as group_id, member_id, create_time from creative where"
				<<" member_status = 1 AND am_status = 1 AND audit_status = 1 AND delete_flag= 1 AND (display_type = 1 OR display_type = 2)"
				<< " AND create_time >= '" << begin_time <<"'"; */
    sql << sql_string.str();
    MCE_INFO("FreshCreativeCache::LoadAuditedOnlineFreshAd try to execute:" << sql_string.str());
    try {
      mysqlpp::StoreQueryResult res = QueryRunner(AdDemoConfig::instance().db_ad_name(), CDbRServer, "creative").store(sql);
      if (!res || res.num_rows() < 0){
        MCE_WARN("FreshCreativeCache::LoadAuditedOnlineFreshAd error");
        return;
      }
      else if (res.num_rows() == 0) {
        MCE_INFO("FreshCreativeCache::LoadAuditedOnlineFreshAd.NO create NEW ADS");
        return;
      }
      MCE_INFO("FreshCreativeCache::LoadAuditedOnlineFreshAd. create Fresh Size=: " << res.num_rows());
      //stringstream fresh_ad_list;
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long group_id = (Ice::Long) row["group_id"];
        Ice::Long creative_id = (Ice::Long) row["creative_id"];
        Ice::Long member_id = (Ice::Long) row["member_id"];
        FreshCreativePtr f_ptr = new FreshCreative(creative_id, group_id, member_id);
        string create_time = (string) row["create_time"];
        Ice::Long time = TimeForm2Long(create_time);
        f_ptr->set_create_time(time);
        creative_map[creative_id] = f_ptr;
      }
    }catch (std::exception& e) {
      MCE_WARN("FreshCreativeCache::LoadAuditedOnlineFreshAd error: " << e.what());
    } catch (...) {
      MCE_WARN("FreshCreativeCache::LoadAuditedOnlineFreshAd error!");
    }
  }
  MCE_INFO("FreshCreativeCache::LoadAuditedOnlineFreshAd stop , online new create group:" << creative_map.size());
}
void FreshCreativeCache::LoadFreshAdData(map<Ice::Long, FreshCreativePtr>& creative_map){
  MyUtil::Date begin_day = MyUtil::Date::seconds(time(NULL) - duration_day * 24 * 3600);  //10天的数据
  Ice::Long begin_time = GetDay(begin_day);
  mysqlpp::StoreQueryResult res;
  try {
    const char* db_host = AdDemoConfig::instance().db_host().c_str();
    const char* db_name = AdDemoConfig::instance().db_name().c_str();
    Connection conn(db_name,db_host,"ad","adjb###");
    stringstream sql;
    sql.str("");
    sql << "select adzone_id, creative_id, time ,sum(pv_count) as sum_impr , sum(click_count) as sum_click  from "
        " Log_DCAGZGM  where  time >= " << begin_time << " group by adzone_id, creative_id,  time";
    MCE_INFO("try to excute " << sql.str());
    string s= sql.str();
    Query q(&conn,true,s.c_str());
    //mysqlpp::StoreQueryResult res = q.store();
    res = q.store();
    if (!res || res.num_rows() <= 0) {
      MCE_INFO("FreshCreativeCache::LoadFreshAdData all gids is truely fresh ");
    }
  }catch (std::exception& e) {
    MCE_WARN("FreshCreativeCache::LoadFreshAdData error: " << e.what());
  } catch (...) {
    MCE_WARN("FreshCreativeCache::LoadFreshAdData error!");
  }

  MCE_INFO("FreshCreativeCache::LoadFreshAdData res Size=: " << res.num_rows());
  if (res && res.num_rows() > 0) {
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);
      Ice::Long creative_id = (Ice::Long) row["creative_id"];
      Ice::Long pv = (Ice::Long) row["sum_impr"];
			map<Ice::Long, FreshCreativePtr>::iterator it = creative_map.find(creative_id);
			if(it != creative_map.end() && pv > pv_limit_count){
            creative_map.erase(it);
			}
		} 
	}
}
 
void FreshCreativeCache::PrintDisplayCount(map<Ice::Long, FreshCreativePtr>& creative_map) {
  for(map<Ice::Long, FreshCreativePtr>::iterator it = creative_map.begin(); it != creative_map.end(); ++it) {
    it->second->print_display_count();
  }
}

void FreshCreativeCache::AssignFreshCreative(FreshAdQualityMap & fresh_pool, map<Ice::Long, FreshCreativePtr>& creative_map) {
  map<Ice::Long, double > cid_weight;
  for(map<Ice::Long, FreshCreativePtr>::iterator it = creative_map.begin(); it != creative_map.end(); ++it) {
      double weight =  0.01;
			cid_weight[it->first] = weight;
  }
  fresh_pool["reserved"] = cid_weight;
}
}
}


