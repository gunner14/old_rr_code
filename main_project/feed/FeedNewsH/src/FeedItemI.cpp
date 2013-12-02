
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
#include "XceLoggerAdapter.h"
#include <boost/foreach.hpp>
#include "SharedFeedCache.h"
#include "EDMFeed.h"

//by jin.zhang, 2012.03.01
#include "FeedItemWrapperReplicaAdapter.h"

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
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

  MCE_INFO("@@@@ HeavyFeed:" << sizeof(HeavyFeed) << " object:" << sizeof(Ice::Object)
  		<< " SharedFeed:" << sizeof(SharedFeed) << " SharedFeedPtr:" << sizeof(SharedFeedPtr)
  );
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

	//rong.tang 120321  热度
	TaskManager::instance().schedule(&InteractionDownloader::instance());

}

FeedItemManagerI::FeedItemManagerI() {
	white_list_.insert(-1);
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

void FeedItemManagerI::CheckLoad(int uid, const vector<Ice::Long> & fids, FeedContentDict & dict) {
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
    MCE_INFO("FeedItemManagerI::CheckLoad --> LoadFeedSeq uid:" << uid << " size:" << miss_fids.size());
  }else{
    FeedContentDict dict_loader;
    try {
      dict_loader= FeedMemcLoaderAdapter::instance().GetFeedDict(miss_fids, 100);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedItemManagerI::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " err, " << e);
      return;
    }
    MCE_INFO("FeedItemManagerI::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " res:" << dict_loader.size());
    dict.insert(dict_loader.begin(),dict_loader.end());
  }
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
    CheckLoad(req.uid_,fids,fdict);
    load_db = true;
  }
  //zj//int dbcnt = fdict.size() - memcnt;
  float dbcost = ts.Get();

  return make_pair(memcost,dbcost);
}

FeedDataResPtr FeedItemManagerI::GetLiveFeedData(int uid, int begin, int limit,
    const Ice::Current&) {
  return GetFeedDataImpl(uid, IntSeq(), true, begin, limit, false);
}

FeedDataResPtr FeedItemManagerI::GetLiveFeedDataByStype(int uid,
    const IntSeq& stypes, int begin, int limit, const Ice::Current&) {
  return GetFeedDataImpl(uid, stypes, true, begin, limit, false);
}

FeedDataResPtr FeedItemManagerI::GetLiveFeedDataByType(int uid,
    const IntSeq& types, int begin, int limit, const Ice::Current&) {
	return GetFeedDataImpl(uid, types, false, begin, limit, false);
}

FeedDataResPtr FeedItemManagerI::GetHotFeedData(int uid, int begin, int limit,
    const Ice::Current&) {
		return GetFeedDataImpl(uid, IntSeq(), true, begin, limit, true);
}

FeedDataResPtr FeedItemManagerI::GetHotFeedDataByStype(int uid,
    const IntSeq& stypes, int begin, int limit, const Ice::Current&) {
  return GetFeedDataImpl(uid, stypes, true, begin, limit, true);
}

FeedDataResPtr FeedItemManagerI::GetHotFeedDataByType(int uid,
    const IntSeq& types, int begin, int limit, const Ice::Current&) {
  return GetFeedDataImpl(uid, types, false, begin, limit, true);
}

