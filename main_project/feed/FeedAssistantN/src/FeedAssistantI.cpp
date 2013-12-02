/*
 * FeedAssistantI.cpp
 *
 *  Created on: Dec 24, 2009
 *      Author: antonio
 */

#include "Tasks.h"
#include "ServiceI.h"
#include "FeedAssistantI.h"
#include "DbHelper.h"
#include "FeedContentAdapter.h"
#include "FeedAssistantReplicaAdapter.h"
#include "FeedDispatcherReplicaAdapter.h"
#include "PerformanceStat.h"
#include "FeedInteractionNAdapter.h"
#include "FeedNewsAdapter.h"
#include "FeedItemFacadeReplicaAdapter.h"
#include "Common.h"
#include "UserFeeds.h"
#include "FeedPipe.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "feed/Degrade/src/Degrade.h"

#include <sys/resource.h>
using namespace com::xiaonei::xce;
using namespace xce::feed;
using namespace std;
using namespace MyUtil;
using namespace ::xce::serverstate;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  
   service.getAdapter()->add(&FeedAssistantI::instance(),
      service.createIdentity(FEED_ASSISTANT, ""));

  TaskManager::instance().scheduleRepeated(new StatTimer());
  TaskManager::instance().scheduleRepeated(&FeedReplyBufferTimer::instance());
  TaskManager::instance().scheduleRepeated(&PerformanceStat::instance());

  MyUtil::ThreadPoolConfig tpc(4, 4);
  TaskManager::instance().config(REMOVE_TASK_LEVEL, tpc);

  PipePool::instance().initialize(new PipeFactoryI);

	ReplyLogger::instance().Init();
 
  DB_INSTANCE = service.getCommunicator()->getProperties()->getPropertyWithDefault(
      "FeedDBInstance", "feed_db");
//------------------For HA------------------
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt(
      "FeedAssistantN.Mod");
  int interval = 5;
  ServerStateSubscriber::instance().initialize("ControllerFeedAssistantNR",&FeedAssistantI::instance(), mod, interval, new XceFeedControllerChannel());

// 注册降级模块 
  DegradeManager::instance().Register("FeedAssistantN::module_FeedItemFacade");
  DegradeManager::instance().Register("FeedAssistantN::module_FeedGroup");
// 然后初始化
  DegradeManager::instance().Init();
}
//---------------------------------------------------------------------------
void IndexCache::add(const FeedIndexPtr& index) {
  if (!index) {
    return;
  }

  //Refresh the container
  IceUtil::Mutex::Lock lock(_mutex);
  std::pair<IndexContenter::iterator, bool> p = _container.push_front(index); //push front

  if (!p.second) {
    _container.relocate(_container.begin(), p.first); /* put in front */
    return;
  } else {
    _kick();
  }
}
void IndexCache::add(const FeedIndexSeq& indexSeq) {
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    add(indexSeq.at(i));
  }
}
void IndexCache::removeFeedByIds(const MyUtil::LongSeq& feeds) {
  MCE_INFO("IndexCache::removeFeedByIds --> feeds size = " << feeds.size());
  IceUtil::Mutex::Lock lock(_mutex);

  IdIndex& index = _container.get<1> ();

  for (size_t i = 0; i < feeds.size(); ++i) {
    index.erase(feeds.at(i));
  }
}

void IndexCache::removeFeedBySourceStypeActor(Ice::Long source, int stype,
    int actor) {
  IceUtil::Mutex::Lock lock(_mutex);
  SourceStypeActorIndex& index = _container.get<2> ();
  pair<SourceStypeActorIndex::iterator, SourceStypeActorIndex::iterator> p =
      index.equal_range(boost::make_tuple(source, stype, actor));

  while (p.first != p.second) {
    p.first = index.erase(p.first);
  }

}

FeedIndexPtr IndexCache::getFeedIndex(Ice::Long feedid) {
  IceUtil::Mutex::Lock lock(_mutex);
  IdIndex& index = _container.get<1> ();
  IdIndex::iterator it = index.find(feedid);
  if (it != index.end()) {
    return *it;
  }
  return 0;
}

