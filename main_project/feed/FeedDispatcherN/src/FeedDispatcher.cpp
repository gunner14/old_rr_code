#include "FeedDispatcher.h"
#include "FeedConfigAdapter.h"
#include "FeedDeliver.h"
#include "UserFeeds.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "FeedCacheNReplicaAdapter.h" 
#include "Date.h"
#include "util/cpp/String.h"
#include "ExpressionCache.h"
#include "TaskManager.h"
#include "WTransitAdapter.h"
#include "FriendRankCacheAdapter.h"
#include "UserStateAdapter.h"
#include "UserConfigAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <ostream>
#include <iostream>
#include <sstream>
#include "FeedNaiveBayesReplicaAdapter.h"
#include <FeedFocusInvertReplicaAdapter.h>
#include "../../FeedMemcProxy/client/feed_interaction_client_n.h"
#include "FeedMemcProxy/client/feed_memc_client.h"
#include "feed/Degrade/src/Degrade.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <exception>
#include <stdexcept>
#include <sys/resource.h>

using namespace xce::socialgraph;
using namespace xce::feed;
using namespace xce::adapter;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace boost;
using namespace ::xce::serverstate;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedDispatcherNI::instance(),
      service.createIdentity(FEED_DISPATCHER, ""));//catagory是空的

  TaskManager::instance().config(ThreadPoolConfig(1, 10));
  TaskManager::instance().config(10000,ThreadPoolConfig(1,1));
  TaskManager::instance().scheduleRepeated(new PrintTask());
  TaskManager::instance().scheduleRepeated(&LoadTask::instance());

  LoadTask::instance().handle();
  FeedDeliver::instance().initLogger();
  MCE_INFO("MyUtil::initialize. kFactorTypes " << kFactorTypes);

  //------------------For HA------------------ 
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt(
      "FeedDispatcher.Mod");
  int interval = 5;
  ServerStateSubscriber::instance().initialize("ControllerFeedDispatcherR",&FeedDispatcherNI::instance(), mod, interval, new XceFeedControllerChannel());


  // 注册降级模块 
  DegradeManager::instance().Register("FeedDispatcher::module_ConfigManager");
  DegradeManager::instance().Register("FeedDispatcher::module_BayesScores");
  DegradeManager::instance().Register("FeedDispatcher::module_WTransit");

  DegradeManager::instance().Register("FeedDispatcher::module_GuangJieExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_MiniSiteTagExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_CheWenExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_MiniSiteExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_XiaoZuExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_MiniGroupExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_PageExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_AppFansExpr");
  DegradeManager::instance().Register("FeedDispatcher::module_AppExpr");

  DegradeManager::instance().Register("FeedDispatcher::module_FiltFriend");
  // 然后初始化
  DegradeManager::instance().Init();
  
  
//  struct rlimit rl;
//  getrlimit(RLIMIT_CORE, &rl);
//  MCE_INFO("MyUtil::initialize:: --> orgin core limit:" << rl.rlim_max << " " << rl.rlim_cur);
//  rl.rlim_cur = RLIM_INFINITY;
//  setrlimit(RLIMIT_CORE, &rl);
//  getrlimit(RLIMIT_CORE, &rl);
//  MCE_INFO("MyUtil::initialize:: --> now core limit:" << rl.rlim_max << " " << rl.rlim_cur);

}

//--------------------------------------------------------------------------
FeedDispatcherNI::FeedDispatcherNI() {
}

void FeedDispatcherNI::dispatch(const FeedSeedPtr& seed,
    const FeedConfigPtr& config, const Ice::Current& cur) {
  if (!seed) {
    MCE_WARN("FeedDispatcherNI::dispatch --> seed is empty");
    return;
  }
  if (!config) {
    MCE_WARN("FeedDispatcherNI::dispatch --> config is empty");
    return;
  }
  MCE_INFO("dispatch --> feed: " << seed->feed << ",actor:"
      << seed->actor << ",stype:" << (seed->type & 0xffff) << ",source:" << seed->source
      << ",news_merge:" << seed->newsMerge << ",mini_merge:" << seed->miniMerge
      << ",expr:" << config->expr << ",sendConfig:" << config->sendConfig
      << ",newsSendConfig:" << config->sendNewsConfig << ",miniSendConfig:"
      << config->sendMiniConfig << " " << GetIp(cur));
//  if(seed->xml.empty()){
//    MCE_INFO("FeedDispatcherNI::dispatch. xml empty fid:" << seed->feed << " " << GetIp(cur));
//  }

  TaskManager::instance().execute(new DispatchTask(seed, config));
}