UserInfoPtr FeedItemManagerI::LocateUserInfo(int uid){
	UserInfoPtr uinfo = UserInfoPool::instance().Find(uid);

	if(uinfo) {
		MCE_INFO("FeedItemManagerI::LocateUserInfo. user in mem. uid:" << uid << " info. " << uinfo->ToString());
		return uinfo;
	}
	MCE_INFO("FeedItemManagerI::LocateUserInfo. user not in mem. uid:" << uid);
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
LongSeq FeedItemManagerI::GetFeedidsFromFriends(int uid, int begin, int limit) {
    LongSeq feedids;
    try {

        UserInfoPtr uinfo = LocateUserInfo(uid);
        if(!uinfo) return feedids;
        Ice::Long minfid = uinfo->GetMinRead();

		try {
			if (minfid == FEEDID_INF) {
				FeedMetaSeq feedmetas = FeedItemWrapperReplicaAdapter::instance().get(uid, 251, 1);//取出250条后面信息，作为最小feedid
				if (!feedmetas.empty()) {
					for (FeedMetaSeq::iterator it = feedmetas.begin(); it != feedmetas.end(); ++it) {
						minfid = std::min(minfid, it->feedid);
						MCE_INFO("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid << " minfid: " << minfid);
					}
				} else {
					MCE_WARN("FeedItemManagerI::GetFeedidsFromFriends uid: " << uid << " minfid: " << minfid << " 251 is empty!");
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

		FeedMetaSeq feedmetas = FeedItemWrapperReplicaAdapter::instance().getLessThanFeedid(uid, minfid, limit);

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

        MCE_INFO("FeedItemManagerI::GetFeedidsFromFriends White list pull model uid: " << uid
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
FeedDataResPtr FeedItemManagerI::GetFeedDataFromFriends(int uid,  int begin, int limit) {
    FeedDataResPtr res = new FeedDataRes;
    if (!res) {
        MCE_WARN("FeedItemManagerI::GetFeedDataFromFriends uid: " << uid
                << ", operator 'new' error." );
        return NULL;
    }
    try {
        LongSeq feedids = GetFeedidsFromFriends(uid, begin, limit);

		FeedDataSeq seq = GetFeedDataByIds(feedids);
		res->data.reserve(seq.size());

		for(FeedDataSeq::reverse_iterator it = seq.rbegin();
				it != seq.rend(); ++it) {
        	res->data.push_back(*it);
		}

        res->UnviewedCount = res->data.size();

        MCE_DEBUG("FeedItemManagerI::GetFeedDataFromFriends --> uid: " << uid
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
	    TimeStat ts; GetCost tc;
        GetRequest req(uid,types,stype,begin,limit,hot);

        if(!req.CheckArgs()) return res;


        UserInfoPtr uinfo = LocateUserInfo(uid);
        if(!uinfo) return res;
        //	long pre_max_read = uinfo->GetMaxRead();
        int recent_cnt = uinfo->GetRecentCnt();
        tc.loadtime = ts.Get();

        uinfo->MergeAndSort(req);
        tc.mergetime = ts.Get();
        //PrintHelper::Print(req.hfeed3d_,uid);
        FeedContentDict fdict;
        pair<float,float> p = GetFeedContentDict(req,fdict);
        tc.contenttime = ts.Get();

        FeedBuilder builder(uid,fdict,req.showed_comment_map_,req.sort_);
        res = builder.BuildFeedData(req);

		//pull more , by guanghe.ge 2012.3.6
		if ((res->data.empty()) && InWhiteList(uid)) {
        	return GetFeedDataFromFriends(uid, begin, limit);
		}

        //以下为收尾工作
        uinfo->SetRecentCnt(0);
        res->UnviewedCount = MakeUnviewCount(req.GetOlderCount());
        tc.buildtime = ts.Get();

        uinfo->SetReadByIdsExactly(builder.GetNullFids());
        req.ClearEdms();
        tc.othertime = ts.Get();


        //设置min feedid, 写memcache
        //by jin.zhang(张劲), 2012.02.24
        //uinfo->SetMinRead(req.GetMinFid());
        uinfo->SetMinRead(FEEDID_INF);
	    MinFidUploader::instance().Add(uid, uinfo->GetMinRead());

        bool maxchanged = uinfo->SetMaxRead(req.GetMaxFid());
        uinfo->SetRefreshTime(req.GetNowTime());

        MaxFidUploader::instance().Add(uid,uinfo->GetMaxRead(),uinfo->GetRefreshTime());

        if(req.GetMaxFid() > 0 and req.MustIncViewBelowFid())
            uinfo->IncViewBelowFid(req.GetMaxFid());



        MCE_INFO("FeedItemManagerI::GetFeedDataImpl --> uid:" << uid << " begin:" << begin << " limit:" << limit
                << " types:" << types.size() << " bystype:" << stype << " hot:" << hot << " feedsize:" << uinfo->Size()
                << " res:" << res->data.size()
                << tc.ToString()
                << " fdict:" << fdict.size()
                << " nullfids:" << builder.GetNullFids().size()
                //			<< " nullfids:" << PrintHelper::Print(builder.GetNullFids())
                << " maxchanged:" << maxchanged
                << " maxreadfid:" << uinfo->GetMaxRead()
                //log输出MinFeedid, by jin.zhang(张劲), 2012.02.24
                << " minreadfid:" << uinfo->GetMinRead()
                << " refresht:" << uinfo->GetRefreshTime()
                << " older_count:" << GetOlderCount(res->UnviewedCount)
                << " has_older:" << HasOlder(res->UnviewedCount)
                << " show_fids:" << req.GetShowedFids().size()
                << " useless_count:" << req.GetUselessCount()
                << " types:" << PrintHelper::Print(types)
                //		<< " include_types:" << PrintHelper::Print(req.GetIncludeTypes())
                << " exclude_types:" << PrintHelper::Print(req.GetExcludeTypes())
                << " recent_cnt:" << recent_cnt
                //			<< " uinfo:" << uinfo->ToString()
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
	//if(fitem.time > 0){
	//	MCE_INFO("FeedItemManager::AddFeed. fid:" << fitem.feed << " time:" << fitem.time << " id2weight:" << id2weight.size()
	//			<< " .not add");
	//	return;
	//}

	SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);

	vector<int> uids, uids_mem;

	BOOST_FOREACH(const MyUtil::Int2IntMap::value_type & vt, id2weight){
		int uid = vt.first;
		UserInfoPtr uinfo = FindUserInfo(uid);
		if(uinfo){
			HeavyFeed hfeed(fitem,hdl);
			hfeed.SetUpdateTime(config->updateTime);
			hfeed.SetWeight(vt.second);
			uinfo->AddHeavyItem(uid,hfeed);
			uids_mem.push_back(uid);
		}
		if(uid == 220678672
				|| uid == 347348947
				|| uid == 263734453 ){
			MCE_INFO("FeedItemManager::AddFeed. @@. target:" << uid << " fid:" << fitem.feed  << " actor:" << fitem.actor << " weight:" << vt.second);	
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
	MCE_INFO("FeedItemManagerI::SetRead. uid:" << uid << " fid:" << fid
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
    CheckLoad(0,fids,dict);
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

	UserInfoPtr uinfo = LocateUserInfo(uid);
	float locatec = ts.Get();

//	uinfo->LoadFeedCache();

	FeedItemSeq res = uinfo->GetOrigFeedItems();
	float get = ts.Get();

	MCE_INFO("FeedItemManagerI::GetOriginalFeedData. uid:" << uid << " size:" << res.size()
			<< " locate:" << locatec  << " get:" << get);
	return res;
}

int FeedItemManagerI::GetUnviewedCount(int uid, int type, const Ice::Current& cur) {
	UserInfoPtr uinfo = LocateUserInfo(uid);
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
	UserInfoPtr uinfo = LocateUserInfo(uid);
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