FeedIndexSeq IndexCache::getFeedIndexSeq(Ice::Long source, Ice::Int stype,
    Ice::Int actor) {
  IceUtil::Mutex::Lock lock(_mutex);
  SourceStypeActorIndex& index = _container.get<2> ();

  pair<SourceStypeActorIndex::iterator, SourceStypeActorIndex::iterator> p;
  if (actor > 0) {
    p = index.equal_range(boost::make_tuple(source, stype, actor));
  } else {
    p = index.equal_range(boost::make_tuple(source, stype));
  }

  FeedIndexSeq seq;
  while (p.first != p.second) {
    seq.push_back(*p.first);
    ++p.first;
  }
  return seq;
}

int IndexCache::size() {
  IceUtil::Mutex::Lock lock(_mutex);
  return _container.size();
}

void IndexCache::_kick() {

  SeqIndex& index = _container.get<0> ();
  while (_container.size() > (size_t)CACHE_SIZE) {
    SeqIndex::iterator it = index.end();
    --it;
    Ice::Long source = (*it)->source;
    Ice::Int stype = (*it)->stype;
    Ice::Int actor = (*it)->actor;

    SourceStypeActorIndex & index2 = _container.get<2> ();
    pair<SourceStypeActorIndex::iterator, SourceStypeActorIndex::iterator> p =
        index2.equal_range(boost::make_tuple(source, stype, actor));
    index2.erase(p.first, p.second);
  }
}

//---------------------------------------------------------------------------

/*
 * remove need delete feed_index, feed_content, feed_mini
 *
 *
 */

void FeedAssistantI::removeFeedByTime(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Int time, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::removeFeedByTime --> source:" << source << " stype:" << stype << " actor:" << actor << " time:" << time);
  TaskManager::instance().execute(new RemoveFeedByTimeTask(source, stype, actor, time));
}


void FeedAssistantI::removeFeedById(Ice::Long feedId, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::removeFeedById --> feedid:" << feedId);
  TaskManager::instance().execute(new RemoveFeedByIdTask(feedId));
}
void FeedAssistantI::removeFeedBySource(Ice::Long source, Ice::Int stype,
    const Ice::Current&) {
  MCE_INFO("FeedAssistantI::removeFeedBySource --> source:"<<source << ", stype:"<<stype );
  //TaskManager::instance().execute(
   //   new RemoveFeedBySourceStypeTask(source, stype));

 //TaskManager::instance().execute(new MessageTask(seq));
  //MCE_DEBUG("DeliverManagerI::deliver --> seq.size="<<seq.size());
  RemoveKeyPtr st = new RemoveKey();
  st->source = source;
  st->type = stype;
  st->op = BySource;

  PipePool::instance().push("FeedPipe", 0, st);
  DbHelper::instance().removeFeedIndexBysource(source,stype);
}
void FeedAssistantI::removeFeedBySourceHS(Ice::Long source, Ice::Int stype,
    const Ice::Current&) {
  MCE_INFO("FeedAssistantI::removeFeedBySourceHS --> source:"<<source << ", stype:"<<stype );
  TaskManager::instance().execute(
      new RemoveFeedBySourceStypeHSTask(source, stype));
  // Db double write add code at 2021-11-12
  DbHelper::instance().removeFeedIndexBysource(source,stype);
}
void FeedAssistantI::removeFeedByParent(Ice::Long parent, Ice::Int ptype,
    const Ice::Current&) {
  MCE_INFO("FeedAssistantI::removeFeedByParent --> parent:"<<parent << ", ptype:"<<ptype);
  //TaskManager::instance().execute(new RemoveFeedByParentTask(parent, ptype));
  RemoveKeyPtr st = new RemoveKey();
  st->source = parent;
  st->type = ptype;
  st->op = ByParent;

  PipePool::instance().push("FeedPipe", 0, st);
  // Db double write add code at 2021-11-12
  DbHelper::instance().removeFeedIndexBysource(parent,ptype);
}
void FeedAssistantI::removeFeed(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::removeFeed --> source:" << source << ", stype:"<<stype << ", actor:"<<actor);
  TaskManager::instance().execute(new RemoveFeedBySourceStypeActorTask(source,
      stype, actor));
  // Db double write add code at 2021-11-12
  DbHelper::instance().removeFeedIndexBysource(source,stype,actor);
}

