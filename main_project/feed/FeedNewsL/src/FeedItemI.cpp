
#include "FeedItemI.h"
#include "FeedBuilder.h"
#include "client/linux/handler/exception_handler.h"
#include "FeedItemI.h"
#include "FeedCacheAdapter.h"
#include "FeedContentAdapter.h"
#include "IceLogger.h"
#include "ServiceI.h"
#include "PerformanceStat.h"
#include "FeedMemcLoaderAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "feed/ReplicaCluster/ServerState/src/client/cxx/FeedServerStateSubscriber.h"
#include "XceLoggerAdapter.h"
#include <boost/foreach.hpp>
#include "SharedFeedCache.h"
#include "EDMFeed.h"
#include "util/cpp/InvokeMonitor.h"

//by jin.zhang, 2012.03.01
#include "FeedItemWrapperReplicaAdapter.h"
#include "LuaInterface.h"
#include "PropertiesManager.h"
#include "ExperimentManager.h"

using namespace xce::feed;

//bool MyCallback(const char *dump_path, const char *minidump_id, void *context,
//    bool succeeded) {
//  printf("got crash %s %s\n", dump_path, minidump_id);
//  return true;
//}

void MyUtil::initialize() {
//  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedItemManagerI::instance(), service.createIdentity("FIM", ""));
  //controller
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "FeedNews.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault( "FeedNews.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerFeedNewsR", &FeedItemManagerI::instance(), mod, interval,
      new XceFeedChannel());
  xce::feed::serverstate::FeedServerStateSubscriber::instance().initialize( "ControllerFeedNewsR", &FeedItemManagerI::instance(), mod, interval, new XceFeedControllerChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

  MCE_INFO("@@@@ HeavyFeed:" << sizeof(HeavyFeed) << " object:" << sizeof(Ice::Object)
  		<< " SharedFeed:" << sizeof(SharedFeed) << " SharedFeedPtr:" << sizeof(SharedFeedPtr)
  );
  PropertiesManager::instance().Init();
  ExperimentManager::instance().Init(mod);
  ExperimentManager::instance().LoadExperiment(-1);
  TypeFilter::instance().Init();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  int index = props->getPropertyAsIntWithDefault("Service.ServerIndex", 0);
  if(index == 50) {
  	index = 12;//TODO 改成index
  }
  EDMFeedManager::instance().setIndex(index);
  TaskManager::instance().execute(new EdmLoadTask());
//  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
//  int index = props->getPropertyAsIntWithDefault("Service.ServerIndex", 0);
//  TaskManager::instance().scheduleRepeated(&PerformanceStat::instance());
//  TaskManager::instance().execute(new LoadTask());
//  TaskManager::instance().scheduleRepeated(&ShiftStatTimer::instance());
  TaskManager::instance().scheduleRepeated(&StatTimer::instance());
  TaskManager::instance().schedule(&MaxFidUploader::instance());
	GetLogger::instance().Init();

}

FeedItemManagerI::FeedItemManagerI() {
  // 设置db源
  DegradeManager::instance().SetDBSource("feed_degrade");

  // 注册降级模块 
  DegradeManager::instance().Register("FeedNews_LoaderGetDict");
  DegradeManager::instance().Register("FeedNews_IsSort");
  DegradeManager::instance().Register("FeedNews_PullMode");
  DegradeManager::instance().Register("FeedNews_LuaScore");

  // 然后初始化
  DegradeManager::instance().Init();

}

FeedItemManagerI::~FeedItemManagerI() {
}

void FeedItemManagerI::EraseUser(int uid, const Ice::Current& cur) {
	if(uid < 0){
		MCE_WARN("FeedItemManagerI::EraseUser. uid:" << uid << " uid less than 0 " << GetIp(cur));
		return;
	}

	bool status = UserInfoPool::instance().Erase(uid);
	MCE_INFO("FeedItemManagerI::EraseUser. uid:" << uid << " in mem:" << status << " " << GetIp(cur));
}

