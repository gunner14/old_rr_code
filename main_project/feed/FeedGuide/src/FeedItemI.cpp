/*
 * FeedItem.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: rong.tang
 */


#include "client/linux/handler/exception_handler.h"
#include "IceLogger.h"
#include "ServiceI.h"
#include "PerformanceStat.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <boost/foreach.hpp>
#include "util/cpp/InvokeMonitor.h"

#include "FeedItemWrapperReplicaAdapter.h"
#include "FeedCacheAdapter.h"
#include "FeedContentAdapter.h"
#include "FeedMemcLoaderAdapter.h"

#include "FeedItemI.h"
#include "FeedBuilder.h"
#include "SharedFeedCache.h"

using namespace xce::feed;


void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedItemGuideManagerI::instance(), service.createIdentity("FIM", ""));

  //HA
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FeedGuide.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedGuide.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize(
  		  "ControllerFeedGuideR", &FeedItemGuideManagerI::instance(), mod, interval,  new XceFeedControllerChannel());

  MCE_INFO("FeedGuideI::RegisterControllerR. mod:" << mod << " interval:" << interval);


  MCE_DEBUG("@@@@ HeavyFeed:" << sizeof(HeavyFeed) << " object:" << sizeof(Ice::Object)
  		<< " SharedFeed:" << sizeof(SharedFeed) << " SharedFeedPtr:" << sizeof(SharedFeedPtr)
  );
  TaskManager::instance().scheduleRepeated(&StatTimer::instance());
 // LogStatsTask::InitLogPath();      //initialize path of log in hadoop
}

FeedItemGuideManagerI::FeedItemGuideManagerI() {
	white_list_.insert(-1);
}

FeedItemGuideManagerI::~FeedItemGuideManagerI() {
}

void FeedItemGuideManagerI::EraseUser(int uid, const Ice::Current& cur) {
	if(uid < 0){
		MCE_WARN("FeedItemGuideManagerI::EraseUser. uid:" << uid << " uid less than 0 " << GetIp(cur));
		return;
	}
	bool status = UserInfoPool::instance().Erase(uid);
	MCE_INFO("FeedItemGuideManagerI::EraseUser. uid:" << uid << " in mem:" << status << " " << GetIp(cur));
}

void FeedItemGuideManagerI::CheckLoad(int uid, const vector<Ice::Long> & fids, FeedContentDict & dict) {
  vector<Ice::Long> miss_fids;
  BOOST_FOREACH(long fid, fids){
  	if(!dict.count(fid)) miss_fids.push_back(fid);
  }
  if(miss_fids.empty()) return;

  if (!dict.empty()) {//TODO 可加条件
    try {
      FeedMemcLoaderAdapter::instance().LoadFeedSeq(miss_fids);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedItemGuideManagerI::CheckLoad --> LoadFeedSeq uid:" << uid << " size:" << miss_fids.size() << " err, " << e);
      return;
    }
    MCE_INFO("FeedItemGuideManagerI::CheckLoad --> LoadFeedSeq uid:" << uid << " size:" << miss_fids.size());
  }else{
    FeedContentDict dict_loader;
    try {
      dict_loader= FeedMemcLoaderAdapter::instance().GetFeedDict(miss_fids, 100);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedItemGuideManagerI::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " err, " << e);
      return;
    }
    MCE_INFO("FeedItemGuideManagerI::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " res:" << dict_loader.size());
    dict.insert(dict_loader.begin(),dict_loader.end());
  }
}

pair<float,float> FeedItemGuideManagerI::GetFeedContentDict(GetRequest & req,FeedContentDict & fdict){
  set<long> & fidset = req.GetShowedFids();;
  vector<long> fids(fidset.begin(),fidset.end());

  TimeStat ts;
	try {
    fdict = FeedContentAdapter::instance().getFeedDict(fids);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedItemGuideManagerI::GetFeedContentDict. get memcached fail. uid:" << req.uid_ << " err:" << e);
  }
  float memcost = ts.Get();

  bool load_db = false;
  if(fdict.size() < fids.size()){
    CheckLoad(req.uid_,fids,fdict);
    load_db = true;
  }
  float dbcost = ts.Get();

  return make_pair(memcost,dbcost);
}

FeedDataResPtr FeedItemGuideManagerI::GetGuideFeedData(int uid, int begin, int limit,
    const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
	return GetFeedDataImpl(uid, IntSeq(), true, begin, limit, true);
}

FeedDataResPtr FeedItemGuideManagerI::GetGuideFeedDataByStype(int uid,
    const IntSeq& stypes, int begin, int limit, const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " stype_size:" << stypes.size() << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
	return GetFeedDataImpl(uid, stypes, true, begin, limit, true);
}


