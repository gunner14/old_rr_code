/*
 * GeneralQuerySuggestion.cpp
 *
 *  Created on: 2012-8-27
 *      Author: cm
 */

#include "GeneralQSI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace com::renren::search::generalquerysuggestion;
using namespace MyUtil;
using namespace xce::serverstate;


void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	MCE_INFO("initialize -----------------------");
	service.getAdapter()->add(&GeneralQSI::instance(), service.createIdentity("GeneralQS",""));
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "GeneralQS.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("GeneralQS.Interval", 5);

	ServerStateSubscriber::instance().initialize("ControllerGeneralQS",&GeneralQSI::instance(), mod, interval, new MyUtil::SearchControllerChannel);
	ServerStateSubscriber::instance().setStatus(1);
	GeneralQSI::instance().init();
}

void GeneralQSI::init() {
	MCE_INFO("initial dict and config---------");
	string path = "/data/xce/XiaoNeiSearch/etc/config.properties";
	qs = new ZJQuerySuggestion();
	if (qs->init(path)) {
		MCE_INFO("init success!");
	} else {
		MCE_INFO("init failure!");
	}
	return ;

}

MyUtil::IntSeq GeneralQSI::search(int uid,const string& cquery, int len, int limit, bool isPre, const ::Ice::Current& ) {
  MCE_INFO("search for query :"<<cquery);
  string query = cquery;
  return qs->search(uid, query, len,limit, isPre);
}