void FeedItemManagerI::CheckLoad(GetRequest & req, const vector<Ice::Long> & fids, FeedContentDict & dict) {
  int uid = req.uid_;
  vector<Ice::Long> miss_fids;
  BOOST_FOREACH(long fid, fids){
  	if(!dict.count(fid)) miss_fids.push_back(fid);
  }
  if(miss_fids.empty()) return;

  if (!dict.empty()) {//TODO 可加条件
    try {
      FeedMemcLoaderAdapter::instance().LoadFeedSeq(miss_fids);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedItemManagerI::CheckLoad --> LoadFeedSeq uid:" << uid << " size:" << miss_fids.size() << " err, " << e);
      return;
    }
  }else{
    if (DegradeManager::instance().IsEnable("FeedNews_LoaderGetDict")) {
      FeedContentDict dict_loader;
      try {
        dict_loader= FeedMemcLoaderAdapter::instance().GetFeedDict(miss_fids, 100);
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemManagerI::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " err, " << e);
        return;
      }
      req.logmap_["CheckLoad_DictLoader"] = dict_loader.size();
      dict.insert(dict_loader.begin(),dict_loader.end());
    }
  }
  req.logmap_["CheckLoad_MissFids"] = miss_fids.size();
}

pair<float,float> FeedItemManagerI::GetFeedContentDict(GetRequest & req,FeedContentDict & fdict){
  set<long> & fidset = req.GetShowedFids();;
  vector<long> fids(fidset.begin(),fidset.end());

  TimeStat ts;
	try {
    fdict = FeedContentAdapter::instance().getFeedDict(fids);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedItemManagerI::GetFeedContentDict. get memcached fail. uid:" << req.uid_ << " err:" << e);
  }
  //zj//int memcnt = fdict.size();
  float memcost = ts.Get();

  bool load_db = false;
  if(fdict.size() < fids.size()){
    CheckLoad(req,fids,fdict);
    load_db = true;
  }
  //zj//int dbcnt = fdict.size() - memcnt;
  float dbcost = ts.Get();

  return make_pair(memcost,dbcost);
}

FeedDataResPtr FeedItemManagerI::GetLiveFeedData(int uid, int begin, int limit,
    const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  return GetFeedDataImpl(uid, IntSeq(), true, begin, limit, false);
}

FeedDataResPtr FeedItemManagerI::GetLiveFeedDataByStype(int uid,
    const IntSeq& stypes, int begin, int limit, const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  return GetFeedDataImpl(uid, stypes, true, begin, limit, false);
}

FeedDataResPtr FeedItemManagerI::GetLiveFeedDataByType(int uid,
    const IntSeq& types, int begin, int limit, const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
	return GetFeedDataImpl(uid, types, false, begin, limit, false);
}

FeedDataResPtr FeedItemManagerI::GetHotFeedData(int uid, int begin, int limit,
    const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

		return GetFeedDataImpl(uid, IntSeq(), true, begin, limit, true); 
}

FeedDataResPtr FeedItemManagerI::GetHotFeedDataByStype(int uid,
    const IntSeq& stypes, int begin, int limit, const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  return GetFeedDataImpl(uid, stypes, true, begin, limit, true);
}

FeedDataResPtr FeedItemManagerI::GetHotFeedDataByType(int uid,
    const IntSeq& types, int begin, int limit, const Ice::Current& current) {
	ostringstream oss;
	oss << " uid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  return GetFeedDataImpl(uid, types, false, begin, limit, true);
}

