/*
 * FeedMark.cpp
 *
 *  Created on: Feb 07, 2012
 *      Author: yejingwei
 */

#include "LogWrapper.h"
#include "QueryRunner.h"
#include "ServiceI.h"
#include "FeedMarkI.h"
//#include "ScoreCache.h"
#include "TaskManager.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
//#include "Date.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "DbHelper.h"
#include "FeedSinkAdapter.h"

using namespace boost;
using namespace std;
using namespace xce::feed;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::serverstate;



void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedMarkI::instance(), service.createIdentity("M", ""));
	
  //controller
  int interval = 5;
	int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedMark.Mod", 0);
	string controller = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedMark.Controller", 
        "ControllerFeedMark");
	ServerStateSubscriber::instance().initialize(controller, &FeedMarkI::instance(), 
        mod, interval, new XceFeedControllerChannel());
}

void WriteTask::handle() {
  DbHelper::instance().MarkFeed(uid_, actor_, mark_);//update db
}

void FeedMarkI::MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark, const Ice::Current& ){
	MCE_INFO("FeedMarkI::MarkFeed. uid:" << uid << " fid:" << fid << " source:" << source << " stype:" << stype << " actor:" << actor
			<< " mark:" << mark);
  try {
    FeedSinkAdapter::instance().MarkFeed(uid,fid,source,stype,actor,mark);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedMarkI::MarkFeed. uid:" << uid << " call FeedSink err: " << e);
  } catch (std::exception& e) {
    MCE_WARN("FeedMarkI::MarkFeed. uid:" << uid << " call FeedSink err: " << e.what());
  }

  if (uid < 0) {
    MCE_WARN("FeedMarkI::MarkFeed   uid < 0 ?   uid:" << uid
              << " fid "<<fid<<" source "<<source<<" stype " <<stype
              << " actor "<<actor<<" mark "<<mark ); 
    return ;
  }

  UserMarkStatusPtr uinfo= UserMarkStatusPool::instance().Find(uid);
  if (uinfo) {
    if (uinfo->IsSameMark(actor, mark)) {
      MCE_INFO("FeedMarkI::MarkFeed -----> uid:" <<uid<< " stay same in mem  ,  Don't Need Changed !! "); 
      return ;
    } else {
      MCE_INFO("FeedMarkI::MarkFeed -----> uid:" <<uid<< " update in mem  ,  But Need Changed !! "); 
      uinfo->Add(actor, mark);
    }
  } 
  
  MCE_INFO("FeedMarkI::MarkFeed ----->  uid:" <<uid<< " not in mem  ||  Need Change !! --> DB" );
  TaskManager::instance().execute(new WriteTask(uid, actor, mark));

}


MarkInfoPtr FeedMarkI::GetMarkInfo(int uid, const Ice::Current& ){
  UserMarkStatusPtr uinfo;  
  if (uid < 0) {
    MCE_WARN("FeedMarkI::GetMarkInfo   uid: " << uid << " < 0 ?");
    uinfo = new UserMarkStatus(uid);
  } else {
    uinfo = Locate(uid);
	  MCE_INFO("FeedMarkI::GetMarkInfo  -->  " << uinfo->ToString());
  }

  return uinfo->MakeMarkInfo();
}


UserMarkStatusPtr FeedMarkI::Locate(int uid){
	//find in cache
  UserMarkStatusPtr uinfo = UserMarkStatusPool::instance().Find(uid);
	if (uinfo) {
		MCE_INFO("FeedMarkI::Locate.  --->   user in mem.    uid: " <<  uid);
		return uinfo;
	}
	MCE_INFO("FeedMarkI::Locate.  --->    user not in mem.    uid:" << uid);

  // find in db
  uinfo = DbHelper::instance().GetFeedMark(uid);
  if (uinfo) {
    UserMarkStatusPool::instance().Put(uid, uinfo);
    MCE_INFO("FeedMarkI::Locate.  --->   user in db    &&   putted in cache .   uid: "<< uid);
    return uinfo;
  }
	MCE_INFO("FeedMarkI::Locate.  --->    user not in db. uid:" << uid);
  
  MCE_WARN("FeedMarkrI::Locate.  --->    uid:" << uid << " locate fail");
  uinfo = new UserMarkStatus(uid);// empty
  return uinfo;
}


class PrintHelper {
public:
  template<typename T>
  static string Print(const T & container,int limit = 30) {
    ostringstream ios;

    ios << "(";
    int idx = 0;
    typedef typename T::value_type vt;
    BOOST_FOREACH(const vt & v,container){
    	if(idx++ > 0) ios << ",";
    	ios << v;
      if(idx >= limit) break;
    }
    ios << ")";
    return ios.str();
  }
} ;