void FeedDispatcherNI::dispatchReply(const ReplyDataPtr& replyData,
    const FeedConfigPtr& config, const Ice::Current&) {
  if (!replyData) {
    MCE_WARN("FeedDispatcherNI::dispatchReply --> replyData is empty");
    return;
  }
  if (!config) {
    MCE_WARN("FeedDispatcherNI::dispatchReply --> config is empty");
    return;
  }
  
  MCE_INFO("dispatcherReply --> actor" << replyData->actor << ",stype"
      << replyData->stype << ",source:" << replyData->source << ",reply_count:"
      << replyData->replyCount << ",reply_id" << replyData->replyId << ",expr:"
      << config->expr);

  TaskManager::instance().execute(new DispatchReplyTask(replyData, config));
}

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
    throw logic_error("FeedDispatcherNI::Reload ");
  }
  oss << "(" << id << ")";
  return oss.str();
}
void FeedDispatcherNI::Reload(LoadType type, int id, const Ice::Current&) {
  string expr = MakeExpression(type, id);
  MCE_INFO("FeedDispatcherNI::Reload --> factor:" << expr );
  ExprCache::instance().InvalidateFactor(expr);
}
//shilong.li 2012-07-23
void FeedDispatcherNI::Reload2(LoadType type, long id, const Ice::Current&) {
/*
  string expr = MakeExpression(type,id);
  MCE_INFO("FeedDispatcherNI::Reload2 --> factor:"<< expr);
  ExprCache::instance().InvalidateFactor(expr);
  */
}
void FeedDispatcherNI::ReloadBatch(LoadType type, const vector<int> & ids,
    const Ice::Current&) {
  for (unsigned i = 0; i != ids.size(); ++i) {
    string expr = MakeExpression(type, ids.at(i));
    MCE_INFO("FeedDispatcherNI::ReloadBatch --> factor:" << expr );
    ExprCache::instance().InvalidateFactor(expr);
  }
}