FeedDataResPtr FeedItemManagerI::GetFriendOriginal(int uid, const IntSeq & friends, int begin, int limit, const Ice::Current& current) {
  static const int FRIENDORIGTPYE[] = {601,701,709, 8200};
	limit &= 0xff;
	IntSeq stypes;
	int type_size = sizeof(FRIENDORIGTPYE)/sizeof(FRIENDORIGTPYE[0]);
	for (int i = 0; i < type_size; i++) {
		stypes.push_back(FRIENDORIGTPYE[i]);
	}
	ostringstream oss;
	oss << "userid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	FeedMetaSeq feedmetas;
	LongSeq feedids;
	FeedDataResPtr res = new FeedDataRes;

	try {
    if (friends.empty()) {
      feedmetas	= FeedItemWrapperReplicaAdapter::instance().GetFeeds(uid, begin, limit, stypes);
    } else {
      feedmetas = FeedItemWrapperReplicaAdapter::instance().getByTypesWithFriends(uid, friends, begin, limit, stypes);
    }
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemManagerI::GetFriendOriginal --> FeedItemWrapper getByTypes uid:" << uid << " begin: " << begin << " limit: " << limit << "friends size: " << friends.size() << " err: " << e);
	}

	for (FeedMetaSeq::iterator it = feedmetas.begin(); it != feedmetas.end(); ++it) {
		if (it->feedid > 0) {
			feedids.push_back(it->feedid);
		}
	}

	if (!feedids.size()) {
		return res;
	}

	MCE_INFO("FeedItemManagerI::GetFriendOriginal --> FeedItemWrapper getByTypes Result uid:" << uid << " begin: " << begin << " limit: " << limit << " feedids: " << PrintHelper::Print(feedids));

	FeedContentDict fdict;
	try {
		fdict = FeedContentAdapter::instance().getFeedDict(feedids);
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemManagerI::GetFriendOriginal --> FeedContentAdapter getFeedDict uid: " << uid << " begin: " << begin << " limit: " << limit << " err: " << e);
	}
	if(fdict.size() < feedids.size()){
		vector<Ice::Long> miss_fids;

		BOOST_FOREACH(long fid, feedids){
			if (!fdict.count(fid)) {
				miss_fids.push_back(fid);
			}
		}

    FeedContentDict dict_loader;
    try {
      dict_loader = FeedMemcLoaderAdapter::instance().GetFeedDict(miss_fids, 100);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedItemManagerI::GetFriendOriginal --> FeedMemcLoader GetFeedDict uid: " << uid << " size: " << miss_fids.size() << " err: " << e);
    }
    MCE_INFO("FeedItemManagerI::GetFriendOriginal --> FeedMemcLoader GetFeedDict uid: " << uid << "miss feedids: " << PrintHelper::Print(miss_fids) << " res: " << dict_loader.size());
    fdict.insert(dict_loader.begin(),dict_loader.end());
  }

	res->UnviewedCount = feedids.size();
  for (LongSeq::iterator it = feedids.begin(); it != feedids.end(); ++it) {
    FeedContentDict::const_iterator dict_it = fdict.find(*it);
    if (dict_it == fdict.end()) {
      continue;
    }
    FeedContentPtr content = dict_it->second;
 		if (content) {
      if (content->reply) {
        ostringstream xml;
        xml << content->data->xml;
        xml << content->reply->oldReply << content->reply->newReply
            << "<reply_count>" << content->reply->count << "</reply_count>";
        content->data->xml = xml.str();
      }
			content->data->weight = 0;
			res->data.push_back(content->data);
		}
  }

	return res;
}

FeedDataResPtr	FeedItemManagerI::GetFeedDataByGroupName(int uid, const string & groupname, int begin, int limit, const Ice::Current& current) {
	FeedDataResPtr res = new FeedDataRes;

	ostringstream oss;
	oss << "userid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	FeedMetaSeq feedmetas;
	LongSeq feedids;

	try {
		feedmetas	= FeedItemWrapperReplicaAdapter::instance().GetFeedsByGroupName(uid,groupname, begin, limit, IntSeq());
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemManagerI::GetFeedDataByGroupName --> FeedItemWrapper getByTypes uid:" << uid << " begin: " << begin << " limit: " << limit << " err: " << e);
	}

	for (FeedMetaSeq::iterator it = feedmetas.begin(); it != feedmetas.end(); ++it) {
		if (it->feedid > 0) {
			feedids.push_back(it->feedid);
		}
	}

	if (!feedids.size()) {
		return res;
	}
	MCE_INFO("FeedItemManagerI::GetFeedDataByGroupName --> FeedItemWrapper GetFeedsByGroupName Result uid:" << uid << " begin: " << begin << " limit: " << limit << " feedids: " << PrintHelper::Print(feedids));

	try {
		FeedDataSeq seq = GetFeedDataByIds(feedids);
		res->data.reserve(seq.size());

		for(FeedDataSeq::reverse_iterator it = seq.rbegin();
				it != seq.rend(); ++it) {
			res->data.push_back(*it);
		}

		res->UnviewedCount = res->data.size();

		MCE_INFO("FeedItemManagerI::GetFeedDataByGroupName --> uid: " << uid
				<< ", begin: " << begin << ", limit: " << limit
				<< ", feed data size: " << res->data.size());

	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemManagerI::GetFeedDataByGroupName uid: " << uid
				<< "err: " << e);
	}
	return res;
}

