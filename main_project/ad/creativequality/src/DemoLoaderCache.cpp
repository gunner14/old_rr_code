/*
 * DemoLoaderCache.cpp
 *
 *  Created on: May 10, 2011
 *      Author: kingsfield
 */

#include "DemoLoaderCache.h"
#include <connection.h>
#include <query.h>
#include <string>
#include <algorithm>
#include "AdConfig.h"
#include "QueryRunner.h"
#include "FreshCreative.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
using namespace boost;
using namespace xce::ad;
using namespace mysqlpp;


bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void ::MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&xce::ad::DemoLoaderCache::instance(), service.createIdentity("M",""));

  xce::ad::AdDemoConfig::instance().LoadConfig();
  MyUtil::TaskManager::instance().schedule(new xce::ad::AdDemoConfig::ConfigLoadTimer(0));
  MyUtil::TaskManager::instance().schedule(new xce::ad::FreshCreativeCacheTimer(0));    
  xce::ad::DemoLoaderCache::instance().LoadPool();
  TaskManager::instance().schedule(new xce::ad::DemoLoaderCacheTimer());

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdCreativeQuality.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdCreativeQuality.Interval",5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdCreativeQuality",
				      &DemoLoaderCache::instance(),
				      mod,
				      interval,
				      new EdmChannel());
}

AdQualityMap DemoLoaderCache::GetDemoPool(const ::Ice::Current& current){
  MCE_INFO("DemoLoaderCache::GetDemoPool--> call from " << current.con->toString());
  AdQualityMap result;
  IceUtil::RWRecMutex::RLock lock(mutex_);  
  result = demo_pool;
  MCE_INFO("DemoLoaderCache::GetDemoPool,pool size:"<<result.size());
  return result;
}

FreshAdQualityMap DemoLoaderCache::GetFreshAdMap(const ::Ice::Current& current){
  MCE_INFO("DemoLoaderCache::GetFreshAdMap--> call from " << current.con->toString());
	FreshAdQualityMap result;
  FreshCreativeCache::instance().LoadFreshCreativeMap(result);
  MCE_INFO("DemoLoaderCache::GetFreshAd,pool size:"<<result.size()); 
  return result;
}

