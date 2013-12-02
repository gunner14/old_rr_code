#include <sys/resource.h>
#include "ServiceI.h"
#include "FeedAssistantReplicaAdapter.h"
#include "FeedCacheNReplicaAdapter.h" 
#include "PerformanceStat.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "FeedMemcProxy/client/feed_memc_client.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "feed/FeedRedisClient/src/FeedIndex/FeedIndexRedisClient.h"
#include "FeedNewsReplicaAdapter.h"
#include "FeedGroupAdapter.h"
#include "FeedItemFacadeReplicaAdapter.h"
#include "FeedDbWriterReplicaAdapter.h"
#include "UserStateAdapter.h"
#include "UserConfigAdapter.h"
#include "UserFeeds.h"
#include "Tasks.h"
#include "Common.h"
#include "FeedPipe.h"
#include "FeedDispatchNewI.h"
#include "ExpressionCache.h"

using namespace com::xiaonei::xce;
using namespace xce::feed;
using namespace xce::adapter;
using namespace std;
using namespace MyUtil;
using namespace boost;
using namespace ::xce::serverstate;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  
   service.getAdapter()->add(&FeedDispatchNewI::instance(),
      service.createIdentity(FEED_DISPATCHNEW, ""));

  TaskManager::instance().scheduleRepeated(new StatTimer());
  //TaskManager::instance().scheduleRepeated(&FeedReplyBufferTimer::instance());
  TaskManager::instance().scheduleRepeated(&PerformanceStat::instance());

  MyUtil::ThreadPoolConfig tpc(4, 4);
  MyUtil::ThreadPoolConfig fdt(200,500);
  MyUtil::ThreadPoolConfig fdrt(200,500);
  TaskManager::instance().config(REMOVE_TASK_LEVEL, tpc);
  TaskManager::instance().config(DISPATCH_TASK_LEVEL,fdt);
  TaskManager::instance().config(DISPATCH_BIG_FANST_FEED_ASK_LEVEL,ThreadPoolConfig(1,1));
  PipePool::instance().initialize(new PipeFactoryI);
	ReplyLogger::instance().Init();

  DB_INSTANCE = service.getCommunicator()->getProperties()->getPropertyWithDefault(
      "FeedDBInstance", "feed_db");
//------------------For HA------------------
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt(
      "FeedDispatchNew.Mod");
  int interval = 5;
  ServerStateSubscriber::instance().initialize("ControllerFeedDispatchNewR",&FeedDispatchNewI::instance(), mod, interval, new XceFeedControllerChannel());

}
//---------------------------------------------------------------------------
static string MakeExpression(LoadType type, long id) {
  ostringstream oss;
  switch (type) {
  case Friend:
    oss << "f";
    break;
  case App:
    oss << "a";
    break;
  case AppFan:
    oss << "A";
    break;
  case Page:
    oss << "p";
    break;
  case HighSchool:
    oss << "s";
    break;
  case Follow:
    oss << "v";
    break;
  case MiniGroup:
    oss << "g";
    break;
  case XiaoZu:
    oss << "z";
    break;
  case MiniSite:
    oss << "x";
    break;
  case GuangJie:
    oss << "j";
    break;
// shilong.li 2012-07-23
//  case GuangJieTopic:
//    oss << "J";
//    break;
  case Tag:
    oss << "t";
    break;
//shilong.li 2012-07-23
//  case Album:
//    oss << "b";
//    break;
  default:
    throw logic_error("FeedDispatchNewI::Reload ");
  }
  oss << "(" << id << ")";
  return oss.str();
}

//---------------------------------------------------------------------------

void FeedDispatchNewI::Reload(LoadType type, int id, const Ice::Current& current) {
  string expr = MakeExpression(type, id);
  MCE_INFO("FeedDispatchNewI::Reload --> factor:" << expr<<" "<<current.con->toString() );
  ExprCache::instance().InvalidateFactor(expr);
}
void FeedDispatchNewI::Reload2(LoadType type, long id, const Ice::Current&) {
/*
  string expr = MakeExpression(type,id);
  MCE_INFO("FeedDispatchNewI::Reload2 --> factor:"<< expr);
  ExprCache::instance().InvalidateFactor(expr);
  */
}
void FeedDispatchNewI::ReloadBatch(LoadType type, const vector<int> & ids,
    const Ice::Current&) {
  for (unsigned i = 0; i != ids.size(); ++i) {
    string expr = MakeExpression(type, ids.at(i));
    MCE_INFO("FeedDispatchNewI::ReloadBatch --> factor:" << expr );
    ExprCache::instance().InvalidateFactor(expr);
  }
}
void FeedDispatchNewI::AddBuddyNotify(int host, int guest, const Ice::Current& current) {
  MCE_INFO("FeedDispatchNewI::addBuddyNotify. host:" << host << " guest:" << guest<<" "<<current.con->toString() );
	char * funcname = "addBuddyNotify";
  AppendNewsFeed(host, guest, 600, 1, funcname);//日志
  AppendNewsFeed(host, guest, 700, 2, funcname);//照片
  AppendNewsFeed(host, guest, 500, 1, funcname);//状态类
  AppendNewsFeed(host, guest, 100, 2, funcname);//分享

  AppendNewsFeed(guest, host, 600, 1, funcname);//日志
  AppendNewsFeed(guest, host, 700, 2, funcname);//照片
  AppendNewsFeed(guest, host, 500, 1, funcname);//状态类
  AppendNewsFeed(guest, host, 100, 2, funcname);//分享
}
void FeedDispatchNewI::JoinPageNotify(int page, int fans, const Ice::Current& current) {
  MCE_INFO("FeedDispatchNewI::joinPageNotify. page:" << page << " fans:" << fans<<" "<<current.con->toString() );
  AppendNewsFeed(page, fans, 5, "joinPageNotify");
}