FeedDataResPtr	FeedItemManagerI::GetFeedDataByGroupId(int uid, int groupid, int begin, int limit, const Ice::Current& current) {
	FeedDataResPtr res = new FeedDataRes;

	ostringstream oss;
	oss << "userid: " << uid << " begin: " << begin << " limit: " << limit;
	InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	FeedMetaSeq feedmetas;
	LongSeq feedids;

	try {
		feedmetas	= FeedItemWrapperReplicaAdapter::instance().GetFeedsByGroupId(uid,groupid, begin, limit, IntSeq());
	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemManagerI::GetFeedDataByGroupId --> FeedItemWrapper getByTypes uid:" << uid << " begin: " << begin << " limit: " << limit << " err: " << e);
	}

	for (FeedMetaSeq::iterator it = feedmetas.begin(); it != feedmetas.end(); ++it) {
		if (it->feedid > 0) {
			feedids.push_back(it->feedid);
		}
	}

	if (!feedids.size()) {
		return res;
	}
	MCE_INFO("FeedItemManagerI::GetFeedDataByGroupId --> FeedItemWrapper GetFeedsByGroupId Result uid:" << uid << " begin: " << begin << " limit: " << limit << " feedids: " << PrintHelper::Print(feedids));

	try {
		FeedDataSeq seq = GetFeedDataByIds(feedids);
		res->data.reserve(seq.size());

		for(FeedDataSeq::reverse_iterator it = seq.rbegin();
				it != seq.rend(); ++it) {
			res->data.push_back(*it);
		}

		res->UnviewedCount = res->data.size();

		MCE_INFO("FeedItemManagerI::GetFeedDataByGroupId --> uid: " << uid
				<< ", begin: " << begin << ", limit: " << limit
				<< ", feed data size: " << res->data.size());

	} catch (Ice::Exception& e) {
		MCE_WARN("FeedItemManagerI::GetFeedDataByGroupId uid: " << uid
				<< "err: " << e);
	}
	return res;
}

UserInfoPtr FeedItemManagerI::LocateUserInfo(int uid, GetRequest & req) {
	UserInfoPtr uinfo = UserInfoPool::instance().Find(uid);

	if(uinfo) {
    req.logmap_["isinmem"] = 1;
		return uinfo;
	}
  req.logmap_["isinmem"] = 0;
 	uinfo = new UserInfo(uid);
 	if(!(uinfo->Init())){
		MCE_WARN("FeedItemManagerI::LocateUserInfo. uid:" << uid << " init fail");
		return 0;
 	}

 	UserInfoPool::instance().Put(uid,uinfo);

	uinfo = UserInfoPool::instance().Find(uid);
	if(!uinfo){
		MCE_WARN("FeedItemManagerI::LocateUserInfo. uid:" << uid << " locate fail");
	}
	return uinfo;
}

UserInfoPtr FeedItemManagerI::FindUserInfo(int uid){
	return UserInfoPool::instance().Find(uid);
}

