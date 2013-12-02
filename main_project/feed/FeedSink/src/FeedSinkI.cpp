/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "FeedSinkI.h"
#include "LogWrapper.h"
#include "ServiceI.h"
#include "Singleton.h"
#include <boost/foreach.hpp>
#include "FeedNews50Adapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "QueryRunner.h"
using namespace xce::feed;
using namespace std;
using namespace MyUtil;
using namespace boost;
using namespace com::xiaonei::xce;
//using namespace com::xiaonei::xce;


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
};

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&FeedSinkI::instance(), service.createIdentity("M",			""));

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "FeedSink.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault( "FeedSink.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerFeedSinkR", &FeedSinkI::instance(), mod, interval,
      new XceFeedChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

	FeedNews50Handler::instance().LoadWhitelist();
}

void FeedSinkI::AddFeed(const FeedItem& fitem,
    const FeedConfigPtr& config, const MyUtil::Int2IntMap& id2weight,
    const Ice::Current&) {
	MCE_INFO("FeedSinkI::AddFeed. fid:" << fitem.feed
			<< " stype:" << (fitem.type & 0xFFFF)
			<< " actor:" << fitem.actor
			<< " time:" << fitem.time
			<< " merge:" << fitem.merge
			<< " target size:" << id2weight.size()
	);
	FeedNews50Handler::instance().AddFeed(fitem, config,id2weight);
}

void FeedSinkI::SetRead(int uid, int stype, Ice::Long merge,
    const Ice::Current&) {
	MCE_INFO("FeedSinkI::SetRead uid:" << uid
			<< " stype:" << stype
			<< " merge:" << merge
	);

	FeedNews50Handler::instance().SetRead(uid,stype,merge);
}

void FeedSinkI::SetReadById(int uid, long fid,
    const Ice::Current&) {
	MCE_INFO("FeedSinkI::SetRead uid:" << uid
			<< " fid:" << fid
	);

	FeedNews50Handler::instance().SetReadById(uid,fid);
}

void FeedSinkI::SetReadAll(int uid,
    const Ice::Current&) {
	MCE_INFO("FeedSinkI::SetRead uid:" << uid);

	FeedNews50Handler::instance().SetReadAll(uid);
}
void FeedSinkI::ReloadWhitelist(const Ice::Current&){
	FeedNews50Handler::instance().LoadWhitelist();
}

void  FeedSinkI::GetFeedData(int uid, bool hot, bool bystype,const MyUtil::IntSeq & stypes, int begin, int limit,const Ice::Current&){
	MCE_INFO("FeedSinkI::GetFeedData. uid:" << uid << " hot:" << hot << " bystype:" << bystype << " stypes size:" << stypes.size()
			<< " begin:" << begin << " limit:" << limit);
}

void FeedSinkI::MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark, const Ice::Current& ){
	MCE_INFO("FeedMarkI::MarkFeed. uid:" << uid << " fid:" << fid << " source:" << source << " stype:" << stype
			<< " actor:" << actor << " mark:" << mark);
	FeedNews50Handler::instance().MarkFeed(uid,fid,source,stype,actor,mark);
}
//void FeedNews50Handler::Init(){
//	int uids[] = {
//			239556923,//zjc
//			241610785,//kw
//			220678672,//yjw
//			347348947,//jw1
//			225092473,//ld
//			40125272,//lq
//			34924406,// 阮欣
//			237295999,// 王冲
//			260168919 ,// 刘煜宸
//			1001425569,// 田立群
//			1920648986,// 童冠
//			200569301,// 吴惠彩
//			83970,// 徐瑞航
//	};
//	testers_.insert(uids,uids+sizeof(uids)/sizeof(uids[0]));
//	MCE_INFO("FeedNews50Handler::Init. testers size:" << testers_.size() << " testers:" << PrintHelper::Print(testers_));
//}

void FeedNews50Handler::LoadWhitelist(){
	MCE_INFO("@@@@ BEGIN");
	set<int> testers;
	Statement sql;
	sql << "select * from feednews_whitelist";
	MCE_INFO("@@@@ " << __LINE__);
	mysqlpp::StoreQueryResult res;
	try {
		res = QueryRunner("feed_db", CDbRServer, "feednews_whitelist").store(sql);
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedNews50Handler::LoadWhitelist --> error:" << e);
	} catch (std::exception& e) {
		MCE_WARN("FeedNews50Handler::LoadWhitelist --> error:" << e.what());
	}

	if(!res){
		MCE_WARN("FeedNews50Handler::LoadWhitelist. res null");
	} else {
		for (size_t i = 0; i<res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			int uid = (int)row["uid"];
			int view = (int)row["view"];
			testers.insert(uid);
		}
	}
	IceUtil::Mutex::Lock lock(mutex_);
	MCE_INFO("FeedNews50Handler::Load. testers size:" << testers.size() << " testers:" << PrintHelper::Print(testers,1000) << " old testers:" << PrintHelper::Print(testers_,1000));
	testers_.swap(testers);
}