void FeedDispatchNewI::GuideBuddyReplyNotify(int newUser, int buddy,
    const Ice::Current& current) {
  MCE_INFO("FeedDispatcherNI::guideBuddyReplyNotify --> newUser:"<<newUser << ", buddy:"<<buddy << " " << current.con->toString());
	char * funcname = "guideBuddyReplyNotify";
  AppendNewsFeed(buddy, newUser, 600, 1, funcname);//日志
  AppendNewsFeed(buddy, newUser, 700, 2, funcname);//照片
  AppendNewsFeed(buddy, newUser, 500, 1, funcname);//状态类
  AppendNewsFeed(buddy, newUser, 100, 2, funcname);//分享
}

void FeedDispatchNewI::BuddyApplyNotify(int host, int guest,
    const Ice::Current& current) {
  bool
      is_guide =
          !(xce::adapter::userstate::UserStateAdapter::instance().getUserState(
              host)->isGuideDone());
  if (!is_guide) {
    return;
  }
  UserConfigInfoPtr user_config = UserConfigAdapter::instance().getUserConfig(
      guest);
  if (user_config->basicConfig() == 0) {
    return;
  }
  MCE_INFO("FeedDispatchNewI::buddyApplyNotify --> host:" << host << " guest:" << guest<< " " << current.con->toString());
	char * funcname = "buddyApplyNotify";
  AppendNewsFeed(guest, host, 600, 1, funcname);//日志
  AppendNewsFeed(guest, host, 700, 2, funcname);//照片
  AppendNewsFeed(guest, host, 500, 1, funcname);//状态类
  AppendNewsFeed(guest, host, 100, 2, funcname);//分享 

}

void FeedDispatchNewI::AddMiniSiteFeed(int siteid, int userid, const Ice::Current& current ) {
	static const int limit = 5;
	MCE_INFO("FeedDispatchNewI::addMiniSiteFeed siteid: " << siteid << " userid: " << userid << " limit: " << limit<<" "<<current.con->toString());

  ostringstream expr;
  expr << "u(" << userid << ")";

  FeedConfigPtr config = new FeedConfig();
  config->expr = expr.str();
  config->sendConfig = 0;
  config->sendMiniConfig = 0;
  config->sendNewsConfig = 1;
  config->updateTime = 1;

	IntSeq siteseq;
	siteseq.push_back(siteid);
	FeedMetaSeq items = FeedItemCacheByUseridReplicaAdapter::instance().get(siteseq, 0, limit);

	if (items.empty()) {
		MCE_INFO("FeedDispatchNewI::AddMiniSiteFeed -> FeedItemCacheByUseridReplicaAdapter::get Result is EMPTY. siteid: " << siteid << " limit: " << limit);
		return ;
	}
	MyUtil::LongSeq feedids;
	feedids.reserve(items.size());
	for(FeedMetaSeq::iterator it = items.begin(); it != items.end(); ++it) {
		feedids.push_back(it->feedid);
	}

	ostringstream os;
	os << "[ ";
	for(FeedMetaSeq::iterator it = items.begin(); it != items.end(); ++it) {
		feedids.push_back(it->feedid);
		os << it->feedid << " ";
	}
	os << "]";
	MCE_INFO("FeedDispatchNewI::AddMiniSiteFeed -> Dispatcher source feedids: " << os.str());
	FeedContentDict dict;
	try{
		dict = FeedMemcClient::instance().GetFeedDict(feedids);
	}catch(Ice::Exception& e){
		MCE_WARN("FeedDispatchNewI::AddMiniSiteFeed --> err:"<<e << ", file:"<<__FILE__ << ", line:"<<__LINE__ << " uid:" << siteid); 
	}

	for(FeedMetaSeq::iterator it = items.begin(); it != items.end(); ++it) {

		FeedContentDict::iterator findit = dict.find(it->feedid);
		long source = 0;
    if (findit == dict.end()) {
       FeedIndexPtr index =  DbHelper::instance().getFeedIndex(it->feedid);
       if (index) {
         source = index->source;
       } else {
         continue;
       }
    } else {
      source = findit->second->data->source;
    }
    FeedSeedPtr seed = new FeedSeed;

    seed->feed = it->feedid;
    seed->actor = it->userid;
    seed->baseWeight = it->weight;
    seed->miniMerge = it->mergeid;
    seed->newsMerge = it->mergeid;
    seed->psource = 0;
    seed->ptype = 0;
    seed->source = source;
    seed->time = it->time;
    seed->type = it->type;

    seed->extraProps.insert(make_pair("MachineDispatch", "addMiniSiteFeed"));  //由机器分发统一添加youjiqifenfa属性，并在Value中指定函数名
    TaskManager::instance().execute(new FeedDispatchTask(seed,0,config));
  } 
}

