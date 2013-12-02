/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "CarIndexManager.h"
#include "LogWrapper.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "QueryRunner.h"
#include "boost/foreach.hpp"
#include "Ice/Object.h"
#include <map>
#include <vector>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"


using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace boost;
using namespace com::xiaonei::search2::model;
using namespace com::xiaonei::search2::server;

//class ReloadTask: public Timer{ 
class ReloadTask: public Timer, public Singleton<ReloadTask> { 
public:
	ReloadTask() :
		Timer(30 * 1000, 0) { }
	void handle() {
		MCE_INFO("ReloadTask.");
		IndexManagerI::instance().Reload();
	}
	private:
};

void MyUtil::initialize() {
	MCE_INFO("initialize begin");
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&IndexManagerI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "CarIndexManager.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault( "CarIndexManager.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerCarIndexManager", &IndexManagerI::instance(), mod, interval, new SearchControllerChannel());
    	xce::serverstate::ServerStateSubscriber::instance().setStatus(1);
	IndexManagerI::instance().Reload();
	MCE_INFO("initialize done");
}
//class ReloadTask: public Timer, public Singleton<ReloadTask> { 

class TimeStat {
public:
	TimeStat() {  reset(); }
	void reset() { gettimeofday(&_begin, NULL); }
	float Get(bool clear = true) {
		gettimeofday(&_end, NULL);
		float timeuse = 1000000 * (_end.tv_sec - _begin.tv_sec)
			+( _end.tv_usec - _begin.tv_usec);
		if(clear) reset();
		return timeuse / 1000;
	}
private:
	timeval _begin, _end;
};

string GetIp(const Ice::Current& current){
	if(!current.con){
		MCE_WARN("Current con is false")
			return "NULL";
	}
	string ip = current.con->toString();
	size_t found = ip.find('\n');
	if (found == string::npos) {
		return ip;
	}
	string res ( ip.begin() + found + 1,ip.end());
	return res;
}


IndexResultPtr IndexManagerI::searchIndex(const IndexCondition & condition,int begin,int limit ,const Ice::Current& ){
	return new IndexResult();
}
IndexResultPtr IndexManagerI::searchIndexWithRestriction(const IndexCondition & condition,int begin,int limit ,const string & caller ,const Ice::Current& cur){
	IndexResultPtr res = new IndexResult();
	TimeStat ts;
	//if(condition.queryCondition == 0){
	//	MCE_WARN("IndexManagerI::searchIndexWithRestriction. queryCondition is null");
	//	return res;
	//}
	//if(condition.opUserCondition == 0){
	//	MCE_WARN("IndexManagerI::searchIndexWithRestriction. opUserCondition is null");
	//	return res;
	//}
	map<string,string>::const_iterator it = condition.queryCondition.find("query");
	string query = "";
	if(it == condition.queryCondition.end()){
		res->currentPos = begin;
		res->totalCount = 0;
		MCE_INFO("IndexManagerI::searchIndexWithRestriction. no query");
		return res;
	}

	query = it->second;
	CarInfoPtr cinfo;
	{
		IceUtil::Mutex::Lock lock(mutex_);				
		map<string,CarInfoPtr>::iterator cit = carmap_.find(query);
		if(cit != carmap_.end()){
			cinfo = cit->second;
		}
	}
	if(!cinfo){
		res->currentPos = begin;
		res->totalCount = 0;
		MCE_INFO("IndexManagerI::searchIndexWithRestriction. no found. query:" << query);
		return res;
	}
	IndexResultItem item ;	
	item.id = cinfo->id;
	item.content = cinfo->content;
	item.type = Car;
	res->currentPos = begin;
	res->timeCost = ts.Get();
	res->totalCount = 1;
	res->typedCount[Car] = 1;
	res->contents.push_back(item);
	MCE_INFO("IndexManagerI::searchIndexWithRestriction. found. query:" << query << " cinfo:" << cinfo->Dump());
	return res;
}
void IndexManagerI::Reload(){
	map<string,CarInfoPtr> carmap_swap;
	try{
		Statement sql;
		sql<<"SELECT * FROM cw_topic" ;
		mysqlpp::StoreQueryResult res = QueryRunner("chewen", CDbRServer, "cw_topic").store(sql);
		if (!res) {
			MCE_WARN("IndexManagerI::Reload fail");
			return;
		}

		mysqlpp::Row row;
		for (size_t i = 0; i<res.num_rows(); ++i) {
			row = res.at(i);
			int id = int(row["id"]);
			CarInfoPtr c = new CarInfo(id);
			c->content["id"] = string(row["id"]);
			c->content["icon2"] = string(row["icon2"]);
			string name = string(row["name"]);
			c->content["name"] = name;
			carmap_swap[name] = c;
			MCE_INFO("IndexManagerI::Reload:count:" << c->Dump());
		}

	}catch(std::exception& e){
		MCE_WARN("IndexManagerI::Reload --> load chewen err, " << e.what());
	}
	IceUtil::Mutex::Lock lock(mutex_);				
	MCE_INFO("IndexManagerI::Reload. old:" << carmap_.size() << " new:" << carmap_swap.size());
	carmap_swap.swap(carmap_);

	TaskManager::instance().schedule(&ReloadTask::instance());
	
}