UserInfoPtr FeedItemGuideManagerI::LocateUserInfo(int uid, int begin) {
	UserInfoPtr uinfo = UserInfoPool::instance().Find(uid);
	if (uinfo) {
    time_t seconds = time(NULL);
		struct tm * local = localtime(&seconds);
		int now_date =  local->tm_mday;

    if (now_date != uinfo->GetExpireDate()) {
      UserInfoPool::instance().Erase(uid);
		  MCE_INFO("FeedItemGuideManagerI::LocateUserInfo. --> user in mem but expired ,cleanup guide_hbuf_. uid:" << uid << " info. " << uinfo->ToString());
    } else {
      if (begin == 0 ) {
		    MCE_INFO("FeedItemGuideManagerI::LocateUserInfo. --> user in mem but begin == 0 , fresh_up hbuf_. uid:" << uid);
        if ( !uinfo->LoadFeedCache() ) {
		      MCE_WARN("FeedItemGuideManagerI::LocateUserInfo.  --> user in mem, uid:" << uid << " load cache fail");
		      return 0;
 	      }
      }
		  MCE_INFO("FeedItemGuideManagerI::LocateUserInfo. --> user in mem. uid:" << uid << " info. " << uinfo->ToString());
		  return uinfo;
    }
	}

	MCE_INFO("FeedItemGuideManagerI::LocateUserInfo.  --> user not in mem. uid:" << uid);
 	uinfo = new UserInfo(uid);
  uinfo->LoadGuideFeedCache();
  if( !uinfo->LoadFeedCache() ) {
		MCE_WARN("FeedItemGuideManagerI::LocateUserInfo.  --> uid:" << uid << " load cache fail");
		return 0;
 	}

 	UserInfoPool::instance().Put(uid, uinfo);

	uinfo = UserInfoPool::instance().Find(uid);
	if (!uinfo) {
		MCE_WARN("FeedItemGuideManagerI::LocateUserInfo.  --> uid:" << uid << " locate fail");
	}
	return uinfo;
}

UserInfoPtr FeedItemGuideManagerI::FindUserInfo(int uid) {
	return UserInfoPool::instance().Find(uid);
}

//-----------------
class GetCost{
public:
	float loadtime, mergetime, buildtime, totaltime, contenttime, othertime;
	string ToString(){
		ostringstream os;
		os << " loadtime:" << loadtime
				<< " mergetime:" << mergetime
				<< " buildtime:" << buildtime
				<< " totaltime:" << loadtime+mergetime+buildtime+contenttime+othertime
				<< " contenttime:" << contenttime
				<< " othertime:" << othertime
				;
		return os.str();
	}
};
//-----------------

bool FeedItemGuideManagerI::CheckArgs(int uid, const IntSeq& types,
    bool stype, int begin, int limit, bool hot) {
	if (uid < 0 || begin < 0 || limit <= 0) {
		MCE_WARN("FeedItemGuideManagerI::CheckArgs.  --> arguments invalid. uid:" << uid
				<< " types:" << PrintHelper::Print(types) << " begin:" << begin
				<< " limit:" << limit << " hot:" << hot);
		return false;
	}
	return true;
}

set<long> GetNullFids(FeedContentDict & fdict){
	set<long> fids;
	BOOST_FOREACH(FeedContentDict::value_type & vt, fdict) {
		if (!(vt.second)) {
			fids.insert(vt.first);
		}
	}
	return fids;
}


FeedDataResPtr FeedItemGuideManagerI::GetFeedDataImpl(int uid, const IntSeq& types,
      bool stype, int begin, int limit, bool hot) {
  FeedDataResPtr res = new FeedDataRes;

  try{
    TimeStat ts; GetCost tc;

    GetRequest req(uid, types, stype, begin, limit, hot);
    if(!req.CheckArgs()) return res;

    UserInfoPtr uinfo = LocateUserInfo(uid, begin);
    if(!uinfo) return res;
    tc.loadtime = ts.Get();

    uinfo->MergeAndSort(req);
    tc.mergetime = ts.Get();

    FeedContentDict fdict;
    pair<float,float> p = GetFeedContentDict(req, fdict);
    tc.contenttime = ts.Get();

    FeedBuilder builder(uid, fdict, req.showed_comment_map_, req.sort_);
    res = builder.BuildFeedData(req);

    //以下为收尾工作
    tc.buildtime = ts.Get();

    uinfo->SetReadByIdsExactly(builder.GetNullFids());
    tc.othertime = ts.Get();

    MCE_INFO("FeedItemGuideManagerI::GetFeedDataImpl --> uid:" << uid
        << " begin:"	<< begin << " limit:" << limit
        << " typesize:" << types.size() << " bystype:" << stype << " hot:" << hot
        << " feedsize:" << uinfo->Size()
        << " resultsize:" << res->data.size()
        << tc.ToString()
        << " fdict:" << fdict.size()
        << " nullfids:" << builder.GetNullFids().size()
        << " show_fids:" << req.GetShowedFids().size()
        << " useless_count:" << req.GetUselessCount()
        << " types:" << PrintHelper::Print(types)
        );

    return res;
  } catch(Ice::Exception& e) {
    MCE_WARN("FeedItemGuideManagerI::GetFeedDataImpl uid:" << uid <<" error:" << e);
  } catch(...) {
    MCE_WARN("FeedItemGuideManagerI::GetFeedDataImpl uid:" << uid << " UnKnow error");
  }
  return res;
}