void FeedDispatcherNI::addBuddyNotify(int host, int guest, const Ice::Current&) {
  MCE_INFO("FeedDispatcherNI::addBuddyNotify. host:" << host << " guest:" << guest);
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
void FeedDispatcherNI::joinPageNotify(int page, int fans, const Ice::Current&) {
  MCE_INFO("FeedDispatcherNI::joinPageNotify. page:" << page << " fans:" << fans);
  AppendNewsFeed(page, fans, 5, "joinPageNotify");
}

void FeedDispatcherNI::guideBuddyReplyNotify(int newUser, int buddy,
    const Ice::Current& current) {
  MCE_INFO("FeedDispatcherNI::guideBuddyReplyNotify --> newUser:"<<newUser << ", buddy:"<<buddy << " " << current.con->toString());
	char * funcname = "guideBuddyReplyNotify";
  AppendNewsFeed(buddy, newUser, 600, 1, funcname);//日志
  AppendNewsFeed(buddy, newUser, 700, 2, funcname);//照片
  AppendNewsFeed(buddy, newUser, 500, 1, funcname);//状态类
  AppendNewsFeed(buddy, newUser, 100, 2, funcname);//分享
}

void FeedDispatcherNI::buddyApplyNotify(int host, int guest,
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
  MCE_INFO("FeedDispatcherNI::buddyApplyNotify --> host:" << host << " guest:" << guest);
	char * funcname = "buddyApplyNotify";
  AppendNewsFeed(guest, host, 600, 1, funcname);//日志
  AppendNewsFeed(guest, host, 700, 2, funcname);//照片
  AppendNewsFeed(guest, host, 500, 1, funcname);//状态类
  AppendNewsFeed(guest, host, 100, 2, funcname);//分享 

}

void FeedDispatcherNI::addMiniSiteFeed(int siteid, int userid, const Ice::Current& current ) {
	static const int limit = 5;
	static const int timeval = 301;  //直接硬编码时间，如果FeedNews逻辑修改，此处需要修改
	MCE_INFO("FeedDispatcherNI::addMiniSiteFeed siteid: " << siteid << " userid: " << userid << " limit: " << limit<<" "<<current.con->toString());

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
		MCE_WARN("FeedDispatcherNI::addMiniSiteFeed -> FeedItemCacheByUseridReplicaAdapter::get Result is EMPTY. siteid: " << siteid << " limit: " << limit);
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
	MCE_INFO("FeedDispatcherNI::addMiniSiteFeed -> Dispatcher source feedids: " << os.str());

	FeedContentDict dict;
	try{
		dict = FeedMemcClient::instance().GetFeedDict(feedids);
	}catch(Ice::Exception& e){
		MCE_WARN("FeedDispatcherNI::addMiniSiteFeed --> err:"<<e << ", file:"<<__FILE__ << ", line:"<<__LINE__ << " uid:" << siteid); 
	}
	if(dict.empty()){
		MCE_WARN("FeedDispatcherNI::addMiniSiteFeed -> FeedMemcClient::GetFeedDict Result is EMPTY.");
		return ;
	}

	for(FeedMetaSeq::iterator it = items.begin(); it != items.end(); ++it) {

		FeedContentDict::iterator findit = dict.find(it->feedid);
		long source = 0;
		if (findit == dict.end()) {
			ostringstream os;
      string db_instance;
      if ( isNewFeedidForDb(it->feedid)) {
         db_instance = "feed_content_db";
         if (it->feedid > 0) {
           os<< "feed_content_" << it->feedid % 100 << "_" << it->feedid/DB_CONTENT_ZONE;
         } else {
           os<< "feed_content_bc_" << abs(it->feedid)%100 << "_" << abs(it->feedid)/DB_CONTENT_ZONE;
         }
      } else {
         db_instance = "feed_db";
         os<<"feed_content_"<<it->feedid % 100;
      }
			Statement sql;
			sql << "select source from " << os.str() << " where feed = " << it->feedid;
			mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer, os.str()).store(sql);
			if (res.num_rows() <= 0) {
				MCE_INFO("FeedDispatcherNI::addMiniSiteFeed Not Found FeedContent. feedid: " << it->feedid);
				continue ;
			}
			mysqlpp::Row row = res.at(0);
			source = row["source"];
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
		dispatch(seed, config);
	} 
}

void FeedDispatcherNI::addMiniSiteFeedToGuides(long source,int type,int siteid, const MyUtil::IntSeq& guides, const Ice::Current& current) {
	static const int limit = 200;
	static const int timeval = 301;  //直接硬编码时间，如果FeedNews逻辑修改，此处需要修改
  ostringstream og;
  og<<"(";
  for(size_t i = 0; i < guides.size(); ++i) {
    og<<guides[i]<<" ";
  }
  og<<")";
	MCE_INFO("FeedDispatcherNI::addMiniSiteFeedToGuides --> source:" << source << " type:"<< type <<" siteid:"<< siteid << " guides:"<<og.str()<<" "<<current.con->toString());

  FeedConfigPtr config = new FeedConfig();
  config->sendConfig = 0;
  config->sendMiniConfig = 0;
  config->sendNewsConfig = 1;
  config->updateTime = 1;

	IntSeq siteseq;
	siteseq.push_back(siteid);
  IntSeq typeseq;
  typeseq.push_back(type);
	FeedMetaSeq items = FeedItemCacheByUseridReplicaAdapter::instance().getByTypes(siteseq, 0, limit,typeseq);

	if (items.empty()) {
		MCE_WARN("FeedDispatcherNI::addMiniSiteFeedToGuides --> FeedItemCacheByUseridReplicaAdapter::getByTypes Result is EMPTY. siteid:"<< siteid<<" type:"<<type );
		return ;
	}

	MyUtil::LongSeq feedids;
	feedids.reserve(items.size());
	for(FeedMetaSeq::iterator it = items.begin(); it != items.end(); ++it) {
		feedids.push_back(it->feedid);
	}

	ostringstream os;
	os << "( ";
	for(FeedMetaSeq::iterator it = items.begin(); it != items.end(); ++it) {
		feedids.push_back(it->feedid);
		os << it->feedid << " ";
	}
	os << ")";
	MCE_INFO("FeedDispatcherNI::addMiniSiteFeedToGuides --> Dispatcher source feedids:"<< os.str());

	FeedContentDict dict;
	try{
		dict = FeedMemcClient::instance().GetFeedDict(feedids);
	}catch(Ice::Exception& e){
		MCE_WARN("FeedDispatcherNI::addMiniSiteFeedToGuides --> err:"<<e << " file:"<<__FILE__ << " line:"<<__LINE__ << " uid:" << siteid); 
	}
	if(dict.empty()){
		MCE_WARN("FeedDispatcherNI::addMiniSiteFeedToGuides --> FeedMemcClient::GetFeedDict Result is EMPTY.");
		return ;
	}

	for(FeedMetaSeq::iterator it = items.begin(); it != items.end(); ++it) {

		FeedContentDict::iterator findit = dict.find(it->feedid);
		long feed_source = 0;
		if (findit == dict.end()) {
			ostringstream os;
      string db_instance;
    if ( isNewFeedidForDb(it->feedid)) {
         db_instance = "feed_content_db";
         if (it->feedid > 0) {
           os<< "feed_content_" << it->feedid % 100 << "_" << it->feedid/DB_CONTENT_ZONE;
         } else {
           os<< "feed_content_bc_" << abs(it->feedid)%100 << "_" << abs(it->feedid)/DB_CONTENT_ZONE;
         }
      } else {
         db_instance = "feed_db";
         os<<"feed_content_"<<it->feedid % 100;
      }
			Statement sql;
			sql << "select source from " << os.str() << " where feed = " << it->feedid;
			mysqlpp::StoreQueryResult res = QueryRunner(db_instance, CDbRServer, os.str()).store(sql);
			if (res.num_rows() <= 0) {
				MCE_INFO("FeedDispatcherNI::addMiniSiteFeedToGuides --> Not Found FeedContent. feedid:" << it->feedid);
				continue ;
			}
			mysqlpp::Row row = res.at(0);
			feed_source = row["source"];
		} else {
			feed_source = findit->second->data->source;
		}
    MCE_INFO("addMiniSiteFeedToGuides --> source:"<<source<<"  feed_source:"<<feed_source)
    if ( source != feed_source ) {
      continue;
    } else { // 满足条件,向guides们分发此新鲜事 
		  FeedSeedPtr seed = new FeedSeed;
      seed->feed = it->feedid;
      seed->actor = it->userid;
      seed->baseWeight = it->weight;
      seed->miniMerge = it->mergeid;
      seed->newsMerge = it->mergeid;
      seed->psource = 0;
      seed->ptype = 0;
      seed->source = feed_source;
      seed->time = it->time;
      seed->type = it->type;
      seed->extraProps.insert(make_pair("MachineDispatch", "addMiniSiteFeed"));  //由机器分发统一添加youjiqifenfa属性，并在Value中指定函数名

      ostringstream expr;
      for (size_t i = 0; i < guides.size() && i < 2000; ++i) {
        FeedCacheNReplicaAdapter::instance().get(guides[i]);// 推送新鲜事之前,先将guide用户加载到FeedCache中,避免出现向guide用户分发新鲜事丢失的情况
        expr << "u(" << guides[i] << ")";
        config->expr = expr.str();
        dispatch(seed, config);
        expr.str("");
        MCE_INFO("FeedDispatcherNI::addMiniSiteFeedToGuides --> dispatch feed:"<<seed->feed<<" to guide:"<<guides[i]);
      }
    }
  } 
}


void FeedDispatcherNI::AppendNewsFeed(int from, int to, int limit, char * funcname) {
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
			MCE_WARN("FeedDispatcherN::AppendNewsFeed -> funcname is NULL. feedid: " << seed->feed << " actor: " << seed->actor);  //由机器构造函数必需传入调用函数名称
		}
    dispatch(seed, config);
  }
}