void FeedDispatchNewI::AddMiniSiteFeedToGuides(long source,int stype,int siteid, const MyUtil::IntSeq& guides, const Ice::Current& current) {
  static const unsigned int limit = 5;
  ostringstream og;
  og<<"[";
  for(size_t i = 0; i < guides.size(); ++i) {
    og<<guides[i]<<" ";
  }
  og<<"[";
  MCE_INFO("FeedDispatchNewI::AddMiniSiteFeedToGuides --> source:" << source << " stype:"<< stype <<" siteid:"<< siteid << " guides:"<<og.str()<<" "<<current.con->toString());

  FeedConfigPtr config = new FeedConfig();
  config->sendConfig = 0;
  config->sendMiniConfig = 0;
  config->sendNewsConfig = 1;
  config->updateTime = 1;
  FeedIndexSeq indexSeq  =  GetFeedIndex(source,stype,siteid);
  if (indexSeq.empty()) {
    MCE_WARN("FeedDispatchNewI::AddMiniSiteFeedToGuides --> GetFeedIndex return empty siteid:"<< siteid<<" stype:"<<stype <<" source:"<<source);
    return ;
  }

  MyUtil::LongSeq feedids;
  ostringstream os;
  os << "[ ";
  for(size_t i =0; i < indexSeq.size(); ++i) {
    feedids.push_back(indexSeq.at(i));
    os << indexSeq.at(i) << " ";
  }
  os << "]";
  MCE_INFO("FeedDispatchNewI::AddMiniSiteFeedToGuides -->  feedids:"<< os.str());
	FeedContentDict dict;
	try{
		dict = FeedMemcClient::instance().GetFeedDict(feedids);
	}catch(Ice::Exception& e){
		MCE_WARN("FeedDispatchNewI::AddMiniSiteFeedToGuides --> err:"<<e <<" uid:" << siteid); 
	}

  for(size_t i = 0 ; i < indexSeq.size() && i < limit; ++i ) {
    FeedContentDict::iterator findit = dict.find(indexSeq[i]->feed);
    int baseWeight = 15;
    int type = 0;
    map<string,string> content;
    if (findit == dict.end()) {
      if (DbHelper::instance().isNewFeedidForDb(indexSeq[i]->feed)) {
        content = DbHelper::instance().getFeedZoneContent(indexSeq[i]->feed);
      } else {
        content = DbHelper::instance().getFeedContent(indexSeq[i]->feed);
      }
      if (content.empty()) {
        continue;
      }
      string prop = content["prop"];
      string str  = prop.substr(0,prop.find(','));
      string weight = str.substr(str.find('=') + 1);
      try {
        baseWeight =  boost::lexical_cast<int>(weight); 
        type       =  boost::lexical_cast<int>(content["type"]);
      } catch (...) {
        MCE_WARN("FeedDispatchNewI::AddMiniSiteFeedToGuides --> string weight to int err "); 
      }
      MCE_DEBUG("FeedDispatcherNewI::AddMiniSiteFeedToGuides--> prop"<<prop<<" str:"<<str<<" weight:"<<weight<<" baseWeight:"<<baseWeight<<" type:"<<type);
    } else {
       baseWeight = findit->second->data->weight; 
       type       = findit->second->data->type;
    }
    FeedSeedPtr seed = new FeedSeed;
    seed->feed = indexSeq[i]->feed;
    seed->actor = indexSeq[i]->actor;
    seed->baseWeight = baseWeight;
    seed->miniMerge = indexSeq[i]->miniMerge;
    seed->newsMerge = indexSeq[i]->newsMerge;
    seed->psource = 0;
    seed->ptype = 0;
    seed->source = indexSeq[i]->source;
    seed->time = indexSeq[i]->time;
    seed->type = type;
    seed->extraProps.insert(make_pair("MachineDispatch", "addMiniSiteFeed"));  //由机器分发统一添加youjiqifenfa属性，并在Value中指定函数名
    ostringstream expr;
    for (size_t i = 0; i < guides.size() && i < 2000; ++i) {
      FeedCacheNReplicaAdapter::instance().get(guides[i]);// 推送新鲜事之前,先将guide用户加载到FeedCache中,避免出现向guide用户分发新鲜事丢失的情况
      expr << "u(" << guides[i] << ")";
      config->expr = expr.str();
      TaskManager::instance().execute(new FeedDispatchTask(seed,0,config));
      expr.str("");
      MCE_INFO("FeedDispatchNewI::AddMiniSiteFeedToGuides --> Dispatch feed:"<<seed->feed<<" to guide:"<<guides[i]);
    }
  } 
}

void FeedDispatchNewI::AppendNewsFeed(int from, int to, int limit, char * funcname) {
  ostringstream expr;
  expr << "u(" << to << ")";
  FeedConfigPtr config = new FeedConfig();
  config->expr = expr.str();
  config->sendConfig = 0; //设定为不发送
  config->sendMiniConfig = 0;
  config->sendNewsConfig = 1;
  config->updateTime = 1;

  ostringstream tab;
  tab << "feed_mini_" << from % 100;
  Statement sql;
  sql << "select * from " << tab.str() << " where user=" << from
    << " order by time desc limit " << limit;//按时间排序，最多只有limit个
  mysqlpp::StoreQueryResult res =
    QueryRunner("feed_db", CDbRServer, tab.str()).store(sql);
  if (!res) {
    return;
  } 

  for (size_t i = 0; i < res.num_rows(); ++i) {

    FeedSeedPtr seed = new FeedSeed;
    mysqlpp::Row row = res.at(i);
    seed->feed = row["feed"];
    seed->actor = row["user"];
    seed->baseWeight = 0;
    seed->miniMerge = row["mini_merge"];
    seed->newsMerge = seed->miniMerge;
    seed->psource = 0;
    seed->ptype = 0;
    seed->source = row["source"];
    seed->time = (time_t) mysqlpp::DateTime(row["time"]);
    seed->type = row["type"];
    if (NULL != funcname) {
      seed->extraProps.insert(make_pair("MachineDispatch", funcname));  //由机器分发统一添加youjiqifenfa属性，并在Value中指定函数名
    } else {
      seed->extraProps.insert(make_pair("MachineDispatch","AppendNewsFeed"));
      MCE_WARN("FeedDispatchN::AppendNewsFeed -> funcname is NULL. feedid: " << seed->feed << " actor: " << seed->actor);  //由机器构造函数必需传入调用函数名称
    }
    TaskManager::instance().execute(new FeedDispatchTask(seed,0,config));
  }
}