void FeedItemGuideManagerI::AddFeed(const FeedItem& fitem, const FeedConfigPtr& config, 
    const MyUtil::Int2IntMap& id2weight, const Ice::Current& cur) {

	SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);

	vector<int> uids, uids_mem;
	BOOST_FOREACH(const MyUtil::Int2IntMap::value_type & vt, id2weight) {
		int uid = vt.first;
		UserInfoPtr uinfo = FindUserInfo(uid);
		if (uinfo) {
			HeavyFeed hfeed(fitem, hdl);
			hfeed.SetUpdateTime(config->updateTime);
			hfeed.SetWeight(vt.second);
			uinfo->AddHeavyItem(uid, hfeed);
			uids_mem.push_back(uid);
		}
		if (uid == 220678672 || uid == 347348947	|| uid == 263734453) {
			MCE_INFO("FeedItemManager::AddFeed. -->  target:" << uid
					<< " fid:" << fitem.feed  
          << " actor:" << fitem.actor
					<< " weight:" << vt.second);
		}
		uids.push_back(uid);
	}


	MCE_INFO("FeedItemManager::AddFeed.  --> fid:" << fitem.feed << " stype:" << hdl->GetStype()
			<< " actor:" << fitem.actor	
			<< " time:" << fitem.time 
			<< " mg:" << fitem.merge
			<< " uids_size:" << uids.size()
			<< " uids:" << PrintHelper::Print(uids,3)
			<< " uids_mem size:" << uids_mem.size()
			<< " uids_mem:" << PrintHelper::Print(uids_mem,3));
}


void FeedItemGuideManagerI::SetRead(int uid, int stype, Ice::Long merge, const Ice::Current&) {
	UserInfoPtr uinfo = FindUserInfo(uid);
	vector<long> fids;
	if (uinfo) {
		fids = uinfo->SetRead(stype, merge);
	}
	MCE_INFO("FeedItemGuideManagerI::SetRead. uid:" << uid << " stype:" << stype
			<< " merge:" << merge << " in mem:" << boolalpha <<(uinfo != 0)
			<< " removed fids:" << PrintHelper::Print(fids));
}

void FeedItemGuideManagerI::SetReadById(int uid, Ice::Long fid, const Ice::Current& cur) {
	UserInfoPtr uinfo = FindUserInfo(uid);
	vector<long> fids;
	if (uinfo) {
		fids = uinfo->SetReadById(fid);
	}
	MCE_INFO("FeedItemGuideManagerI::SetReadId. uid:" << uid << " fid:" << fid
			<< " in mem:" << boolalpha <<(uinfo != 0)
			<< " removed fids:" << PrintHelper::Print(fids)
			<< " " << GetIp(cur)
	);
}

void FeedItemGuideManagerI::SetReadAll(int uid, const Ice::Current& cur) {
	UserInfoPtr uinfo = FindUserInfo(uid);
	if (uinfo) {
		uinfo->SetReadAll();
	}
	  MCE_INFO("FeedItemGuideManagerI::SetReadAll. uid:" << uid
			  << " in mem:" << (uinfo != 0)
				<< " " << GetIp(cur)
	  ) ;
}

FeedDataSeq FeedItemGuideManagerI::GetFeedDataByIds(const MyUtil::LongSeq& fids,
    const Ice::Current& cur) {
  FeedContentDict dict;
  try {
    dict = FeedContentAdapter::instance().getFeedDict(fids);
    CheckLoad(0, fids, dict);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedItemGuideManagerI::getFeedDataByIds --> " << e);
  }

  FeedDataSeq seq;
  for (FeedContentDict::iterator it = dict.begin(); it != dict.end(); ++it) {
    if (it->second) {
      if (it->second->reply) { //如果有回复 把回复的内容加入到xml中
        ostringstream xml;
        xml << it->second->data->xml;
        xml << it->second->reply->oldReply << it->second->reply->newReply
            << "<reply_count>" << it->second->reply->count << "</reply_count>";
        it->second->data->xml = xml.str();
      }
      it->second->data->weight = 0;
      seq.push_back(it->second->data);
    }
  }

  MCE_INFO("FeedItemGuideManagerI::GetFeedDataByIds --> size:" << fids.size() << " fids:"
  		<< " fids:" << PrintHelper::Print(fids) << " res size:" << seq.size()
			<< " " << GetIp(cur)
  );
  return seq;
}