void FeedDispatcherNI::AppendNewsFeed(int from, int to, int type, int limit, char * funcname) {
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
    seed->time = (time_t) mysqlpp::DateTime(row["time"]);
    seed->type = row["type"];

		if (NULL != funcname) {
			seed->extraProps.insert(make_pair("MachineDispatch", funcname));  //由机器分发统一添加youjiqifenfa属性，并在Value中指定函数名
		} else {
			MCE_WARN("FeedDispatcherN::AppendNewsFeedWithType -> funcname is NULL. feedid: " << seed->feed << " actor: " << seed->actor);  //由机器构造函数必需传入调用函数名称
		}
    dispatch(seed, config);
  } 
}

//--------------------------------------------------------------------------
int DispatchTask::GetAppId(){
  MyUtil::Str2StrMap::iterator i = feed_->extraProps.find("AppId");
  if(i == feed_->extraProps.end()){
    return 0;
  }
  int app_id = 0;
  try{
    app_id = lexical_cast<int>(i->second);
  } catch (Ice::Exception& e) {
    MCE_WARN("DispatchTask::GetAppId exception :" <<e);
    return 0;
  } catch (std::exception& e) {
    MCE_WARN("DispatchTask::GetAppId exception :" << e.what());
    return 0;
  }
  return app_id;
}