void FeedAssistantI::removeFeedReply(Ice::Long source, Ice::Int stype,
    Ice::Int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply,
    const Ice::Current&) {

  MCE_INFO("FeedAssistantI::removeFeedReply -->  source="<<source<<"  stype="<<stype<<" actor="<<actor);
  FeedIndexSeq indexSeq = IndexCache::instance().getFeedIndexSeq(source, stype,
      actor);
  MyUtil::LongSeq feedIds;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    feedIds.push_back(indexSeq.at(i)->feed);
  }
  if (indexSeq.empty()) {

    indexSeq = DbHelper::instance().getFeedIndex(stype, source, actor);
    if (indexSeq.empty()) {
      MCE_WARN("FeedAssistantI::removeFeedReply ---> can not get feed, source:"<<source<<" actor:"<<actor << " stype:"<<stype);
      return;
    }
    for (size_t h = 0; h < indexSeq.size(); ++h) {
      feedIds.push_back(indexSeq.at(h)->feed);
    }
    IndexCache::instance().add(indexSeq);
  }

  try {
    FeedReplyBufferTimer::instance().changeReply(feedIds, reply); 
    //FeedContentAdapter::instance().replaceFeedReply(feedIds, reply);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedAssistantI::_replaceFeedReply --> call FeedContentAdapter replaceFeedReply  err:"<<e);
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
  //todo  remove notify to IM
}

