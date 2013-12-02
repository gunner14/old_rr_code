/*
 * DemoLoaderCache.cpp
 *
 *  Created on: sep 20,2012 
 *      Author: yu.fu 
 */

#include "DemoLoaderCache.h"
#include <connection.h>
#include <query.h>
#include <string>
#include <algorithm>
#include "EdmConfig.h"
#include "QueryRunner.h"
#include "EdmQualityUtil.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
using namespace boost;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
	printf("got crash %s %s\n", dump_path, minidump_id);
	return true;
}

void ::MyUtil::initialize() {
	static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&xce::ad::DemoLoaderCache::instance(), service.createIdentity("M",""));

	string Ad_DbInstance = service.getCommunicator()->getProperties()->getPropertyWithDefault("AdDbInstance","ad_st");
	string Edm_DbInstance = service.getCommunicator()->getProperties()->getPropertyWithDefault("EdmDbInstance","ad_st");
	xce::ad::EdmDemoConfig::instance().SetAdDBST(Ad_DbInstance);
	xce::ad::EdmDemoConfig::instance().SetEdmDBST(Edm_DbInstance);

	//TODO 加载配置和业务数据
	xce::ad::EdmDemoConfig::instance().LoadConfig();
	MyUtil::TaskManager::instance().schedule(new xce::ad::EdmDemoConfig::ConfigLoadTimer(0));
	MyUtil::TaskManager::instance().execute(new xce::ad::InitTask());

	//TODO HA
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("EdmDemoLoaderCache.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("EdmDemoLoaderCache.Interval",5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerEdmDemoLoaderCache",&xce::ad::DemoLoaderCache::instance(),mod,interval,new EdmChannel());
	MCE_INFO("MyUtil::initialize.mod:"<<mod<<" interval:"<<interval);
}