void PrintProp(int actor,const MyUtil::Str2StrMap & props){
  ostringstream oss;
  oss << "PrintProp uid:" << actor;
  for(Str2StrMap::const_iterator i = props.begin(); i != props.end();++i){
    oss << " " << i->first << ":" << i->second;
  }
  MCE_INFO(oss.str());
}

bool IsTester(Ice::Int uid){
  return (uid == 347348947 || uid == 242332421 || uid == 220678672);
}

bool IsTestToId(int uid){
	return 		uid == 239556923||
			uid == 		221489137||
			uid == 		238489851||
			uid == 		240599893||
			uid == 		128487631||
			uid == 	225092473||
			uid == 		241610785||
			uid == 	220678672||
			uid == 	322856518||
			uid == 	34344507||
			uid == 	343518722||
			uid == 	347348947||
			uid == 	347349486||
			uid == 	200000032;
}
map<int,int> DispatchTask::GetBayesScores(int actor,const set<int> & target,int stype){
	map<int, int> scores;
  if (target.empty()) {
    return scores;
  }

  map<int,int> scores_tmp;
  vector<int> target_v;
  if (target.size() < 5000 ) {
    for (set<int>::iterator it = target.begin(); it != target.end(); ++it) {
      target_v.push_back(*it);
    }
    try {
    scores = FeedNaiveBayesReplicaAdapter::instance().GetScoreDict(actor,
        target_v, stype);
    } catch (Ice::Exception& e) {
      MCE_WARN("DispatchTask::GetBayesScores  call Bayes Exception"
          << " fid:" << feed_->feed << " actor:" << feed_->actor
          << " error:  "<< e);
    }
    return scores;
  } else {
    int i = 1;
    for (set<int>::iterator it = target.begin(); it != target.end(); ++it,++i) {
      target_v.push_back(*it);
      map<int,int> scoresPart;
      if (target_v.size() == 5000 || i == target.size()) {
        try {
         scoresPart = FeedNaiveBayesReplicaAdapter::instance().GetScoreDict(actor,
             target_v,stype);
        } catch (Ice::Exception& e) {
          MCE_WARN("DispatchTask::GetBayesScores  call Bayes Exception"
              << " fid:" << feed_->feed << " actor:" << feed_->actor
              << " error:  "<< e);
        }
        for (map<int,int>::iterator it = scoresPart.begin(); it != scoresPart.end(); ++it ) {
          scores.insert(map<int,int>::value_type(it->first,it->second));
        }
        scoresPart.clear();
        target_v.clear();
      }
    }
    return scores;
  }
  return scores;
}

void DispatchTask::GetFocusSource(const FeedSeedPtr & seed,map<int,int> & rank){
	vector<int> sources;
	int actor = seed->actor;
	int stype = seed->type & 0xFFFF;
	long fid = seed->feed;
	int oldsz = rank.size();
	TimeStat ts;
	try{
		sources = FeedFocusInvertReplicaAdapter::instance().GetSources(actor);
		//sources = FeedFocusInvertAdapter::instance().GetSources(actor);
	} catch (Ice::Exception& e) {
		MCE_WARN("DispatchTask::GetFocusSource. actor:" << actor << " fid:" << fid << " stype:" << stype << " error:  "<< e);
		return;
	}catch(...){
		MCE_WARN("DispatchTask::GetFocusSource. actor:" << actor << " fid:" << fid << " stype:" << stype << " unknown error" );
		return;
	}
	BOOST_FOREACH(int source,sources){
		rank[source] = 100;
	}
	MCE_INFO("DispatchTask::GetFocusSource. actor:" << actor << " fid:" << fid << " stype:" << stype
			<< " sources:" << sources.size() << " oldrank:" << oldsz << " newranksize:" << rank.size()
			<< " cost:" << ts.getTime()
	);
}