void FeedDispatchNewI::AppendNewsFeed(int from, int to, int type, int limit, char * funcname) {
  ostringstream expr;
  expr << "u(" << to << ")";

  FeedConfigPtr config = new FeedConfig();
  config->expr = expr.str();
  config->sendConfig = 0;
  config->sendMiniConfig = 0;
  config->sendNewsConfig = 1;
  config->updateTime = 1;

  ostringstream tab;
  tab << "feed_mini_" << from % 100;
  Statement sql;
  sql << "select * from " << tab.str() << " where user=" << from
    << " and big_type = " << type << " order by time desc limit " << limit;
  mysqlpp::StoreQueryResult res =
    QueryRunner("feed_db", CDbRServer, tab.str()).store(sql);

  if (!res) {
    return;
  }
  for (size_t i = 0; i < res.num_rows(); ++i) { 
    FeedSeedPtr seed = new FeedSeed;
    mysqlpp::Row row = res.at(i);
    seed->feed = row["feed"];
    seed->actor = row["user"];
    seed->baseWeight = 0;
    seed->miniMerge = row["mini_merge"];
    seed->newsMerge = seed->miniMerge;
    seed->psource = 0;
    seed->ptype = 0;
    seed->source = row["source"];
    seed->time = time(NULL);//(time_t) mysqlpp::DateTime(row["time"]);
    seed->type = row["type"];
    if (NULL != funcname) {
      seed->extraProps.insert(make_pair("MachineDispatch", funcname));  //由机器分发统一添加youjiqifenfa属性，并在Value中指定函数名
    } else {
      seed->extraProps.insert(make_pair("MachineDispatch","AppendNewsFeed"));
      MCE_WARN("FeedDispatchN::AppendNewsFeedWithType -> funcname is NULL. feedid: " << seed->feed << " actor: " << seed->actor);  //由机器构造函数必需传入调用函数名称
    }
    TaskManager::instance().execute(new FeedDispatchTask(seed,0,config)); 
  } 
}

void FeedDispatchNewI::Dispatch(const FeedSeedPtr& seed, const Ice::Current&) {
  if (seed) {
    MCE_INFO("FeedDispatchNewI::Dispatch --> feed:" << seed->feed << " actor:" << seed->actor << " stype:" << (seed->type&0xFFFF)
        << " source:" << seed->source << " mergetype:" << (((unsigned int)seed->type) >> 30));
    if (seed->feed <= 0) {
      MCE_WARN("FeedDispatchNewI::Dispatch  feedid < 0");
      return;
    }
    TaskManager::instance().execute(new FeedDispatchTask(seed));//没有reply
    try{
      FeedDbWriterReplicaAdapter::instance().FeedDispatch2DB(seed,NULL);
    } catch (Ice::Exception & e){
      MCE_WARN("FeedDispatchNewI::Dispatch --> call FeedDbWriter::FeedDispatch2DB err:"<<e);
    }
  }
}