namespace xce{
namespace ad{
using namespace mysqlpp;

EdmQualityMap DemoLoaderCache::GetEdmPool(const ::Ice::Current& current){
	MCE_INFO("EdmDemoLoaderCache::GetEdmPool--> call from " << current.con->toString());
	EdmQualityMap result;
	IceUtil::RWRecMutex::RLock lock(mutex_);  
	result = edm_pool_;
	MCE_INFO("EdmDemoLoaderCache::GetEdmPool,pool size:"<<result.size());
	return result;
}

EdmQualityMap DemoLoaderCache::GetAdEdmPool(const ::Ice::Current& current){
	MCE_INFO("EdmDemoLoaderCache::GetAdEdmPool--> call from " << current.con->toString());
	EdmQualityMap result;
	IceUtil::RWRecMutex::RLock lock(mutex_);  
	result = ad_edm_pool_;
	MCE_INFO("EdmDemoLoaderCache::GetAdEdmPool,pool size:"<<result.size());
	return result;
}

void DemoLoaderCache::DataValidityCheck(EdmQualityMap& res_pool){
	int erase_cnt = 0;
	for (EdmQualityMap::iterator it = res_pool.begin(); it != res_pool.end(); ) {
		if (it->second <= 1.0 && it->second >= 0.0) {
			++it; 
		} else {
			res_pool.erase(it++); 
			++erase_cnt;
		}
	}
	MCE_INFO("[EdmDemoLoaderCache]::DataValidityCheck erase " << erase_cnt << " invalid record");
}

void DemoLoaderCache::LoadEdmPool(){
	EdmQualityMap res_pool;
	MyUtil::Date begin = MyUtil::Date::seconds(time(NULL));
	Ice::Long begin_time_pos = GetDay(begin.year(),begin.month(),begin.day());
	MyUtil::Date end = MyUtil::Date::seconds(time(NULL) - DAYCOUNT * HOURCOUNT * 3600);
	Ice::Long end_time_pos = GetDay(end.year(),end.month(),end.day());
	try{
		string dbsource = EdmDemoConfig::instance().GetEdmDBST(); 
		stringstream sql;
		/****************no_gender*****************************/
		sql<< "Select adgroup_id as group_id,stage, sum(pv_count) as sum_impr, sum(click_count) as sum_click from Log_DMGGS where time <= "<< begin_time_pos << " and time >= "<< end_time_pos <<" group by adgroup_id,stage";
		LoadPool(dbsource,sql.str(),edm_no_gender,res_pool);

		/****************no_stage*****************************/
		sql.str("");
		sql.clear();
		sql<< "Select adgroup_id as group_id, gender,sum(pv_count) as sum_impr, sum(click_count) as sum_click from Log_DMGGS where time <= "<< begin_time_pos << " and time >="<< end_time_pos <<" group by adgroup_id,gender";
		LoadPool(dbsource,sql.str(),edm_no_stage,res_pool);

		/****************spid_type*****************************/
		sql.str("");
		sql.clear();
		sql<< "Select gender,stage, sum(pv_count) as sum_impr, sum(click_count) as sum_click from Log_DMGGS where time <= " << begin_time_pos << " and time >="<< end_time_pos <<" group by gender,stage";
		LoadPool(dbsource,sql.str(),edm_spid_type,res_pool);

		/****************two-feature*****************************/
		sql.str("");
		sql.clear();
		sql<< "Select adgroup_id as group_id, gender,stage,sum(pv_count) as sum_impr, sum(click_count) as sum_click from Log_DMGGS where time <= " << begin_time_pos << " and time >="<< end_time_pos <<" group by adgroup_id,gender,stage";
		LoadPool(dbsource,sql.str(),edm_two_feature,res_pool);

		/****************all-type*****************************/
		sql.str("");
		sql.clear();
		sql<< "Select adgroup_id as group_id, sum(pv_count) as sum_impr, sum(click_count) as sum_click from Log_DMGGS where time <= "<< begin_time_pos << " and time >= "<< end_time_pos <<" group by adgroup_id";
		LoadPool(dbsource,sql.str(),edm_all_type,res_pool);

	}catch(std::exception& e) {
		MCE_WARN("LoadByTime --> load edmpool error: " << e.what());
	} catch (...) {
		MCE_WARN("LoadByTime --> load edmpool error!");
	}
	DataValidityCheck(res_pool);
	{
		IceUtil::RWRecMutex::WLock lock(mutex_);
		edm_pool_.swap(res_pool);
		MCE_INFO("[DemoLoaderCache]::LoadEdmPool End, size is " << edm_pool_.size());
	}

}

////////////////////////////////////
void DemoLoaderCache::LoadAdEdmPool(){
	EdmQualityMap res_pool;
	MCE_INFO("[DemoLoaderCache]::try to load.LoadAdEdmPool begin");
	MyUtil::Date begin = MyUtil::Date::seconds(time(NULL));
	string begin_time = begin.str("%Y-%m-%d %H:%M:%S");
	Ice::Long begin_time_pos = GetDay(begin.year(),begin.month(),begin.day());
	MyUtil::Date end = MyUtil::Date::seconds(time(NULL) - DAYCOUNT * HOURCOUNT * 3600);
	Ice::Long end_time_pos = GetDay(end.year(),end.month(),end.day());
	MyUtil::Date state_2_end = MyUtil::Date::seconds(time(NULL) - 2 * HOURCOUNT * 3600);
	string end_time = end.str("%Y-%m-%d %H:%M:%S");
	string state_2_end_time = state_2_end.str("%Y-%m-%d %H:%M:%S");
	Ice::Long state_2_end_time_pos = GetDay(state_2_end.year(),state_2_end.month(),state_2_end.day());

	try {
		string dbsource = EdmDemoConfig::instance().GetAdDBST();
		stringstream sql;
		/****************no_gender*****************************/
		sql << "select member_id, adgroup_id as group_id,  stage, sum(pv_count) as sum_impr, sum(click_count) as sum_click from rt_DZPMGGS "
			" where adzone_id in "<< ad_zone_list << " and time <= " << begin_time_pos << " and time >= "
			<< end_time_pos << " "
			<< " group by adgroup_id,stage ";
		LoadPool(dbsource,sql.str(),ad_edm_no_gender,res_pool);

		/****************no_stage*****************************/
		sql.str("");
		sql.clear();
		sql << "select adgroup_id as group_id,gender, sum(pv_count) as sum_impr, "
			<<" sum(click_count) as sum_click from rt_DZPMGGS  where adzone_id in " << ad_zone_list << " and time <= " << begin_time_pos
			<< " and time >=" << end_time_pos <<" group by adgroup_id, gender ";
		LoadPool(dbsource,sql.str(),ad_edm_no_stage,res_pool);

		/*******************spid_type*************************************/
		sql.str("");
		sql.clear();
		sql << "select adgroup_id as group_id,gender, stage,sum(pv_count) as sum_impr, "
			<<" sum(click_count) as sum_click from rt_DZPMGGS  where adzone_id in "<< ad_zone_list << " and time <= " << begin_time_pos
			<< " and time >=" << end_time_pos <<" group by gender ,stage";
		LoadPool(dbsource,sql.str(),ad_edm_spid_type,res_pool);

		/*************************************two feature**********************************/
		sql.str("");
		sql.clear();
		sql << "select adgroup_id as group_id, gender, stage, sum(pv_count) as sum_impr, sum(click_count) as sum_click from rt_DZPMGGS  where adzone_id in "
			<< ad_zone_list << " and time <= " << begin_time_pos  << " and time >=" << end_time_pos << "" <<" group by adgroup_id , gender,stage ";
		LoadPool(dbsource,sql.str(),ad_edm_two_feature,res_pool);

		/******************************all**********************************/
		sql.str("");
		sql.clear();
		sql << "select adgroup_id as group_id, sum(pv_count) as sum_impr, sum(click_count) as sum_click from rt_DZPMGGS  where adzone_id in "
			<< ad_zone_list << " and time <= " << begin_time_pos  << " and time >=" << state_2_end_time_pos << "" << " group by adgroup_id ";
		LoadPool(dbsource,sql.str(),ad_edm_all_type,res_pool);
	}catch (std::exception& e) {
		MCE_WARN("LoadByTime --> load adedmpool error: " << e.what());
	} catch (...) {
		MCE_WARN("LoadByTime --> load adedmpool error!");
	}
	DataValidityCheck(res_pool);
	{
		IceUtil::RWRecMutex::WLock lock(mutex_);
		ad_edm_pool_.swap(res_pool);
		MCE_INFO("[DemoLoaderCache]::LoadAdEdmPool End, size is " << ad_edm_pool_.size());
	}
}

void DemoLoaderCache::LoadPool(const string & dbsource,const string& sql, const int& type, EdmQualityMap & res_pool){
	MCE_DEBUG("DemoLoaderCache::LoadPool type:"<< type << ", DbSource:" << dbsource << " ,sql:"<< sql);
	try{
		Statement ssql; 
		ssql << sql;
		mysqlpp::StoreQueryResult res = QueryRunner(dbsource.c_str(), CDbRServer).store(ssql);
		MCE_INFO("DemoLoaderCache::LoadPool type:"<< type << " ,DBSource:" << dbsource << ", res.size:" << res.num_rows()  << ", sql:"<< sql);
		if (!res) {
			MCE_WARN("Load Ctr Data Error,sql:" << sql);
			return;
		}
		for (size_t i = 0; i < res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			Ice::Long group_id = NO_USE;
			Ice::Int gender = NO_USE;
			Ice::Int stage = NO_USE;
			switch(type%10){
				case 1:
					stage = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["stage"]);
					group_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["group_id"]);
					break;
				case 2:
					gender = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["gender"]);
					group_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["group_id"]);
					break;
				case 3:
					gender = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["gender"]);
					stage = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["stage"]);
					break;
				case 4:
					group_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["group_id"]);
					break;
				case 5:
					gender = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["gender"]);
					stage = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["stage"]);
					group_id = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["group_id"]);
					break;
			}
			Ice::Long impr = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["sum_impr"]);
			Ice::Long click = boost::lexical_cast<Ice::Long> ((mysqlpp::Null<string,mysqlpp::NullIsZero>)row["sum_click"]);
			string key = GetKey(group_id,gender,stage);
			if(IsCredible(impr, click, type)) {
				res_pool[key] = (double) click / (double) impr;
			}
		}
	}catch (std::exception& e) {
		MCE_WARN("LoadByTime --> load ctr pool error: " << e.what());
	} catch (...) {
		MCE_WARN("LoadByTime --> load ctr pool error!");
	}
}
void InitTask::handle(){
	try {
		xce::ad::DemoLoaderCache::instance().LoadEdmPool();
		MCE_INFO("DemoLoaderCache::-> LoadEdmPool; DONE");
		xce::ad::DemoLoaderCache::instance().LoadAdEdmPool();
		MCE_INFO("DemoLoaderCache::-> LoadAdEdmPool; DONE");
	} catch (std::exception& e) {
		MCE_WARN("DemoLoaderCache::LoadPool ERR  line:" << __LINE__ << " err:" << e.what());
	} catch (...) {
		MCE_WARN("DemoLoaderCache::LoadPool reload ERR  line:" << __LINE__);
	}  

	usleep(2 * 60 * 1000 * 1000);
	MyUtil::TaskManager::instance().execute(new ReloadTask());
}   

void ReloadTask::handle(){
	while(true) {
		try {
			xce::ad::DemoLoaderCache::instance().LoadEdmPool();
			MCE_INFO("DemoLoaderCache::-> LoadEdmPool; ReLoad DONE");
			xce::ad::DemoLoaderCache::instance().LoadAdEdmPool();
			MCE_INFO("DemoLoaderCache::-> LoadAdEdmPool; ReLoad DONE");
			usleep(2 * 60 * 1000 * 1000);
		} catch (std::exception& e) {
			MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__ << " err:" << e.what());
		} catch (...) {
			MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__);
		}
	}
}

}//end ad
}