void ChangeExpr(int actor,int stype,string & expr){
	if(!(stype == 2901 || stype == 2902 || stype == 2906)){
		return;
	}
	int cnt = count(expr.begin(),expr.end(),'f');
	if(cnt == 1){
		ostringstream oss;
		oss << "&f(" << actor << ")";
		expr += oss.str();
	}
}
void DispatchBigFansSizeFeedTask::handle() {
  map<int, int> scores;
  vector<int> target_v;
  timeval begin,end;
  gettimeofday(&begin,NULL); 
  int i = 1;
  bool isEnable = DegradeManager::instance().IsEnable("FeedDispatcher::module_BayesScores");
  for (set<int>::iterator it = target_.begin(); it != target_.end() && isEnable; ++it,++i) {
    target_v.push_back(*it);
    map<int,int> scoresPart;
    if (target_v.size() == 5000 || i == target_.size()) {
      try {
       scoresPart = FeedNaiveBayesReplicaAdapter::instance().GetScoreDict(feed_->actor,
           target_v,feed_->type & 0xffff);
      } catch (Ice::Exception& e) {
        MCE_WARN("DispatchBigFansSizeFeedTask::GetBayesScores  call Bayes Exception"
            << " fid:" << feed_->feed << " actor:" << feed_->actor
            << " error:  "<< e);
      }
      for (map<int,int>::iterator it = scoresPart.begin(); it != scoresPart.end(); ++it ) {
        scores.insert(map<int,int>::value_type(it->first,it->second));
      }
      scoresPart.clear();
      target_v.clear();
    }
  }

  gettimeofday(&end,NULL); 
  float tc1 = (end.tv_sec - begin.tv_sec) * 1000 + (end.tv_usec - begin.tv_usec)*1.0 / 1000;

  DispatchCost cost;
  FeedDeliver::instance().deliver(feed_, config_, target_,scores,cost);
  InteractionUploader::instance().UploadRange(feed_->feed,target_.size());
  gettimeofday(&begin,NULL); 
  float tc2 = (begin.tv_sec - end.tv_sec) * 1000 + (begin.tv_usec - end.tv_usec)*1.0 / 1000;
  MCE_INFO("DispatchBigFansSizeFeedTask::handle --> source:"<<feed_->source<<" stype:"<<(feed_->type & 0xffff)<<" actor:"<<
      feed_->actor<<" target size:"<<target_.size()<<" caculate byes score cost:"<<tc1<<" deliver cost:"<<tc2);
}


