/*
 * =====================================================================================
 *
 *       Filename:  FriendInputManagerI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月20日 
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  tanbokan (), min.chen1@renren-inc.com
 *        Company:  opi-corp.com
 *
 * =====================================================================================
 */
#include "BuddyByIdReplicaAdapter.h"
#include "FriendInputManagerI.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <boost/foreach.hpp>

using namespace com::xiaonei::xce;
using namespace com::xiaonei::services::FriendInput;
using namespace xce::buddy::adapter;
using namespace xce::serverstate;

// =============== MyUtil::initialize ====================================
void MyUtil::initialize() {
  MCE_INFO("initialze~");
	ServiceI& service = ServiceI::instance();
	service.registerObjectCache(FriendInputManagerI::instance().FRIENDINPUT_DATA,"FI");
  string zk = service.getCommunicator()->getProperties()->getProperty("FriendInput.ZK");
  string loaderpath = service.getCommunicator()->getProperties()->getProperty("FriendInput.LoadPath");
  service.getAdapter()->add(&FriendInputManagerI::instance(), service.createIdentity("Manager",""));
  FriendInputManagerI::instance().init(zk,loaderpath);
}

//==============FriendInputManagerI============
FriendInputManagerI::FriendInputManagerI():FRIENDINPUT_DATA(0),TIMEOUT_CACHE(5*60),DB_FRIENDINPUT_PINYIN("friendinput_pinyin") {
  adapter.Init(zk_,loaderpath_);

  Statement sql;
  sql << "SELECT hanzi, pinyin FROM search_pinyin";
  PinYinResultHandler handler(dict_);
  QueryRunner(DB_FRIENDINPUT_PINYIN, CDbRServer).query(sql, handler);
}

void FriendInputManagerI::init(string& zk,string& loaderpath) {
  zk_ = zk;
  loaderpath_ = loaderpath;
};


MyUtil::IntSeq FriendInputManagerI::getFriendInput(Ice::Int hostID,const string& condition,Ice::Int limit,const Ice::Current& crt) {
	MCE_INFO("getFriendInput hostID= "<<hostID<<" condition="<<condition<<" limit="<<limit<<" condition size:"<<condition.size());

  if(limit <= 0) {
    MCE_WARN("limit is lower than zero~");
    return vector<int>();
  }

  int len = FriendSuggestUtil::getStrLen(condition);
  MCE_DEBUG("conditin len is:"<<len);
  if(len > 15 || len == 0) {
    MCE_WARN("condition len is:"<<len<<" is zero or large than 15");
		return vector<int>();
  }	else  {
		FriendInputDataIPtr  server = ServiceI::instance().findObject<FriendInputDataIPtr>(FRIENDINPUT_DATA, (long)hostID);
		if(server != NULL){
			time_t old = server->getTime();
			int duration = time(NULL) - old;
			MCE_DEBUG("duration is:"<<duration);
			if(duration >= TIMEOUT_CACHE)  {//remove and add
        MCE_DEBUG("cache timeout~~");
				ServiceI::instance().removeObject(FRIENDINPUT_DATA, (long)hostID);
		    createObject(hostID,server);
      } 
		} else {//add
       createObject(hostID,server);
    }
   MyUtil::IntSeq pids;
   if(server == NULL) {
     MCE_DEBUG("the server is null,createObject failure~");
     return vector<int>();
   }

   pids = server->search(condition,limit);

   if(pids.size() == 0) {
     MCE_INFO("getFriendInputManagerI result is null~");
   } else {
     MCE_INFO("res is not null~ and res.size()="<<pids.size()<<"FriendInputManagerI::getFriendInput done hostID= "<<hostID<<" condition="<<condition<<" limit="<<limit << " res:"  << PrintHelper::Print(pids));
   }
   return pids;
	}
};


void FriendInputManagerI::reLoadFriend(Ice::Int hostID,const Ice::Current& crt)
{
	MCE_INFO("reLoadFriend hostID="<<hostID);
	FriendInputDataIPtr obj = ServiceI::instance().findObject<FriendInputDataIPtr>(FRIENDINPUT_DATA, (long)hostID);
	if(!obj){
		 ServiceI::instance().addObject(obj, FRIENDINPUT_DATA, (long)hostID);
	}
};

//====================================================================
void FriendInputManagerI::createObject(int hostID,FriendInputDataIPtr& server) {
  server = new FriendInputDataI(hostID,dict_,adapter);
  server->buildIndex();
  ServiceI::instance().addObject(server, FRIENDINPUT_DATA, (long)hostID);
}