void FeedDispatchNewI::DispatchWithReply(const FeedSeedPtr& seed,
    const FeedReplyPtr& reply, const Ice::Current&) {
  if (seed) {
    MCE_INFO("FeedDispatchNewI::DispatchWithReply  --> feed:" << seed->feed << " actor:" << seed->actor << " stype:" << (seed->type&0xFFFF)
        << " source:" << seed->source << " mergetype:" << (((unsigned int)seed->type) >> 30));
    if (seed->feed <= 0) {
      MCE_WARN("FeedDispatchNewI::DispatchWithReply  feedid < 0");
      return;
    }
    TaskManager::instance().execute(new FeedDispatchTask(seed, reply));
    try{
      FeedDbWriterReplicaAdapter::instance().FeedDispatch2DB(seed,reply);
    } catch (Ice::Exception & e){
      MCE_WARN("FeedDispatchNewI::Dispatch --> call FeedDbWriter::FeedDispatch2DB err:"<<e);
    }
  }
}
void FeedDispatchNewI::DispatchEDM(const FeedSeedPtr& seed, const Ice::Current&) {
  if (seed) {
    IceUtil::Mutex::Lock lock(_EDMMutexs[seed->source % EDM_MUTEX_COUNT]);

    unsigned int baseWeightId = 0;
    if (seed->extraProps.find("base_weight_id") != seed->extraProps.end()) {
      try {
        baseWeightId = boost::lexical_cast<int>(
            seed->extraProps["base_weight_id"]);
      } catch (std::exception & e) {
        MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> cast base_weight_id err:" << e.what());
      }
    }
    Ice::Long feedid = -1;
    FeedIndexSeq seq;
    seq = GetFeedIndex(seed->source,(seed->type & 0xffff),-1);
    if (!seq.empty()) {
      feedid = seq.at(0)->feed;
    }
    if (feedid < 0) {
      try {
        feedid = FeedAssistantReplicaAdapter::instance().createFeedId();
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> call FeedDispatchNew to get feed id err, " << e);
        return;
      }
      if (feedid < 0) {
        MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> feed less than 0");
        return;
      }
      seed->feed = feedid;
      MCE_INFO("FeedDispatchNewI::DispatchEDMFeed --> add to db source:" << seed->source << " feedid:"
          << seed->feed << " stype:" << (seed->type&0xFFFF) << " actor:" << seed->actor);
      if (seed->time < 0) {
        //if time less than 0, sql will fail
        seed->time = time(NULL);
      }

      try {
        FeedDbWriterReplicaAdapter::instance().FeedDispatchEDM2DB(seed);
      } catch (Ice::Exception & e) {
        MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> call FeedDbWriter::FeedDispatchEDM2DB err:"<<e);
      }
      FeedDataPtr data = new FeedData();
      data->feed = seed->feed;
      data->source = seed->source;
      data->actor = seed->actor;
      data->type = seed->type;
      data->xml = seed->xml;
      data->time = seed->time;

      data->weight = (baseWeightId << 16) & 0x00ff0000;
      seed->baseWeight = data->weight;
      try {
        FeedContentPtr content = new FeedContent();
        content->data = data;
        bool res = FeedMemcClient::instance().SetFeed(content);
        if (!res) {
          MCE_INFO("FeedDispatchNewI::DispatchEDMFeed --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__);
        }
      } catch (std::exception& e) {
        MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> call Memc err, " << e.what());
      } catch (...) {
        MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> call Memc err, unknown");
      }
    }

    seed->feed = feedid;
    seed->baseWeight = (baseWeightId << 16) & 0x00ff0000;

    FeedConfigPtr config = new FeedConfig();
    config->expr = seed->extraProps["expr"];
    config->updateTime = seed->extraProps["update_time"] == "1";
    config->sendConfig = seed->extraProps.find("send_config")
      != seed->extraProps.end();
    config->sendNewsConfig = seed->extraProps["publish_news"] == "1";
    config->sendMiniConfig = seed->extraProps["publish_mini"] == "1";

    try {
      SetEdmTime(seed);
      seed->extraProps.insert(make_pair("MachineDispatch","DispatchEDM"));
      TaskManager::instance().execute(new FeedDispatchTask(seed,0,config));
      MCE_INFO("FeedDispatchNewI::DispatchEDMFeed --> Dispatch feedid:" << seed->feed
          << " stype:" << (seed->type&0xFFFF) << "source:" << seed->source << " actor:" << seed->actor << " time:" << seed->time);
      try {
        FeedItemFacadeReplicaAdapter::instance().put(seed);
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedDispatchNewI::DispatchEDM --> call FeedItemFacade feed:" << seed->feed << " err, " << e);
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> Dispatch err, " << e);
    } catch (...){
      MCE_WARN("FeedDispatchNewI::DispatchEDMFeed --> Dispatch unknown err. line:" << __LINE__);
    }
  }
}

void FeedDispatchNewI::DispatchAgain(Ice::Long source,int stype,int actor,int toid,const Ice::Current & current) {
  FeedIndexSeq index_seq = GetFeedIndex(source,stype,actor);
  FeedIndexPtr index;
  for (size_t i = 0; i < index_seq.size(); ++i) {
    if(!index || index->feed < index_seq.at(i)->feed){
      index = index_seq.at(i);
    }
  }
  if (!index) {
    MCE_DEBUG("FeedDispatchNewI::DispatchAgain --> can not find index");
    return;
  }else{
    MCE_DEBUG("FeedDispatchNewI::DispatchAgain --> find index --> feedid = " << index->feed);
  }		

  ostringstream to;
  to << "u(" << toid << ")";
  FeedConfigPtr config = new FeedConfig();
  config->expr = to.str();
  config->sendConfig = 1; //设定为不发送
  config->sendMiniConfig = 0;
  config->sendNewsConfig = 1;
  config->updateTime = 1;

  FeedSeedPtr seed = new FeedSeed;
  seed->feed = FeedAssistantReplicaAdapter::instance().createFeedId();
  seed->actor = index->actor;
  seed->baseWeight = 18 << 16;
  seed->miniMerge = index->source;
  seed->newsMerge = index->source;
  seed->psource = index->psource;
  seed->ptype = index->ptype;
  seed->source = index->source;
  seed->time = time(NULL);
  seed->type = 0x50000000;

  if (stype == 601) {
    seed->type |= 3001;
  } else if (stype == 701) {
    seed->type |= 3002;
  } else if (stype == 709) {
    seed->type |= 3003;
  } else {
    return;
  }

  FeedDataPtr data = new FeedData();
  map<string, string> old_data;
  data->feed = seed->feed;
  data->source = seed->source;
  data->actor = seed->actor;
  data->type = seed->type;
  data->time = seed->time;
  data->weight = seed->baseWeight;
  if( DbHelper::instance().isNewFeedidForDb(index->feed)) {
    old_data = DbHelper::instance().getFeedZoneContent(index->feed);
  }else {
    old_data = DbHelper::instance().getFeedContent(index->feed);
  }
  data->xml = old_data["xml"]; 

  try {
    FeedContentPtr content = new FeedContent();
    content->data = data;
    bool res =  FeedMemcClient::instance().SetFeed(content);
    if (!res) {
      MCE_INFO("FeedDispatchNewI::DispatchAgain --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__);
    }
    //}
} catch (std::exception& e) {
  MCE_WARN("FeedDispatchNewI::DispatchAgain --> call Memc err, " << e.what());
} catch (...) {
  MCE_WARN("FeedDispatchNewI::DispatchAgain --> call Memc err, unknown");
}
seed->extraProps.insert(make_pair("MachineDispatch","DispatchAain"));
TaskManager::instance().execute(new FeedDispatchTask(seed,0,config));
}
void FeedDispatchNewI::DispatchNewsAgain(Ice::Long feedid,const string& expr,const Ice::Current & current) {
  FeedIndexPtr index = DbHelper::instance().getFeedIndex(feedid);
  if (!index) {
    return;
  }

  FeedConfigPtr config = new FeedConfig();
  config->expr = expr;
  config->sendConfig = 1; //设��~Z为��~M�~O~Q�~@~A
  config->sendMiniConfig = 0;
  config->sendNewsConfig = 1;
  config->updateTime = 1;

  FeedSeedPtr seed = new FeedSeed;

  seed->feed = index->feed;
  seed->actor = index->actor;
  seed->baseWeight = 18 << 16;
  seed->miniMerge = index->source;
  seed->newsMerge = index->source;
  seed->psource = index->psource;
  seed->ptype = index->ptype;
  seed->source = index->source;
  seed->time = time(NULL);
  seed->type = 0x50000000 | index->stype;

  seed->extraProps.insert(make_pair("MachineDispatch","DispatchNewsAain"));
  TaskManager::instance().execute(new FeedDispatchTask(seed,0,config));
}

void FeedDispatchNewI::AddFeedReply(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const FeedReplyPtr& reply, const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::AddFeedReply --> source = " << source << ",stype = " << stype << ",actor = " << actor);
  if (!reply) {
    MCE_WARN("FeedDispatchNewI::AddFeedReply --> reply is NULL source = " << source << ",stype = " << stype << ",actor = " << actor);
    return;
  }
  TaskManager::instance().execute(new ReplyDispatchTask(source, stype, actor,
        reply));
}

void FeedDispatchNewI::AddFeedReplyById(int stype, Ice::Long feedid,
    const FeedReplyPtr& reply, const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::AddFeedReplyById ---> stype:" << stype << "feedid:" << feedid);
  FeedIndexPtr feedIndex= DbHelper::instance().getFeedIndex(feedid);
  if (!feedIndex) {
    MCE_WARN("FeedDispatchNewI::AddFeedReplyById ---> can not get feed, stype:" << stype << "feedid:" << feedid);
    return;
  }
  if (!reply) {
    MCE_WARN("FeedDispatchNewI::AddFeedReplyById ---> reply is NULL stype:" << stype << "feedid:" << feedid);
    return;
  }
  ReplyLogger::instance().AddReply(feedid,feedIndex->source,feedIndex->stype,feedIndex->actor,reply);

  try {
    FeedContentPtr content =
      FeedMemcClient::instance().GetFeed(feedIndex->feed);
    if (content) {
      content->reply = reply;
      FeedMemcClient::instance().SetFeed(content);
    }
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchNewI::AddFeedReplyById--> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("FeedDispatchNewI::AddFeedReplyById --> call Memc err, unknown");
  }
  try {
    LongSeq feedIds;
    feedIds.push_back(feedid);
    FeedDbWriterReplicaAdapter::instance().UpdateReply2DB(feedIds,reply);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedDispatchNewI::AddFeedReplyById --> call FeedDbWriter::UpdateReply2DB  err:"<<e);
  }
}
void FeedDispatchNewI::AddFeedReplyWithConfig(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const FeedReplyPtr& reply, const map<string,string> & config, const Ice::Current& current){
  MCE_INFO("FeedDispatchNewI::AddFeedReplyWithConfig --> source = " << source << ",stype = " << stype << ",actor = " << actor << ",config size:" << config.size());
  vector<long> fids;
  FeedIndexSeq indexSeq = GetFeedIndex(source,stype,actor);
  for( size_t i = 0; i < indexSeq.size(); ++i) {
    fids.push_back(indexSeq.at(i)->feed);
  }
  if (fids.empty()) {
    MCE_WARN("FeedDispatchNewI::AddFeedReplyWithConfig Locate fids fail");
    return;
  }
  if (!reply) {
    MCE_WARN("FeedDispatchNewI::AddFeedReplyWithConfig ---> reply is NULL source:"<<source<<" stype"<< stype << "actor:" << actor);
    return;
  }
  TaskManager::instance().execute(new ReplyDispatchTask(source,stype,
        actor, reply));
  RefleshMiniGroupTime(config,reply,fids,actor);
}

void FeedDispatchNewI::RemoveFeedByTime(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Int time, const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::RemoveFeedByTime --> source:" << source << " stype:" << stype << " actor:" << actor << " time:" << time);
  TaskManager::instance().execute(new RemoveFeedByTimeTask(source, stype, actor, time));

}

void FeedDispatchNewI::RemoveFeedById(Ice::Long feedId, const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::RemoveFeedById --> feedid:" << feedId);
  TaskManager::instance().execute(new RemoveFeedByIdTask(feedId));
}
void FeedDispatchNewI::RemoveFeedBySourceHS(Ice::Long source,Ice::Int stype, const Ice::Current & current) {
  MCE_INFO("FeedDispatchNewI::RemoveFeedBySourceHS --> source:"<<source << ", stype:"<<stype );
  RemoveFeedBySource(source,stype);
}
void FeedDispatchNewI::RemoveFeedBySource(Ice::Long source, Ice::Int stype,
    const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::RemoveFeedBySource --> source:"<<source << ", stype:"<<stype );
  RemoveKeyPtr st = new RemoveKey();
  st->source = source;
  st->type = stype;
  st->op = BySource;

  PipePool::instance().push("FeedPipe", 0, st);
}
void FeedDispatchNewI::RemoveFeedByParent(Ice::Long parent, Ice::Int ptype,
    const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::RemoveFeedByParent --> parent:"<<parent << ", ptype:"<<ptype);
  RemoveKeyPtr st = new RemoveKey();
  st->source = parent;
  st->type = ptype;
  st->op = ByParent;

  PipePool::instance().push("FeedPipe", 0, st);
}
void FeedDispatchNewI::RemoveFeed(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::RemoveFeed --> source:" << source << ", stype:"<<stype << ", actor:"<<actor);
  TaskManager::instance().execute(new RemoveFeedBySourceStypeActorTask(source,
        stype, actor));
}
void FeedDispatchNewI::RemoveFeedReply(Ice::Long source, Ice::Int stype,
    Ice::Int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply,
    const Ice::Current&) {

  MyUtil::LongSeq feedIds;

  FeedIndexSeq indexSeq =  GetFeedIndex(source,stype,actor);
  MCE_INFO("FeedDispatchNewI::RemoveFeedReply -->  source="<<source<<"  stype="<<stype<<" actor="<<actor<<" indexSeq size:"<<indexSeq.size());

  if (indexSeq.empty()) {
    MCE_WARN("FeedDispatchNewI::RemoveFeedReply --> get index empty source="<<source<<"  stype="<<stype<<" actor="<<actor);
    return;
  }
  if (!reply) {
    MCE_WARN("FeedDispatchNewI::RemoveFeedReply --> reply is NULL source:"<<source<<" stype:"<<stype<<" actor:"<<actor);
    return ;
  }
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    feedIds.push_back(indexSeq.at(i)->feed);
  }
  try {
    FeedDbWriterReplicaAdapter::instance().UpdateReply2DB(feedIds,reply);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedDispatchNewI::RemoveFeedReply --> call FeedDbWriter::UpdateReply2DB   err:"<<e);
  }
  try {
    for (size_t i = 0; i < feedIds.size(); i++) {
      FeedContentPtr content =
        FeedMemcClient::instance().GetFeed(feedIds.at(i));
      if (content) {
        content->reply = reply;
        FeedMemcClient::instance().SetFeed(content);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
  }
}

void FeedDispatchNewI::RemoveFeedReplyById(int stype, Ice::Long feedid,
    Ice::Long removeReplyId, const FeedReplyPtr& reply, const Ice::Current&) {
  MCE_INFO("FeedDispatchNewI::RemoveFeedReplyById ---> stype:" << stype << ",feedid:" << feedid);
  FeedIndexPtr feedIndex= DbHelper::instance().getFeedIndex(feedid);
  if (!feedIndex) {
    MCE_WARN("FeedDispatchNewI::RemoveFeedReplyById ---> can not get feed, stype:" << stype << "feedid:" << feedid);
    return;
  }

  LongSeq feedIds;
  try {
    feedIds.push_back(feedid);
    FeedDbWriterReplicaAdapter::instance().UpdateReply2DB(feedIds,reply);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedDispatchNewI::RemoveFeedReplyById --> call FeedDbWriter::UpdateReply2DB  err:"<<e);
  }
  try {
    for (size_t i = 0; i < feedIds.size(); i++) {
      FeedContentPtr content =
        FeedMemcClient::instance().GetFeed(feedIds.at(i));
      if (content) {
        content->reply = reply;
        FeedMemcClient::instance().SetFeed(content);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
  }

}

void FeedDispatchNewI::CacheSync(Ice::Long source, int stype, int actor,
    const Ice::Current &) {
  MCE_INFO("FeedDispatchNewI::CacheSync --> source = " << source << ",stype = " << stype << ",actor = " << actor);
  int r = FeedIndexRedisClient::instance().RemoveIndex(source, stype); 
  if(!r) {
    MCE_WARN("FeedDispatchNewI::CacheSync failed source:"<<source<<" stype:"<<stype<<" actor:"<< actor);
  }
}

Ice::Long FeedDispatchNewI::GetFeedId(Ice::Long source, int stype, int actor,
    const Ice::Current&) {
  FeedIndexSeq index_seq = GetFeedIndex(source,stype,actor);
  Ice::Long max_id = 0;
  for (size_t i = 0; i < index_seq.size(); ++i) {
    max_id = index_seq.at(i)->feed > max_id ? index_seq.at(i)->feed : max_id;
  }
  return max_id;
}


void FeedDispatchNewI::ReplaceXML(Ice::Long source, int stype, int actor,const string & xml, const Ice::Current& ){
  MCE_INFO("FeedDispatchNewI::ReplaceXML --> source = " << source << ",stype" << stype << " ,actor = " << actor);
  TaskManager::instance().execute(new ReplaceXMLTask(source, stype, actor,xml));
}

void FeedDispatchNewI::DirectedSend(Ice::Long source, int stype, int actor, const vector<int> & targets, const Ice::Current&){
  if(targets.empty()){
    MCE_WARN("FeedDispatchNewI::DirectedSend. targets empty. source:" << source << " stype:" << stype << " actor:" << actor);
    return;
  }
  FeedIndexSeq idxseq;
  //IndexLocator::LocateIndex(source,stype,actor,idxseq);
  idxseq =  GetFeedIndex(source,stype,actor);
  if(idxseq.empty()){
    MCE_WARN("FeedDispatchNewI::DirectedSend. find indexs fail. source:" << source << " stype:" << stype << " actor:" << actor);
    return;
  }
  FeedIndexPtr idx = idxseq.front();
  FeedItem item;
  item.actor = idx->actor;
  item.feed = idx->feed;
  item.merge = idx->newsMerge;
  item.time = idx->time;
  item.type = FeedMergeReplace << 30 | idx->stype;
  item.weight = 18 << 16;
  FeedConfigPtr config = new FeedConfig;
  config->updateTime = false;
  MyUtil::Int2IntMap uid2weight;
  for(vector<int>::const_iterator i = targets.begin(); i != targets.end();++i){
    uid2weight[*i] = 1;
  }
  FeedNewsReplicaAdapter::instance().addFeed(item,config,uid2weight);
  MCE_INFO("FeedDispatchNewI::DirectedSend. source:" << source << " stype:" << stype << " actor:" << actor
      << " targets size:" << targets.size() << " first target:" << targets.at(0));
}


bool FeedDispatchNewI::RefleshMiniGroupTime(const map<string,string> & config,const FeedReplyPtr & reply,const vector<long> & fids ,int actor) {
  if (config.empty()) {
    MCE_WARN("FeedDispatchNewI::RefleshMiniGroupTime. empty config");
    return false;
  }
  int gid ;
  map<string, string>::const_iterator i = config.find("minigroupid");
  if (i == config.end()) {
    gid = 0;
  } else {
    try {
      gid = boost::lexical_cast<long>(i->second);
    } catch (boost::bad_lexical_cast & e) {
      gid = 0;
    }
  }

  if (!gid) {
    MCE_WARN("FeedDispatchNewI::RefleshMiniGroupTime. fail Parse minigroupid");
    return false;
  }
  try {
    FeedGroupAdapter::instance().RefreshFeed(gid, fids.at(0), time(NULL));
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedDispatchNewI::RefleshMiniGroupTime. Fail RefreshFeed:"<<e);
  }

  try {
    if(!reply){
      MCE_INFO("FeedDispatchNewI::RefleshMiniGroupTime. reply empty. " );
      return true;
    }
    int replier = 0;
    if(!reply->newReply.empty()){
      replier = ReplyXmlParser::instance().ParseReplier(reply->newReply);
    }else  if(!reply->oldReply.empty()){
      replier = ReplyXmlParser::instance().ParseReplier(reply->oldReply);
    }else{
      MCE_WARN("FeedDispatchNewI::RefleshMiniGroupTime. new and old reply xml empty. " );
      return false;
    }
    if(!replier){
      MCE_INFO("FeedDispatchNewI::RefleshMiniGroupTime. replier empty. " );
      return true;
    }
    FeedGroupAdapter::instance().ReplyFeed(gid, fids.at(0), time(NULL),actor,replier);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedDispatchNewI::RefleshMiniGroupTime. Fail RefreshFeed:"<<e);
  }catch (std::exception& e) {
    MCE_WARN("FeedDispatchNewI::RefleshMiniGroupTime  err, " << e.what());
  }catch(...){
    MCE_WARN("FeedDispatchNewI::RefleshMiniGroupTime  err ...");
  }
  return true;
}




bool FeedDispatchNewI::IsNormalFeedType(int stype) {
  return !(_typeFilter.count(stype));
}

void FeedDispatchNewI::PrintThreadPoolInfoByLevel() { 
  std::map<int, int> taskPoolMap = TaskManager::instance().getLevel2Size();
  for(std::map<int,int>::iterator it = taskPoolMap.begin(); it != taskPoolMap.end(); it++) {
    int level = it->first;    
    size_t queueSize = it->second;
    size_t poolSize = TaskManager::instance().runningThreadSize(level);
    size_t waitSize = TaskManager::instance().waitThreadSize(level);
    size_t completedSize = TaskManager::instance().completedTaskSize(level);
    MCE_INFO("FeedDispatchI::printThreadPoolInfoByLevel --> level: " << level << " poolSize:" << poolSize << " queueSize:" << queueSize << " waitSize:" << waitSize << " completedSize:" << completedSize);
  }


}
FeedIndexSeq FeedDispatchNewI::GetFeedIndex(Ice::Long source,int stype,int actor) {
  FeedIndexSeq seq ;
  set<Ice::Long> filter;
  std::vector<FeedIndexRedisClient::IndexItem> value;
  int   r = xce::feed::FeedIndexRedisClient::instance().GetIndex(source,stype, value);
  for (size_t i = 0; i < value.size() && r; ++i ) {
    //过滤冗余索引
    if( filter.count(value[i].feedid) ) {
      continue;
    }
    filter.insert(value[i].feedid);

    FeedIndexPtr index = new FeedIndex();  
    index->feed = value[i].feedid;
    index->newsMerge = value[i].newsMerge; 
    index->miniMerge = value[i].miniMerge;
    index->source = value[i].source;
    index->psource= value[i].psource;
    index->actor  = value[i].actor;
    index->time   = value[i].time;
    index->stype  = value[i].stype;
    index->ptype  = value[i].ptype;
    //按actor过滤
    if( actor != -1 && index->actor != actor) {
      continue; 
    }
    seq.push_back(index);
  }
  MCE_INFO("FeedDispatchNewI::GetFeedIndex --> get index from redis source:"<<source<<" stype:"<<stype<<" actor:"<<actor<<" indexSeq size:"<<seq.size());
 
  if (seq.empty() ||NotNormalFeedType(stype) ) {
    timeval t1,t2;
    gettimeofday(&t1,NULL);
    seq = DbHelper::instance().getFeedIndex(stype,source,actor);
    if (seq.empty()) {
      seq = DbHelper::instance().getFeedIndexBC(stype,source,actor);
    } 
    gettimeofday(&t2,NULL);
    float tc = (t2.tv_sec - t1.tv_sec)*1000 - (t2.tv_usec - t2.tv_usec)*1.0 / 1000;
    MCE_INFO("FeedDispatchNewI::GetFeedIndex --> get index from db source:"<<source<<" stype: "<<stype<<" actor:"<<actor<<" indexSeq size:"<<seq.size()<<" timecost:"<<tc);
  }
  return seq;
}
FeedIndexPtr FeedDispatchNewI::GetFeedIndexById(int feed) {
  map<string,string> content;
  if (DbHelper::instance().isNewFeedidForDb(feed)) {
    content = DbHelper::instance().getFeedZoneContent(feed);
  } else {
    content = DbHelper::instance().getFeedContent(feed);
  }
  if (content.empty()) {
    return 0;
  }
  FeedIndexPtr index = new FeedIndex;
  try {
    index->feed = boost::lexical_cast<Ice::Long>(content["feed"]);
    index->source = boost::lexical_cast<Ice::Long>(content["source"]);
    index->stype = boost::lexical_cast<int>(content["small_type"]);
    index->actor = boost::lexical_cast<int>(content["actor"]);
    index->time = 0;
    index->psource = boost::lexical_cast<Ice::Long>(content["psource"]);
    index->ptype = boost::lexical_cast<int>(content["ptype"]);
    index->miniMerge = boost::lexical_cast<Ice::Long>(content["mini_merge"]);
    index->newsMerge = boost::lexical_cast<Ice::Long>(content["news_merge"]);

  } catch (std::exception & e) {
    MCE_WARN("FeedDispatchNewI::GetFeedIndexById --> boost lexical cast err:"<<e.what());
  } catch (...) {
    MCE_WARN("FeedDispatchNewI::GetFeedIndexById --> boost lexical cast ");
  }
  MCE_DEBUG("FeedDispatchNewI::GetFeedIndexById --> feed:"<<index->feed<<" source:"<<index->source<<" stype:"<<index->stype
      <<" actor:"<<index->actor<<" time:"<<index->time<<" psource:"<<index->psource<<" ptype:"<<index->ptype
      <<" newsMerge:"<<index->newsMerge<<" miniMerge:"<<index->miniMerge);
  return index;
}


void StatTimer::handle(){
  MCE_INFO("STAT:  thread_size:"<< TaskManager::instance().size(0) << ",ConfigCache size: " <<ConfigCache::instance().Size() );
}