void DemoLoaderCache::LoadPool() {
  AdQualityMap res_pool;

  MCE_INFO("[DemoLoaderCache]::try to load.LoadPool begin");
  MyUtil::Date begin = MyUtil::Date::seconds(time(NULL));
  string begin_time = begin.str("%Y-%m-%d %H:%M:%S");
  Ice::Long begin_time_pos = GetDay(begin.year(),begin.month(),begin.day());
  MyUtil::Date end = MyUtil::Date::seconds(time(NULL) - DAYCOUNT * HOURCOUNT * 3600);
  MyUtil::Date state_2_end = MyUtil::Date::seconds(time(NULL) - 2 * HOURCOUNT * 3600);
  string end_time = end.str("%Y-%m-%d %H:%M:%S");
  string state_2_end_time = state_2_end.str("%Y-%m-%d %H:%M:%S");
  Ice::Long state_2_end_time_pos = GetDay(state_2_end.year(),state_2_end.month(),state_2_end.day());

  try {
    const char* db_host = AdDemoConfig::instance().db_host().c_str();
    const char* db_name = AdDemoConfig::instance().db_name().c_str();
    MCE_INFO("DemoLoaderCache::db_host config: " << db_host << ", again : " << AdDemoConfig::instance().db_host());
    Connection conn(db_name,db_host,"ad","adjb###");
    MCE_INFO("DemoLoaderCache::LoadPool,db host:" << AdDemoConfig::instance().db_host()<< ",db_name:" << db_name);
    stringstream sql;
    {
        Ice::Long max_time = begin_time_pos;
        max_time =(((max_time * 100 + begin.hour()) * 100 + begin.minute())* 100 + begin.second());
        MCE_INFO("[DemoLoaderCache]::LoadPool begin,start time:"<< max_time <<",begin time:" << begin_time);
        res_pool["max_time"] = (double) max_time;
    }
    
    /****************no_gender*****************************/
    {
      sql.str("");
      sql
        << "select adzone_id, member_id, adgroup_id as group_id, ifnull(abs_pos,1) as abs_pos_val, creative_id, stage, "
				<< " sum(pv_count) as sum_impr, sum(click_count) as sum_click from "
        << " Log_DCAGZGM where time <= " << begin_time_pos << " and time >= "
        << state_2_end_time_pos << " group by adzone_id,adgroup_id,abs_pos_val, creative_id, stage ";
 
      MCE_INFO("try to excute " << sql.str());
      string s= sql.str();
      Query q(&conn,true,s.c_str());
      mysqlpp::StoreQueryResult res = q.store();
      if (!res || res.num_rows() <= 0) {
        MCE_WARN("DemoLoaderCache Load Error " << sql.str() << ".  select  fail!");
      }
      MCE_INFO("[DemoLoaderCache]::Load   res size : " << res.num_rows());
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long zone_id = (Ice::Long) row["adzone_id"];
        Ice::Long group_id = (Ice::Long) row["group_id"];
        int pos_id = (int) row["abs_pos_val"];
        Ice::Long sponsor_id = (Ice::Long) row["member_id"];
        Ice::Long creative_id = (Ice::Long)row["creative_id"];
        Ice::Int gender = NO_USE;
        Ice::Int stage = (Ice::Int) row["stage"];
        Ice::Long impr = (Ice::Long) row["sum_impr"];
        Ice::Long click = (Ice::Long) row["sum_click"];
        string key = GetKey(zone_id,sponsor_id, group_id,pos_id, creative_id, gender,stage);
        if(IsCredible(impr, click, no_gender)) {
          res_pool[key] = (double) click / (double) impr;
        }
      }
    }

    /*******************no_stage*************************************/  
    {
      sql.str("");
      sql << "select adzone_id,member_id, adgroup_id as group_id, ifnull(abs_pos,1) as abs_pos_val, creative_id, gender,  "
				  << " sum(pv_count) as sum_impr, sum(click_count) as sum_click from "
   			  <<" Log_DCAGZGM  where time <= " << begin_time_pos  << " and time >=" << state_2_end_time_pos << " " <<" group by adzone_id, adgroup_id, abs_pos_val, creative_id,gender ";
      MCE_INFO("try to excute " << sql.str());
      string s= sql.str();
      Query q(&conn,true,s.c_str());
      mysqlpp::StoreQueryResult res = q.store();
      if (!res || res.num_rows() <= 0) {
        MCE_WARN("DemoLoaderCache Load Error " << sql.str() << ".  select  fail!");
      }
      MCE_INFO("[DemoLoaderCache]::Load   res size : " << res.num_rows());
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long zone_id = (Ice::Long) row["adzone_id"];
        Ice::Long group_id = (Ice::Long) row["group_id"];
        int pos_id = (int) row["abs_pos_val"];
        Ice::Long sponsor_id = (Ice::Long) row["member_id"];
				Ice::Long creative_id = (Ice::Long)row["creative_id"];
        Ice::Int gender = (int) row["gender"];
        Ice::Int stage = NO_USE;
        Ice::Long impr = (Ice::Long) row["sum_impr"];
        Ice::Long click = (Ice::Long) row["sum_click"];
        string key = GetKey(zone_id, sponsor_id, group_id, pos_id, creative_id,  gender, stage);
        if(IsCredible(impr, click, no_stage)) {
          res_pool[key] = (double) click / (double) impr;
        }
      }
    }
    /*****************************spid_type***********************************************/
    {
      sql.str("");
      sql << "select adzone_id,member_id, ifnull(abs_pos,1) as abs_pos_val ,gender,stage, sum(pv_count) as sum_impr, sum(click_count) as sum_click from "
				  << " Log_DCAGZGM  where time <= " << begin_time_pos  << " and time >=" << state_2_end_time_pos  <<" group by adzone_id, member_id, abs_pos_val, stage,gender ";
      MCE_INFO("try to excute " << sql.str());
      string s= sql.str();
      Query q(&conn,true,s.c_str());
      mysqlpp::StoreQueryResult res = q.store();
      if (!res || res.num_rows() <= 0) {
        MCE_WARN("DemoLoaderCache Load Error " << sql.str() << ".  select  fail!");
      }
      MCE_INFO("[DemoLoaderCache]::Load   res size : " << res.num_rows());
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long zone_id = (Ice::Long) row["adzone_id"];
        Ice::Long group_id = NO_USE;
        int pos_id = (int) row["abs_pos_val"];
        Ice::Long sponsor_id = (Ice::Long) row["member_id"];
				Ice::Long creative_id = NO_USE;
        Ice::Int gender = (int) row["gender"];
        Ice::Int stage = (Ice::Int) row["stage"];
        Ice::Long impr = (Ice::Long) row["sum_impr"];
        Ice::Long click = (Ice::Long) row["sum_click"];
        string key = GetKey(zone_id,sponsor_id, group_id,pos_id, creative_id, gender,stage);
        if(IsCredible(impr, click, spid_type)) {
          res_pool[key] = (double) click / (double) impr;
        }
      }
    }

    /******************************all**********************************/
    {
      sql.str("");
      sql << "select adzone_id,member_id, adgroup_id as group_id, ifnull(abs_pos,1) as abs_pos_val, creative_id, sum(pv_count) as sum_impr, sum(click_count) as sum_click "
					<< " from Log_DCAGZGM  where time <= " << begin_time_pos  << " and time >=" << state_2_end_time_pos << " group by adzone_id, adgroup_id ,abs_pos_val, creative_id";
      MCE_INFO("try to excute " << sql.str());
      string s= sql.str();
      Query q(&conn,true,s.c_str());
      mysqlpp::StoreQueryResult res = q.store();
      if (!res || res.num_rows() <= 0) {
        MCE_WARN("DemoLoaderCache Load Error " << sql.str() << ".  select  fail!");
      }
      MCE_INFO("[DemoLoaderCache]::Load   res size : " << res.num_rows());
      for (size_t i = 0; i < res.num_rows(); ++i) {
        mysqlpp::Row row = res.at(i);
        Ice::Long zone_id = (Ice::Long) row["adzone_id"];
        Ice::Long group_id = (Ice::Long) row["group_id"];
        int pos_id = (int) row["abs_pos_val"];
        Ice::Long sponsor_id = (Ice::Long) row["member_id"];
				Ice::Long creative_id = (Ice::Long)row["creative_id"];

        Ice::Int gender = NO_USE;
        Ice::Int stage = NO_USE;
        Ice::Long impr = (Ice::Long) row["sum_impr"];
        Ice::Long click = (Ice::Long) row["sum_click"];
				MCE_DEBUG("creative_id = " << creative_id);
        string key = GetKey(zone_id,sponsor_id, group_id, pos_id, creative_id, gender,stage);
        if(IsCredible(impr, click, all_type)) {
          res_pool[key] = (double) click / (double) impr;
        }
      }
    }

		/******************************two_feature**********************************/
		{
			sql.str("");
			sql << "select adzone_id,member_id, adgroup_id as group_id, ifnull(abs_pos,1) as abs_pos_val, creative_id, sum(pv_count) as sum_impr, sum(click_count) as sum_click ,gender ,stage "
				<< " from Log_DCAGZGM  where time <= " << begin_time_pos  << " and time >=" << state_2_end_time_pos << " group by adzone_id, adgroup_id ,abs_pos_val, creative_id, gender, stage ";
			MCE_INFO("try to excute " << sql.str());
			string s= sql.str();
			Query q(&conn,true,s.c_str());
			mysqlpp::StoreQueryResult res = q.store();
			if (!res || res.num_rows() <= 0) {
				MCE_WARN("DemoLoaderCache Load Error " << sql.str() << ".  select  fail!");
			}
			MCE_INFO("[DemoLoaderCache]::Load   res size : " << res.num_rows());
			for (size_t i = 0; i < res.num_rows(); ++i) {
				mysqlpp::Row row = res.at(i);
				Ice::Long zone_id = (Ice::Long) row["adzone_id"];
				Ice::Long group_id = (Ice::Long) row["group_id"];
				int pos_id = (int) row["abs_pos_val"];
				Ice::Long sponsor_id = (Ice::Long) row["member_id"];
				Ice::Long creative_id = (Ice::Long)row["creative_id"];

				Ice::Int gender = (int) row["gender"];
				Ice::Int stage = (Ice::Int) row["stage"];
				Ice::Long impr = (Ice::Long) row["sum_impr"];
				Ice::Long click = (Ice::Long) row["sum_click"];
				MCE_DEBUG("creative_id = " << creative_id);
				string key = GetKey(zone_id,sponsor_id, group_id, pos_id, creative_id, gender,stage);
				if(IsCredible(impr, click, two_feature)) {
					res_pool[key] = (double) click / (double) impr;
				}
			}
		}


		MCE_INFO("[DemoLoaderCache]:: stop demo_pool size is " << res_pool.size());

	} catch (std::exception& e) {
		MCE_WARN("LoadByTime --> load demo pool error: " << e.what());
	} catch (...) {
		MCE_WARN("LoadByTime --> load demo pool error!");
	}

  DataValidityCheck(res_pool);

  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    demo_pool.swap(res_pool);
    MCE_INFO("[DemoLoaderCache]::LoadPool stop  size is " << demo_pool.size());
  }
	ShowAllCtr();
}

void DemoLoaderCache::DataValidityCheck(AdQualityMap& res_pool){
  int erase_cnt = 0;
  for (AdQualityMap::iterator it = res_pool.begin(); it != res_pool.end(); ) {
    if (it->second <= 1.0 && it->second >= 0.0) {
      ++it; 
    } else {
      res_pool.erase(it++); 
      ++erase_cnt;
    }
  }
  MCE_INFO("[DemoLoaderCache]::DataValidityCheck erase " << erase_cnt << " invalid record");
}

void DemoLoaderCache::ShowAllCtr(){
	AdQualityMap::iterator it = demo_pool.begin();
	stringstream sctr;
	for(; it != demo_pool.end(); ++it){
		sctr << "AdCTR:" << it->first << " : " << it->second <<" ;"; 
		MCE_INFO(sctr.str());	
		sctr.str("");
	}
	MCE_INFO("[ShowAllCtr] AdCtr size:"<< demo_pool.size());
}