void FeedAssistantI::removeFeedReplyById(int stype, Ice::Long feedid,
    Ice::Long removeReplyId, const FeedReplyPtr& reply, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::removeFeedReplyById ---> stype:" << stype << ",feedid:" << feedid);
  FeedIndexPtr feedIndex = IndexCache::instance().getFeedIndex(feedid);
  if (!feedIndex) {
    feedIndex = DbHelper::instance().getFeedIndex(feedid);
    if (!feedIndex) {
      MCE_WARN("FeedAssistantI::removeFeedReplyById ---> can not get feed, stype:" << stype << "feedid:" << feedid);
      return;
    }
  }

  LongSeq feedIds;
  try {
    feedIds.push_back(feedid);
    FeedReplyBufferTimer::instance().changeReply(feedIds, reply); 
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedAssistantI::addFeedReplyById --> call FeedContentAdapter replaceFeedReply  err:"<<e);
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

void FeedAssistantI::readNews(Ice::Int user, Ice::Long feed,
    const Ice::Current&) {
  MCE_INFO("FeedAssistantI::readNews --> user:"<<user << " feedid:"<<feed);
  TaskManager::instance().execute(new ReadNewsTask(user, feed));
}
void FeedAssistantI::readAllNews(Ice::Int user, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::readAllNews --> user:"<<user);
  TaskManager::instance().execute(new ReadNewsTask(user));
}

void FeedAssistantI::readMini(Ice::Int user, Ice::Long feed,
    const Ice::Current&) {
  MCE_INFO("FeedAssistantI::readMini --> user:"<<user << " feedid:"<<feed);
  TaskManager::instance().execute(new ReadMiniTask(user, feed));
}
void FeedAssistantI::readAllMini(Ice::Int user, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::readAllMini --> user:"<<user);
  TaskManager::instance().execute(new ReadMiniTask(user));
}

void FeedAssistantI::dispatch(const FeedSeedPtr& seed, const Ice::Current&) {
  if (seed) {
    MCE_INFO("FeedAssistantI::dispatch --> feed:" << seed->feed << " actor:" << seed->actor << " stype:" << (seed->type&0xFFFF)
        << " source:" << seed->source << " mergetype:" << (((unsigned int)seed->type) >> 30));
    if (seed->feed <= 0) {
      MCE_WARN("FeedAssistantI::dispatch  feedid < 0");
      return;
    }
    TaskManager::instance().execute(new FeedDispatchTask(seed));//没有reply
    FeedIndexPtr index = new FeedIndex;
    index->feed = seed->feed;
    index->newsMerge = seed->newsMerge;
    index->miniMerge = seed-> miniMerge;
    index->source = seed->source;
    index->psource = seed->psource;
    index->actor = seed->actor;
    index->time = seed->time;
    index->stype = seed->type & 0xffff;
    index->ptype = seed->ptype;
    DbHelper::instance().addFeedIndexBysource(index);

  }
}

void FeedAssistantI::dispatchEDM(const FeedSeedPtr& seed, const Ice::Current&) {
  if (seed) {
    IceUtil::Mutex::Lock lock(_EDMMutexs[seed->source % EDM_MUTEX_COUNT]);

    unsigned int baseWeightId = 0;
    if (seed->extraProps.find("base_weight_id") != seed->extraProps.end()) {
      try {
        baseWeightId = boost::lexical_cast<int>(
            seed->extraProps["base_weight_id"]);
      } catch (std::exception & e) {
        MCE_WARN("FeedAssistantI::dispatchEDMFeed --> cast base_weight_id err:" << e.what());
      }
    }
    Ice::Long feedid = -1;
    FeedIndexSeq seq = IndexCache::instance().getFeedIndexSeq(seed->source,
        seed->type & 0xFFFF, -1);
    if (seq.empty()) {
      MCE_INFO("FeedAssistantI::dispatchEDMFeed --> not found in cache");
      seq = DbHelper::instance().getFeedIndexBySource(seed->type & 0xFFFF,
          seed->source);
      if (!seq.empty()) {
        feedid = seq.at(0)->feed;
        IndexCache::instance().add(seq);
      }
    }else{
			feedid = seq.at(0)->feed;
		}
    if (feedid < 0) {
      try {
        //feedid = FeedAssistantAdapter::instance().createFeedId();
        feedid = FeedAssistantReplicaAdapter::instance().createFeedId();
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedAssistantI::dispatchEDMFeed --> call FeedAssistant to get feed id err, " << e);
        return;
      }
      if (feedid < 0) {
        MCE_WARN("FeedAssistantI::dispatchEDMFeed --> feed less than 0");
        return;
      }
      seed->feed = feedid;
      MCE_INFO("FeedAssistantI::dispatchEDMFeed --> add to db source:" << seed->source << " feedid:"
          << seed->feed << " stype:" << (seed->type&0xFFFF) << " actor:" << seed->actor);
      if (seed->time < 0) {
        //if time less than 0, sql will fail
        seed->time = time(NULL);
      }
      FeedIndexPtr index = new FeedIndex;
      index->feed = seed->feed;
      index->newsMerge = seed->newsMerge;
      index->miniMerge = seed-> miniMerge;
      index->source = seed->source;
      index->psource = seed->psource;
      index->actor = seed->actor;
      index->time = seed->time;
      index->stype = seed->type & 0xffff;
      index->ptype = seed->ptype;

      DbHelper::instance().addFeedIndex(index);
      DbHelper::instance().addFeedIndexBysource(index);
      IndexCache::instance().add(index);
			if (DbHelper::instance().isNewFeedidForDb(seed->feed)) {
				DbHelper::instance().addZoneFeedContent(seed);
			} else {
				DbHelper::instance().addFeedContent(seed);
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
          MCE_INFO("FeedAssistantI::dispatchEDMFeed --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__);
        }
      } catch (std::exception& e) {
        MCE_WARN("FeedAssistantI::dispatchEDMFeed --> call Memc err, " << e.what());
      } catch (...) {
        MCE_WARN("FeedAssistantI::dispatchEDMFeed --> call Memc err, unknown");
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
    	FeedDispatcherReplicaAdapter::instance().dispatch(seed, config);
    	MCE_INFO("FeedAssistantI::dispatchEDMFeed --> dispatch feedid:" << seed->feed
          << " stype:" << (seed->type&0xFFFF) << "source:" << seed->source << " actor:" << seed->actor << " time:" << seed->time);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedAssistantI::dispatchEDMFeed --> dispatch err, " << e);
    } catch (...){
      MCE_WARN("FeedAssistantI::dispatchEDMFeed --> dispatch unknown err. line:" << __LINE__);
    }
  }
}

void FeedAssistantI::dispatchWithReply(const FeedSeedPtr& seed,
    const FeedReplyPtr& reply, const Ice::Current&) {
  if (seed) {
    MCE_INFO("FeedAssistantI::dispatchWithReply  --> feed:" << seed->feed << " actor:" << seed->actor << " stype:" << (seed->type&0xFFFF)
        << " source:" << seed->source << " mergetype:" << (((unsigned int)seed->type) >> 30));
    if (seed->feed <= 0) {
      MCE_WARN("FeedAssistantI::dispatchWithReply  feedid < 0");
      return;
    }
    TaskManager::instance().execute(new FeedDispatchTask(seed, reply));
    FeedIndexPtr index = new FeedIndex;
    index->feed = seed->feed;
    index->newsMerge = seed->newsMerge;
    index->miniMerge = seed-> miniMerge;
    index->source = seed->source;
    index->psource = seed->psource;
    index->actor = seed->actor;
    index->time = seed->time;
    index->stype = seed->type & 0xffff;
    index->ptype = seed->ptype;
    DbHelper::instance().addFeedIndexBysource(index);
  }

}
void FeedAssistantI::addFeedReply(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const FeedReplyPtr& reply, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::addFeedReply --> source = " << source << ",stype = " << stype << ",actor = " << actor 
    << " task size:" << TaskManager::instance().size(0) << " thread:" <<TaskManager::instance().runningThreadSize(0) 
    << " wait:" << TaskManager::instance().waitThreadSize(0)
    << " completed:" << TaskManager::instance().completedTaskSize(0));
  TaskManager::instance().execute(new ReplyDispatchTask(source, stype, actor,
      reply));
}

void FeedAssistantI::addFeedReplyById(int stype, Ice::Long feedid,
    const FeedReplyPtr& reply, const Ice::Current&) {
  MCE_INFO("FeedAssistantI::addFeedReplyById ---> stype:" << stype << "feedid:" << feedid);
  FeedIndexPtr feedIndex = IndexCache::instance().getFeedIndex(feedid);
  if (!feedIndex) {
    feedIndex = DbHelper::instance().getFeedIndex(feedid);
    if (!feedIndex) {
      MCE_WARN("FeedAssistantI::addFeedReplyById ---> can not get feed, stype:" << stype << "feedid:" << feedid);
      return;
    }
  }
  ReplyLogger::instance().AddReply(feedid,feedIndex->source,feedIndex->stype,feedIndex->actor,reply);
  try {
    LongSeq feedIds;
    feedIds.push_back(feedid);
    FeedReplyBufferTimer::instance().changeReply(feedIds, reply); 
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedAssistantI::addFeedReplyById --> call FeedContentAdapter replaceFeedReply  err:"<<e);
  }

}

void FeedAssistantI::addFeedReplyWithConfig(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const FeedReplyPtr& reply, const map<string,string> & config, const Ice::Current& current){
  MCE_INFO("FeedAssistantI::addFeedReplyWithConfig --> source = " << source << ",stype = " << stype << ",actor = " << actor << ",config size:" << config.size()
    << " task size:" << TaskManager::instance().size(0) << " thread:" <<TaskManager::instance().runningThreadSize(0)
    << " wait:" << TaskManager::instance().waitThreadSize(0)
    << " completed:" << TaskManager::instance().completedTaskSize(0));
  TaskManager::instance().execute(new ReplyDispatchWithConfigTask(source, stype, actor,reply,
      config));
}

void FeedAssistantI::dispatchAgain(Ice::Long source, int stype, int actor,
    int toid, const Ice::Current&) {

  FeedIndexSeq index_seq = IndexCache::instance().getFeedIndexSeq(source,
      stype, actor);
  if (!index_seq.empty()) {
    MCE_INFO("FeedAssistantI::dispatchAgain --> get from cache, source:"<<source <<", stype:"<<stype<<", actor:"<<actor);
  } else {
    index_seq = DbHelper::instance().getFeedIndex(stype, source, actor);
		MCE_INFO("FeedAssistantI::dispatchAgain --> ok get from db, source:"<<source <<", stype:"<<stype<<", actor:"<<actor << ", index size = " << index_seq.size());
  }

	FeedIndexPtr index;
	for (size_t i = 0; i < index_seq.size(); ++i) {
		if(!index || index->feed < index_seq.at(i)->feed){
			index = index_seq.at(i);
		}
	}
  if (!index) {
		MCE_DEBUG("FeedAssistantI::dispatchAgain --> can not find index");
    return;
  }else{
		MCE_DEBUG("FeedAssistantI::dispatchAgain --> find index --> feedid = " << index->feed);
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
  data->feed = seed->feed;
  data->source = seed->source;
  data->actor = seed->actor;
  data->type = seed->type;
  data->time = seed->time;
  data->weight = seed->baseWeight;
  map<string, string> old_data = DbHelper::instance().getFeedContent(index->feed);
  data->xml = old_data["xml"]; 
  
  try {
    FeedContentPtr content = new FeedContent();
    content->data = data;
    bool res = FeedMemcClient::instance().SetFeed(content);
    if (!res) {
      MCE_INFO("FeedAssistantI::dispatchAgain --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__);
    }
  } catch (std::exception& e) {
    MCE_WARN("FeedAssistantI::dispatchAgain --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("FeedAssistantI::dispatchAgain --> call Memc err, unknown");
  }
	MCE_DEBUG("FeedAssistantI::dispatchAgain --> call copyFeedContent success, feed id = " << index->feed);
  FeedDispatcherReplicaAdapter::instance().dispatch(seed, config);

}

void FeedAssistantI::dispatchNewsAgain(Ice::Long feedid, const string& expr,
    const Ice::Current&) {

  FeedIndexPtr index = DbHelper::instance().getFeedIndex(feedid);
  //没有放入indexcache
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

  FeedDispatcherReplicaAdapter::instance().dispatch(seed, config);
}

void FeedAssistantI::cacheSync(Ice::Long source, int stype, int actor,
    const Ice::Current &) {
	MCE_INFO("FeedAssistantI::cacheSync --> source = " << source << ",stype = " << stype << ",actor = " << actor);
  IndexCache::instance().removeFeedBySourceStypeActor(source, stype, actor);
}

Ice::Long FeedAssistantI::getFeedId(Ice::Long source, int stype, int actor,
    const Ice::Current&) {
  MCE_INFO("FeedAssistantI::getFeedId --> source:"<<source<<" stype:"<<stype<<" actor:"<<actor);
  FeedIndexSeq index_seq = IndexCache::instance().getFeedIndexSeq(source,
      stype, actor);
  if (index_seq.empty()) {
    index_seq = DbHelper::instance().getFeedIndex(stype, source, actor);
    if (!index_seq.empty()) {
      IndexCache::instance().add(index_seq);
    }
  }
  Ice::Long max_id = 0;
  for (size_t i = 0; i < index_seq.size(); ++i) {
    max_id = index_seq.at(i)->feed > max_id ? index_seq.at(i)->feed : max_id;
  }
  return max_id;
}


void StatTimer::handle(){
	MCE_INFO("STAT: index_cache_size:"<<IndexCache::instance().size() << " thread_size:"<< TaskManager::instance().size(0) << ",ConfigCache size: " <<ConfigCache::instance()    .Size() );
}


void FeedAssistantI::ReplaceXML(Ice::Long source, int stype, int actor,const string & xml, const Ice::Current& ){
	MCE_INFO("FeedAssistantI::ReplaceXML --> source = " << source << ",stype" << stype << " ,actor = " << actor);
	TaskManager::instance().execute(new ReplaceXMLTask(source, stype, actor,xml));
}

void FeedAssistantI::DirectedSend(Ice::Long source, int stype, int actor, const vector<int> & targets, const Ice::Current&){
  if(targets.empty()){
    MCE_WARN("FeedAssistantI::DirectedSend. targets empty. source:" << source << " stype:" << stype << " actor:" << actor);
    return;
  }
  FeedIndexSeq idxseq;
  IndexLocator::LocateIndex(source,stype,actor,idxseq);
  if(idxseq.empty()){
    MCE_WARN("FeedAssistantI::DirectedSend. find indexs fail. source:" << source << " stype:" << stype << " actor:" << actor);
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
  FeedNewsAdapter::instance().addFeed(item,config,uid2weight);
  MCE_INFO("FeedAssistantI::DirectedSend. source:" << source << " stype:" << stype << " actor:" << actor
      << " targets size:" << targets.size() << " first target:" << targets.at(0));
}


bool FeedAssistantI::isNormalFeedType(int stype) {
  return !(_typeFilter.count(stype));
}

void FeedAssistantI::printThreadPoolInfoByLevel() { 
	std::map<int, int> taskPoolMap = TaskManager::instance().getLevel2Size();
	for(std::map<int,int>::iterator it = taskPoolMap.begin(); it != taskPoolMap.end(); it++) {
		int level = it->first;    
		size_t queueSize = it->second;
		size_t poolSize = TaskManager::instance().runningThreadSize(level);
		size_t waitSize = TaskManager::instance().waitThreadSize(level);
		size_t completedSize = TaskManager::instance().completedTaskSize(level);
		MCE_INFO("FeedAssistantI::printThreadPoolInfoByLevel --> level: " << level << " poolSize:" << poolSize << " queueSize:" << queueSize << " waitSize:" << waitSize << " completedSize:" << completedSize);
	}
}

void FeedAssistantI::setUserWeight(ResetWeightType type,Ice::Long id,Ice::Int weight,const Ice::Current & current){
}
void FeedAssistantI::removeUserWeight(ResetWeightType type, Ice::Long id,const Ice::Current& current){
}
MyUtil::Long2IntMap FeedAssistantI::getUserWeights(ResetWeightType type,const Ice::Current& current){
  return MyUtil::Long2IntMap();
}
void FeedAssistantI::clearUserWeight(const Ice::Current & current) {
  
}    
void FeedAssistantI::MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark, const Ice::Current&) {
}