FeedDataSeq FeedItemGuideManagerI::GetFeedDataByIdsWithUid(int uid,
    const MyUtil::LongSeq& fids, Ice::Long first, int stype,
    const Ice::Current&) {
	MCE_WARN("FeedItemGuideManagerI::GetFeedDataByIdsWithUid. uid:" << uid 
      << " fids:" << PrintHelper::Print(fids)
	    << " first:" << first << " stype:" << stype );
	return GetFeedDataByIds(fids);
}

FeedItemSeq FeedItemGuideManagerI::GetOriginalFeedData(int uid, const Ice::Current&) {
	TimeStat ts;
	UserInfoPtr uinfo = LocateUserInfo(uid);
	float locatec = ts.Get();

	FeedItemSeq res = uinfo->GetOrigFeedItems();
	float get = ts.Get();

	MCE_INFO("FeedItemGuideManagerI::GetOriginalFeedData. uid:" << uid << " size:" << res.size()
			<< " locate:" << locatec  << " get:" << get);
	return res;
}

int FeedItemGuideManagerI::GetUnviewedCount(int uid, int type, const Ice::Current& cur) {
	return 0;
}

map<int,int>	FeedItemGuideManagerI::GetCountMapByStype(int uid, const vector<int>& stypes, bool unviewed, const Ice::Current& cur ){
	return map<int,int>();
}

FeedItemSeqSeq FeedItemGuideManagerI::GetFeedItemSeqSeqByUsers(const IntSeq& uids, const Ice::Current&) {
	MCE_INFO("FeedItemGuideManagerI::GetFeedItemSeqSeqByUsers. uids:" << PrintHelper::Print(uids));
  return FeedItemSeqSeq();
}


void FeedItemGuideManagerI::load(int uid, const Ice::Current& cur) {
//	MCE_INFO("FeedItemGuideManagerI::load. input: uid:" << uid);
	static int level = 0;
	if(uid > -100 && uid <= 0 ){
		MCE_INFO("FeedItemGuideManagerI::load.old level:" << level << " new level:" << -uid );
		level = -uid;
		return;
	}

	if(uid /100 % 10 < level){
		MCE_INFO("FeedItemGuideManagerI::load --> " << uid << " level:" << level  );
		GetGuideFeedData(uid, 0, 20);
	}
}

void FeedItemGuideManagerI::AddWhiteList(int uid, const Ice::Current& current) {
	MCE_INFO("FeedItemGuideManagerI::AddWhiteList, uid: " << uid);
	IceUtil::RWRecMutex::WLock lock(white_list_mutex_);
	white_list_.insert(uid);
}

void FeedItemGuideManagerI::DelWhiteList(int uid, const Ice::Current& current) {
	MCE_INFO("FeedItemGuideManagerI::DelWhiteList, uid: " << uid);
	IceUtil::RWRecMutex::WLock lock(white_list_mutex_);
	white_list_.erase(uid);
}

MyUtil::IntSeq FeedItemGuideManagerI::GetWhiteList(const Ice::Current& current) {
	MCE_INFO("FeedItemGuideManagerI::GetWhiteList");
	IceUtil::RWRecMutex::RLock lock(white_list_mutex_);
	MyUtil::IntSeq result(white_list_.begin(),white_list_.end());
	return result;
}

void FeedItemGuideManagerI::ClearWhiteList(const Ice::Current& current) {
	MCE_INFO("FeedItemGuideManagerI::ClearWhiteList");
	IceUtil::RWRecMutex::WLock lock(white_list_mutex_);
	white_list_.clear();
}

bool FeedItemGuideManagerI::InWhiteList(int uid) {
	IceUtil::RWRecMutex::RLock lock(white_list_mutex_);
	bool result = white_list_.empty() || white_list_.find(uid) != white_list_.end() ? true : false; 
	if (!white_list_.empty()) {
		MCE_INFO("FeedItemGuideManagerI::InWhiteList, uid: " << uid << " result: " << result);
	}
	return result;
}

void StatTimer::handle(){
		MCE_INFO("@@@@ StatTimer." << UserInfoPool::instance().ToString()
				<< " " << SharedFeedCache::instance().ToString());
}