class GetCost{
public:
	float loadtime,mergetime,buildtime,totaltime,contenttime,othertime;
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

bool FeedItemManagerI::CheckArgs(int uid, const IntSeq& types,
    bool stype, int begin, int limit, bool hot){
	if(uid < 0 || begin < 0 || limit <= 0){
		MCE_WARN("FeedItemManagerI::CheckArgs. arguments invalid. uid:" << uid
				<< " types:" << PrintHelper::Print(types) << " begin:" << begin
				<< " limit:" << limit << " hot:" << hot);
		return false;
	}
	return true;
}

set<long> GetNullFids(FeedContentDict & fdict){
	set<long> fids;
	BOOST_FOREACH(FeedContentDict::value_type & vt, fdict){
		if(!(vt.second)){
			fids.insert(vt.first);
		}
	}
	return fids;
}

static int MakeUnviewCount(int count){
	int flag = count > 0?1:0;
	return flag << 31|count;
}
static int GetOlderCount(int UnviewCount){
	return UnviewCount & 0x7FFFFFFF;
}

static bool HasOlder(int UnviewCount){
	return UnviewCount & 0x80000000;
}

//当begin+limit>=250时,从FeedItemCacheByUserid拉feed,过滤types, users
//添加2个函数:GetFeedidsFriends, GetFeedDataFromFriends
//by jin.zhang, 2012.03.01
LongSeq FeedItemManagerI::GetFeedidsFromFriends(int uid, int begin, int limit, const IntSeq& types) {
    LongSeq feedids;
    try {

        GetRequest req;
        UserInfoPtr uinfo = LocateUserInfo(uid, req);
        if(!uinfo) return feedids;
        Ice::Long minfid = uinfo->GetMinRead();

		try {
			if (minfid == FEEDID_INF) {
				FeedMetaSeq feedmetas = FeedItemWrapperReplicaAdapter::instance().get(uid, 251, 1);//取出250条后面信息，作为最小feedid
				if (!feedmetas.empty()) {
					for (FeedMetaSeq::iterator it = feedmetas.begin(); it != feedmetas.end(); ++it) {
						minfid = std::min(minfid, it->feedid);
					}
				} else {
					MCE_INFO("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid << " minfid: " << minfid << " 251 is empty!");
					return LongSeq();
				}
			}
		} catch (Ice::Exception& e) {
			MCE_WARN("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid << " minfid: " << minfid << " 251!" << e.what());
			return LongSeq();
		} catch (...) {
			MCE_WARN("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid << " minfid: " << minfid << " 251! UNKNOWN EXCEPTION!");
			return LongSeq();
		}

		FeedMetaSeq feedmetas = FeedItemWrapperReplicaAdapter::instance().getLessThanFeedidByTypes(uid, minfid, limit, types);

        for (FeedMetaSeq::iterator it = feedmetas.begin(); it != feedmetas.end(); ++it) {
            if (it->feedid > 0) {
                feedids.push_back(it->feedid);
				minfid = std::min(minfid, it->feedid);
                //MCE_DEBUG("userid: " << uid << " feedid: " << it->feedid);
            }
        }

        if (feedids.size() > 0) {
            uinfo->SetMinRead(minfid, true);
            MinFidUploader::instance().Add(uid, uinfo->GetMinRead());
        }

        MCE_DEBUG("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid
          << ", begin: " << begin
          << ", limit: " << limit
          << ", feedids size: " << feedids.size()
          << ", minReadFeedid: " << minfid);

    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid
                << "err: " << e);
    } catch (...) {
        MCE_WARN("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid
                << " UNKOWN EXCEPTION!");
    }
    return feedids;
}
FeedDataResPtr FeedItemManagerI::GetFeedDataFromFriends(int uid,  int begin, int limit, const IntSeq& types) {
    FeedDataResPtr res = new FeedDataRes;
    if (!res) {
        MCE_WARN("FeedItemManagerI::GetFeedDataFromFriends uid: " << uid
                << ", operator 'new' error." );
        return NULL;
    }
    try {
        LongSeq feedids = GetFeedidsFromFriends(uid, begin, limit, types);

		FeedDataSeq seq = GetFeedDataByIds(feedids);
		res->data.reserve(seq.size());

		for(FeedDataSeq::reverse_iterator it = seq.rbegin();
				it != seq.rend(); ++it) {
        	res->data.push_back(*it);
		}

        res->UnviewedCount = res->data.size();

        MCE_INFO("FeedItemManagerI::GetFeedDataFromFriends --> uid: " << uid
                << ", begin: " << begin << ", limit: " << limit
                << ", feed data size: " << res->data.size());

        return res;
    } catch (Ice::Exception& e) {
        MCE_WARN("FeedItemManagerI::GetFeedDataLessThanFeedid uid: " << uid
                << "err: " << e);
        return res;
    }
}

FeedDataResPtr FeedItemManagerI::GetFeedDataImpl(int uid, const IntSeq& types,
    bool stype, int begin, int limit, bool hot) {
    //当超过250条时,从好友处拉新鲜事.
    //by jin.zhang, 2012.03.01
        //return GetFeedDataFromFriends(uid, begin, limit);

	FeedDataResPtr res = new FeedDataRes;

	try{
	    TimeStat ts;
        double temptime;
    if (!DegradeManager::instance().IsEnable("FeedNews_IsSort")) {
      hot = false;
    }
        GetRequest req(uid,types,stype,begin,limit,hot);

        if(!req.CheckArgs()) return res;

        req.logmap_["totaltime"] = 0;
        UserInfoPtr uinfo = LocateUserInfo(uid, req);
        if(!uinfo) return res;
        //	long pre_max_read = uinfo->GetMaxRead();
        int recent_cnt = uinfo->GetRecentCnt();
        temptime = ts.Get();
        req.logmap_["loadtime"] = temptime;
        req.logmap_["totaltime"] += temptime;

        uinfo->MergeAndSort(req);
        temptime = ts.Get();
        req.logmap_["mergetime"] = temptime;
        req.logmap_["totaltime"] += temptime;
        //PrintHelper::Print(req.hfeed3d_,uid);
        FeedContentDict fdict;
        pair<float,float> p = GetFeedContentDict(req,fdict);
        temptime = ts.Get();
        req.logmap_["contentmemcost"] = p.first;
        req.logmap_["contentdbcost"] = p.second;
        req.logmap_["contenttime"] = temptime;
        req.logmap_["totaltime"] += temptime;

        FeedBuilder builder(uid,fdict,req.showed_comment_map_,req.sort_);
        res = builder.BuildFeedData(req);

		//pull more , by guanghe.ge 2012.3.6
		if ((res->data.empty()) && DegradeManager::instance().IsEnable("FeedNews_PullMode")) {
        	return GetFeedDataFromFriends(uid, begin, limit, types);
		}

        //以下为收尾工作
        uinfo->SetRecentCnt(0);
        res->UnviewedCount = MakeUnviewCount(req.GetOlderCount());
        temptime = ts.Get();
        req.logmap_["buildtime"] = temptime;
        req.logmap_["totaltime"] += temptime;

        uinfo->SetReadByIdsExactly(builder.GetNullFids());
        req.ClearEdms();
        temptime = ts.Get();
        req.logmap_["othertime"] = temptime;
        req.logmap_["totaltime"] += temptime;

        //设置min feedid, 写memcache
        //by jin.zhang(张劲), 2012.02.24
        //uinfo->SetMinRead(req.GetMinFid());
        uinfo->SetMinRead(FEEDID_INF);
	    MinFidUploader::instance().Add(uid, uinfo->GetMinRead());

        bool maxchanged = uinfo->SetMaxRead(req.GetMaxFid());
        uinfo->SetRefreshTime(req.GetNowTime());

        MaxFidUploader::instance().Add(uid,uinfo->GetMaxRead(),uinfo->GetRefreshTime());


        MCE_INFO("FeedItemManagerI::GetFeedDataImpl --> uid:" << uid << " begin:" << begin << " limit:" << limit
                << " bystype:" << stype << " hot:" << hot << " feedsize:" << uinfo->Size()
                << " res:" << res->data.size()
                << " fdict:" << fdict.size()
                << " nullfids:" << builder.GetNullFids().size()
                << " hidesize:" << builder.hidesize_ //根据策略需求，增加统计replace换merge数量
                << " mergesize:" << builder.mergesize_ //根据策略需求，增加统计merge数量
                << " maxchanged:" << maxchanged
                << " maxreadfid:" << uinfo->GetMaxRead()
                << " minreadfid:" << uinfo->GetMinRead() //log输出MinFeedid, by jin.zhang(张劲), 2012.02.24
                << " refresht:" << uinfo->GetRefreshTime()
                << " older_count:" << GetOlderCount(res->UnviewedCount)
                << " has_older:" << HasOlder(res->UnviewedCount)
                << " show_fids:" << req.GetShowedFids().size()
                << " useless_count:" << req.GetUselessCount()
                << " types:" << PrintHelper::Print(types)
                << " exclude_types:" << PrintHelper::Print(req.GetExcludeTypes())
                << " recent_cnt:" << recent_cnt
                << req.PrintLog()
                );

        return res;

	} catch(Ice::Exception& e) {
		MCE_WARN(" FeedItemManagerI::GetFeedDataImpl uid:" << uid
				<<" error:" << e);
		return res;
	}
}

void FeedItemManagerI::AddFeed(const FeedItem& fitem,
    const FeedConfigPtr& config, const MyUtil::Int2IntMap& id2weight,
    const Ice::Current& cur) {
  ostringstream oss;
  oss << " feedid: " << fitem.feed << " actor: " << fitem.actor;
  InvokeClient tm = InvokeClient::create(cur, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);

	vector<int> uids, uids_mem;

	BOOST_FOREACH(const MyUtil::Int2IntMap::value_type & vt, id2weight){
		int uid = vt.first;
		UserInfoPtr uinfo = FindUserInfo(uid);
		if(uinfo){
			HeavyFeed hfeed(fitem,hdl);
			hfeed.SetWeight(vt.second);
			uinfo->AddHeavyItem(uid,hfeed);
			uids_mem.push_back(uid);
		}
		uids.push_back(uid);
	}

	EDMFeedManager::instance().addEDMFeed(fitem,uids);

	MCE_INFO("FeedItemManager::AddFeed. fid:" << fitem.feed << " stype:" << hdl->GetStype() 
			<< " actor:" << fitem.actor	
			<< " time:" << fitem.time 
			<< " mg:" << fitem.merge
			<< " uids_size:" << uids.size()
			<< " uids:" << PrintHelper::Print(uids,3)
			<< " uids_mem size:" << uids_mem.size()
			<< " uids_mem:" << PrintHelper::Print(uids_mem,3));
}


void FeedItemManagerI::SetRead(int uid, int stype, Ice::Long merge,
    const Ice::Current&) {
	UserInfoPtr uinfo = FindUserInfo(uid);
	vector<long> fids;
	if(uinfo) {
		fids = uinfo->SetRead(stype,merge);
	}
	MCE_INFO("FeedItemManagerI::SetRead. uid:" << uid << " stype:" << stype
			<< " merge:" << merge << " in mem:" << boolalpha <<(uinfo != 0)
			<< " removed fids:" << PrintHelper::Print(fids));
}

void FeedItemManagerI::SetReadById(int uid, Ice::Long fid,
    const Ice::Current& cur) {
	UserInfoPtr uinfo = FindUserInfo(uid);
	vector<long> fids;
	if(uinfo) {
		fids = uinfo->SetReadById(fid);
	}
	MCE_INFO("FeedItemManagerI::SetReadId. uid:" << uid << " fid:" << fid
			<< " in mem:" << boolalpha <<(uinfo != 0)
			<< " removed fids:" << PrintHelper::Print(fids)
			<< " " << GetIp(cur)
	);
}

void FeedItemManagerI::SetReadAll(int uid, const Ice::Current& cur) {
	UserInfoPtr uinfo = FindUserInfo(uid);
	if(uinfo) {
		uinfo->SetReadAll();
	}
  MCE_INFO("FeedItemManagerI::SetReadAll. uid:" << uid
  		<< " in mem:" << (uinfo != 0)
			<< " " << GetIp(cur)
  ) ;
}

FeedDataSeq FeedItemManagerI::GetFeedDataByIds(const MyUtil::LongSeq& fids,
    const Ice::Current& cur) {
  FeedContentDict dict;
  try {
    dict = FeedContentAdapter::instance().getFeedDict(fids);
    GetRequest req;
    req.uid_ = 0;
    CheckLoad(req,fids,dict);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedItemManagerI::getFeedDataByIds --> " << e);
  }

  FeedDataSeq seq;
  for (FeedContentDict::iterator it = dict.begin(); it != dict.end(); ++it) {
    if (it->second) {
      if (it->second->reply) {
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

  MCE_INFO("FeedItemManagerI::GetFeedDataByIds --> size:" << fids.size() << " fids:"
  		<< " fids:" << PrintHelper::Print(fids) << " res size:" << seq.size()
			<< " " << GetIp(cur)
  );
  return seq;
}

FeedDataSeq FeedItemManagerI::GetFeedDataByIdsWithUid(int uid,
    const MyUtil::LongSeq& fids, Ice::Long first, int stype,
    const Ice::Current&) {
	MCE_WARN("FeedItemManagerI::GetFeedDataByIdsWithUid. uid:" << uid << " fids:" << PrintHelper::Print(fids)
	<< " first:" << first << " stype:" << stype );
	return GetFeedDataByIds(fids);
}

FeedItemSeq FeedItemManagerI::GetOriginalFeedData(int uid, const Ice::Current&) {
	TimeStat ts;

  GetRequest req;
	UserInfoPtr uinfo = LocateUserInfo(uid, req);
	float locatec = ts.Get();

//	uinfo->LoadFeedCache();

	FeedItemSeq res = uinfo->GetOrigFeedItems();
	float get = ts.Get();

	MCE_INFO("FeedItemManagerI::GetOriginalFeedData. uid:" << uid << " size:" << res.size()
			<< " locate:" << locatec  << " get:" << get);
	return res;
}

int FeedItemManagerI::GetUnviewedCount(int uid, int type, const Ice::Current& cur) {
  GetRequest req;
	UserInfoPtr uinfo = LocateUserInfo(uid, req);
	if(!uinfo){
		MCE_INFO("FeedItemManagerI::GetUnviewedCount. uid:" << uid << " type:" << type << " locate uinfo fail " << GetIp(cur));
		return 0;
	}
	//现在type没用
	set<int> types;
	types.insert(type);
	int cnt = uinfo->GetUnviewCount(types);
	MCE_INFO("FeedItemManagerI::GetUnviewedCount. uid:" << uid << " type:" << type << " cnt:" << cnt <<" " << GetIp(cur));
	return cnt;
}
map<int,int>	FeedItemManagerI::GetCountMapByStype(int uid, const vector<int> & stypes, bool unviewed, const Ice::Current& cur ){
  GetRequest req;
	UserInfoPtr uinfo = LocateUserInfo(uid, req);
	if(!uinfo){
		MCE_INFO("FeedItemManagerI::GetCountMapByStype. uid:" << uid << " types:" << PrintHelper::Print(stypes) << " locate uinfo fail " << GetIp(cur));
		return map<int,int>();
	}
	set<int> typeset(stypes.begin(),stypes.end());
	map<int,int> cntmap = uinfo->GetCountByStypeMerged(typeset,unviewed);
	MCE_INFO("FeedItemManagerI::GetCountMapByStype. uid:" << uid << " types:" << PrintHelper::Print(stypes) << " cntmap:" << cntmap.size() <<" " << GetIp(cur));
	return cntmap;
}

FeedItemSeqSeq FeedItemManagerI::GetFeedItemSeqSeqByUsers(const IntSeq& uids,
    const Ice::Current&) {
	MCE_INFO("FeedItemManagerI::GetFeedItemSeqSeqByUsers. uids:" << PrintHelper::Print(uids));
  return FeedItemSeqSeq();
}

void GetLogger::Init(){                                                                                                                    
   logger_ = XceLoggerAdapter::instance().getLogger(1, 32768);                                                                                
	 logger_->initialize();
	 path_.push_back("feed");
	 path_.push_back("news");
}

void GetLogger::SendLog(int uid,const FeedDataSeq & feeds){
 		//if(feeds.empty()){
		//	return;
		//}
 		ostringstream oss;
		oss << "n " << uid << " ";
		for(FeedDataSeq::const_iterator it = feeds.begin(); it != feeds.end(); ++it){
		 	oss << (*it)->actor << " " << ((*it)->type & 0xFFFF )<< " ";
		}
		logger_->addLog(path_, oss.str());
		//MCE_INFO("GetLogger::SendLog. log: " << oss.str());
}

void FeedItemManagerI::load(int uid, const Ice::Current& cur) {
//	MCE_INFO("FeedItemManagerI::load. input: uid:" << uid);
	static int level = 0;
	if(uid > -100 && uid <= 0 ){
		MCE_INFO("FeedItemManagerI::load.old level:" << level << " new level:" << -uid );
		level = -uid;
		return;
	}

//	if(uid % 20 != 0){
//		return ;
//	}

//	for(int i =0; i < level; ++i){
//		MCE_INFO("FeedItemManagerI::load --> " << uid << " level:" << level );
//		GetHotFeedData(uid, 0, 20);
//	}

	if(uid /100 % 10 < level){
		MCE_INFO("FeedItemManagerI::load --> " << uid << " level:" << level  );
		GetHotFeedData(uid, 0, 20);
	}
}

void FeedItemManagerI::LoadExperiment(int totalsize, const Ice::Current& current) {
  InvokeClient tm = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
  ExperimentManager::instance().LoadExperiment(totalsize);
}

void FeedItemManagerI::AddWhiteList(int uid, const Ice::Current& current) {
	MCE_INFO("FeedItemManagerI::AddWhiteList, uid: " << uid);
	IceUtil::RWRecMutex::WLock lock(white_list_mutex_);
	white_list_.insert(uid);
}

void FeedItemManagerI::DelWhiteList(int uid, const Ice::Current& current) {
	MCE_INFO("FeedItemManagerI::DelWhiteList, uid: " << uid);
	IceUtil::RWRecMutex::WLock lock(white_list_mutex_);
	white_list_.erase(uid);
}

MyUtil::IntSeq FeedItemManagerI::GetWhiteList(const Ice::Current& current) {
	MCE_INFO("FeedItemManagerI::GetWhiteList");
	IceUtil::RWRecMutex::RLock lock(white_list_mutex_);
	MyUtil::IntSeq result(white_list_.begin(),white_list_.end());
	return result;
}

void FeedItemManagerI::ClearWhiteList(const Ice::Current& current) {
	MCE_INFO("FeedItemManagerI::ClearWhiteList");
	IceUtil::RWRecMutex::WLock lock(white_list_mutex_);
	white_list_.clear();
}

bool FeedItemManagerI::InWhiteList(int uid) {
	IceUtil::RWRecMutex::RLock lock(white_list_mutex_);
	bool result = white_list_.empty() || white_list_.find(uid) != white_list_.end() ? true : false; 
	if (!white_list_.empty()) {
		MCE_INFO("FeedItemManagerI::InWhiteList, uid: " << uid << " result: " << result);
	}
	return result;
}

void StatTimer::handle(){
		MCE_INFO("@@@@ StatTimer." << UserInfoPool::instance().ToString()
				<< " " << SharedFeedCache::instance().ToString());
}