void DispatchTask::handle () {
  DispatchCost cost;

  TimeStat t1,t2;
  int stype = feed_->type & 0x0000ffff;
  int bigtype = stype / 100;//
  int appid = feed_->newsMerge;//对于特定的type，newsMerge代表appid

  if(stype == 8190){ //对于81XX的新鲜事，psource为appid
    appid = GetAppId();
  }

  if (config_->sendConfig) { //使用数据库里，用户的设置
    try {
      if (DegradeManager::instance().IsEnable("FeedDispatcher::module_ConfigManager")) {  
         bool p = ConfigManagerAdapter::instance().GetSendConfig(//现在的SendConfig中只需要查询news的设置
             feed_->actor, bigtype, appid);
         if (!p) {
           config_->sendNewsConfig = p;
           config_->sendMiniConfig = p;//根据产品的需求，miniconfig和newsconfig相等
         }
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("DispatchTask::handle.ConfigManagerAdapter::instance().GetSendConfig().uid:<< "<< feed_->actor << " fid:" << feed_->feed << "exception "<<e);
    }
  }
  cost.send_cfg_ = t1.getTime(true);

  set<int> target;
  if (config_->sendNewsConfig) { //需要发送News
    //    if (1) { //TODO del
    MCE_DEBUG("DispatchTask::handle --> sendNewsConfig = true");
    //2
    string oldexpr = config_->expr;
//    ChangeExpr(feed_->actor,stype,config_->expr);
//    MCE_INFO("DispatchTask::handle --> ChangeExpr. fid:" << feed_->feed << " actor = " << feed_->actor << " oldexpr:" << oldexpr << " newexpr:" << config_->expr);
    try {
      target = ExprCache::instance().Expr2Ids(config_->expr, feed_->actor);
      MCE_DEBUG("DispatchTask::handle --> After ExprCache.Expr2Ids ,the target : size = " << target.size());
    } catch (Ice::Exception& e) {
      MCE_WARN("DispatchTask::handle --> ExprCache::instance().Expr2Ids() --> exception :" <<e);
    } catch (std::exception& e) {
      MCE_WARN("DispatchTask::handle --> ExprCache::instance().Expr2Ids() --> exception :" << e.what());
    }
    cost.expr_ = t1.getTime(true);
    if (config_->sendMiniConfig) {
      int stype = feed_->type & 0xFFFF;
      if (!(stype >= 2901 && stype <= 2904)) {
        target.insert(feed_->actor);
      }
    }
    //(feed_->actor>600000000&&feed_->actor<700000000)
    //3
    //target id可能有很多，分批发送
    const static unsigned int kMaxSize = 5000;
    vector<int> in, partIn, partOut;//in为全部的target，partIn为分批发送的target，大小最大为kMaxSize,
    //partOut为部分的返回的不需要发送target
    in.assign(target.begin(), target.end());
    int i = 0;//表示第几批
    size_t not_recv_cnt = 0;
    /**********************************add by shilong.li*/
    //判断待分发的新鲜事 是否是小站通过addMiniSiteFeed或addMiniSiteFeedToGuides 接口向指定用户推送,
    // 如果是跳过ConfigManager::FilterRevConfig这块的逻辑
    bool isMachineDispatch = false;
    ::MyUtil::Str2StrMap::iterator expt = feed_->extraProps.find("MachineDispatch");
    if ( (expt != feed_->extraProps.end()) && expt->second == "addMiniSiteFeed" ) {
      isMachineDispatch = true;
      MCE_INFO("DispatcherTask::handle --> addMiniSiteFeed dispach feed actor:"<<feed_->actor<<" feedid:"<<feed_->feed<<" skip ConfigManager::FilterRecvConfig()  ");
    }
    /*******************************/
    bool isEnable =DegradeManager::instance().IsEnable("FeedDispatcher::module_ConfigManager");
    for (vector<int>::iterator vit = in.begin(); vit < in.end()&& (!isMachineDispatch) && isEnable; vit
        += kMaxSize, ++i) {
      MCE_DEBUG("DispatchTask::handle --> INVOKE --> ConfigManagerAdapter::instance().FilterRecvConfig --> part :" << i);
      partIn.assign(vit, vit + kMaxSize < in.end() ? vit + kMaxSize : in.end());
      try {
        partOut = ConfigManagerAdapter::instance().FilterRecvConfig(partIn,
            bigtype, appid);
        // MCE_DEBUG("PartIn size = " << partIn.size() << ", PartOut ids size = " << partOut.size()<< ", bigtype:"<<bigtype << ",appid:" << appid);
        not_recv_cnt += partOut.size();
        for (vector<int>::iterator vit = partOut.begin(); vit != partOut.end(); ++vit) {
          target.erase(*vit);
        }
      } catch (Ice::Exception& e) {
        MCE_WARN("DispatchTask::handle --> Invoke --> ConfigManagerAdapter::instance().uid : "<< feed_->actor << " fid:" << feed_->feed <<" exception "<<e);
      }
    }
    cost.recv_cfg_ = t1.getTime(true);
    MCE_INFO("DispatchTask::handle --> feedId = " << feed_->feed << ",actor = " << feed_->actor <<
        ",expression = " << config_->expr << ", filter ids size = " << not_recv_cnt << ", result size : " << target.size()
        << " appid:" << appid);
  } else {
    MCE_DEBUG("DispatchTask::handle. sendNewsConfig is false");
  }
  int limitThreshold = 100000;
  map<int,int> scores;
  if (target.size() < limitThreshold) {
    if (DegradeManager::instance().IsEnable("FeedDispatcher::module_BayesScores")) {
      scores = GetBayesScores(feed_->actor, target, stype);
    }
  } else {
    TaskManager::instance().execute(new DispatchBigFansSizeFeedTask(feed_,config_,target));
    return;
  }
  FeedDeliver::instance().deliver(feed_, config_, target,scores,cost);
  
  cost.total_ = t2.getTime();
  InteractionUploader::instance().UploadRange(feed_->feed,target.size());
  MCE_INFO("DispatchTask::handle fid:"<< feed_->feed <<" source:" << feed_->source << " type:" << stype << " actor:" << feed_->actor<<
      " expr:"<<config_->expr<<" target size:"<<target.size()<<" #cost# " << cost.ToString());
}
//--------------------------------------------------------------------------------------------------
void DispatchReplyTask::handle() {
  if (reply_->actor > 600000000 && reply_->actor < 700000000) {
    MCE_INFO("DispatchReplyTask::handle. not send page reply to IM. source:" << reply_->source << " stype:" << reply_->source << " actor:" << reply_->actor);
    return;
  }
  TimeStat t1;
  //计算表达式
  set<int> target;
  try {
    target = ExprCache::instance().Expr2Ids(config_->expr, reply_->actor);
  } catch (Ice::Exception& e) {
    MCE_WARN("DispatchReplyTask::handle --> ExprCache::instance().Expr2Ids() --> exception :" <<e);
  } catch (std::exception& e) {
    MCE_WARN("DispatchReplyTask::handle --> ExprCache::instance().Expr2Ids() --> exception :" << e.what());
  }
  MCE_INFO("DispatchReplyTask::handle --> expr:" << config_->expr << " size:" << target.size());

  //发送给WTransit
  //  try {
  //    MyUtil::IntSeq target_ids;;
  //    target_ids.assign(target.begin(), target.end());
  //    target_ids.push_back(reply_->actor);//把自己也加入

  MyUtil::IntSeq to_ids;
  to_ids.push_back(reply_->actor);
  for (set<int>::iterator it = target.begin(); it != target.end();) {

    to_ids.push_back(*it++);
    if (to_ids.size() == 40000 || it == target.end()) {
      try {
        if ( DegradeManager::instance().IsEnable("FeedDispatcher::module_WTransit") ) {
          talk::adapter::WTransitAdapter::instance().deliverFeedReply(reply_,
             to_ids);
        }
        MCE_INFO("DispatchReplyTask::handle --> Reply to IM, actor:"<< reply_->actor << ",source:" <<reply_->source << ",stype:"<< reply_->stype << ", toid.size:"<<to_ids.size());
        to_ids.clear();
      } catch (Ice::Exception & e) {
        MCE_WARN("DispatchReplyTask::handle --> call WTransit err:"<<e);
        to_ids.clear();
      }
    }
  }

  //  string e = config_->expr.empty() ? string(" NULL"):string(config_->expr.begin(), config_->expr.begin() +1);
  //  e = "R_" + e;
  //  StatTimer::instance().Update(reply_->actor,e,t1.getTime(),target.size());

  //  } catch (Ice::Exception& e) {
  //    MCE_WARN("DispatchReplyTask::handle  Exception:  "<<e);
  //  }

}

//string StatTimer::Dump(){
//  map<string,Record> stat;
//  stat.swap(stat_);
//  ostringstream oss;
//  oss << "index:" << idx_;
//  long targetall = 0,countall = 0;;
//  for(map<string,Record>::iterator i = stat.begin(); i != stat.end();++i){
//    oss << " #type#:" << i->first << " count:" << i->second.count_ << " time:" << i->second.time_ << " target:" << i->second.target_;
//    targetall += i->second.target_;
//    countall += i->second.count_;
//  }
//  oss << " countall:" << countall << " targetall:" << targetall;
//  return oss.str();
//}
string DispatchCost::ToString() {
  ostringstream oss;
//  float send_cfg_,  expr_, recv_cfg_, friend_,  mini_,  log, wtran,news_,interaction,photo;
  oss << " send_cfg:" << send_cfg_ << " expr:" << expr_ << " recv_cfg:" << recv_cfg_ << " friend:" << friend_
      << " mini:" << mini_ << " log:" << log_ << " wtran:" << wtran_ << " news:" << news_
      << " interaction:" << interaction_ << " photo:" << photo_ << " total:" << total_ << " range:" << range_;
  return oss.str();
}


void InteractionUploader::UploadRange(long fid,int r){
	FeedInterPtr inter(new FeedInter);
	inter->SetRange(r);
	try{
		FeedInteractionMemcClientN::instance().SetFeedInter(fid,inter);
  }catch (Ice::Exception& e) {
    MCE_WARN("InteractionUploader::UploadRange. fid:" << fid<< " range:" << r << " err:"<<e);
  } catch (std::exception& e) {
    MCE_WARN("InteractionUploader::UploadRange. fid:" << fid<< " range:" << r << " err:"<<e.what());
  }
}

void InteractionUploader::UploadReply(long fid,int r){
	set<long> fids;
	fids.insert(fid);
	FeedInterMap intermap;
	FeedInteractionMemcClientN::instance().GetFeedInterMap(fids,intermap);
	FeedInterPtr inter;
	FeedInterMap::iterator it = intermap.find(fid);
	if(it != intermap.end() ){
		inter = it->second;
	}
	if(!inter){
		inter = shared_ptr<FeedInter>(new FeedInter);
	}
	inter->SetReply(r);
	FeedInteractionMemcClientN::instance().SetFeedInter(fid,inter);
}