Int2IntMap FeedNews50Handler::GetTester(const MyUtil::Int2IntMap& id2weight){
	Int2IntMap res;
	IceUtil::Mutex::Lock lock(mutex_);
	BOOST_FOREACH(const Int2IntMap::value_type & kv,id2weight){
		if(testers_.count(kv.first)){
			res.insert(kv);
		}
	}
	return res;
}

bool FeedNews50Handler::IsTester(int uid){
	IceUtil::Mutex::Lock lock(mutex_);
	return testers_.count(uid);
}


void FeedNews50Handler::AddFeed(const FeedItem& fitem,
    const FeedConfigPtr& config, const MyUtil::Int2IntMap& id2weight){
	Int2IntMap testers ;
	try{
	 testers = GetTester(id2weight);
	 if(!testers.empty()){
		FeedNews50Adapter::instance().addFeed(fitem,config,testers);
	 }

		MCE_INFO("FeedNews50Handler::AddFeed. fid:" << fitem.feed
				<< " stype:" << (fitem.type & 0xFFFF)
				<< " actor:" << fitem.actor
				<< " time:" << fitem.time
				<< " merge:" << fitem.merge
				<< " target size:" << id2weight.size()
				<< " testers size:" << testers.size()
		);

	}	catch(Ice::Exception& e) {
		MCE_WARN("FeedNews50Handler::AddFeed. fid:" << fitem.feed
				<< " stype:" << (fitem.type & 0xFFFF)
				<< " actor:" << fitem.actor
				<< " time:" << fitem.time
				<< " merge:" << fitem.merge
				<< " testers size:" << testers.size()
				<<" error:" << e);
	} catch (...){
		MCE_WARN("FeedNews50Handler::AddFeed. fid:" << fitem.feed
				<< " stype:" << (fitem.type & 0xFFFF)
				<< " actor:" << fitem.actor
				<< " time:" << fitem.time
				<< " merge:" << fitem.merge
				<< " testers size:" << testers.size()
				<<" unknown error" );
	}
}

void FeedNews50Handler::SetRead(int uid, int stype, Ice::Long merge){
	Int2IntMap testers ;
	try{
	 if(IsTester(uid)){
		FeedNews50Adapter::instance().setRead(uid,stype,merge);
	 }
		MCE_INFO("FeedNews50Handler::SetRead uid:" << uid
				<< " stype:" << stype
				<< " merge:" << merge
		);
	}	catch(Ice::Exception& e) {
	} catch (...){
	}
}

void FeedNews50Handler::SetReadById(int uid, long fid){
	Int2IntMap testers ;
	try{
		if(IsTester(uid)){
			FeedNews50Adapter::instance().setReadById(uid,fid);
		}
		MCE_INFO("FeedNews50Handler::SetRead uid:" << uid
				<< " fid:" << fid
		);
	}	catch(Ice::Exception& e) {
	} catch (...){
	}
}

void FeedNews50Handler::SetReadAll(int uid){
	Int2IntMap testers ;
	try{
		if(IsTester(uid)){
			FeedNews50Adapter::instance().setReadAll(uid);
		}
		MCE_INFO("FeedNews50Handler::SetRead uid:" << uid);
	}	catch(Ice::Exception& e) {
	} catch (...){
	}
}

void  FeedNews50Handler::MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark){
	Int2IntMap testers ;
	try{
		if(IsTester(uid)){
			FeedNews50Adapter::instance().MarkFeed(uid,fid,source,stype,actor,mark);
		}
		MCE_INFO("FeedNews50Handler::SetRead uid:" << uid);
	}	catch(Ice::Exception& e) {
		MCE_WARN("FeedNews50Handler::MarkFeed. uid:" << uid << " err:" << e);
	}
}

//-------------------------------------------------------------------------------------------
void TargetPrinter::Handle(const FeedItem& fitem,const MyUtil::Int2IntMap& id2weight){
	if(fitem.actor == 366549583  || fitem.actor == 425544141){
		set<int> targets;
		BOOST_FOREACH(const Int2IntMap::value_type & p, id2weight){
			targets.insert(p.first);
		}
		MCE_INFO("FeedNews50Handler::Handle. fid:" << fitem.feed
				<< " stype:" << (fitem.type & 0xFFFF)
				<< " actor:" << fitem.actor
				<< " time:" << fitem.time
				<< " merge:" << fitem.merge
				<< " target size:" << id2weight.size()
				<< " target:" << PrintHelper::Print(targets)
		);
	}
}
