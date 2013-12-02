/*
 * FeedInterfaceI.cpp
 *
 *  Created on: Dec 24, 2009
 *      Author: antonio
 */

#include "FeedInterfaceI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "feed/FeedRedisClient/src/FeedIndex/FeedIndexRedisClient.h"  
#include "util/cpp/InvokeMonitor.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "FeedMemcProxy/client/feed_memc_client.h"

using namespace com::xiaonei::xce;
using namespace xce::feed;
using namespace std;
using namespace MyUtil;
using namespace Ice;
using namespace ::xce::serverstate;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedInterfaceI::instance(),
      service.createIdentity("M", ""));
 

 ////////// HA  ////////////////
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FeedInterface.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedInterface.Interval", 5);
  ServerStateSubscriber::instance().initialize("ControllerFeedInterfaceR",&FeedInterfaceI::instance(),mod,interval, new XceFeedControllerChannel);
//  MCE_INFO("mod:" << mod << " interval:" << interval);

 // struct rlimit rl;
 // getrlimit(RLIMIT_CORE, &rl);
 // MCE_INFO("MyUtil::initialize:: --> orgin core limit:" << rl.rlim_max << " " << rl.rlim_cur);
 // rl.rlim_cur = RLIM_INFINITY;
 // setrlimit(RLIMIT_CORE, &rl);
 // getrlimit(RLIMIT_CORE, &rl);
 // MCE_INFO("MyUtil::initialize:: --> now core limit:" << rl.rlim_max << " " << rl.rlim_cur);


}


void FeedInterfaceI::removeFeedById(Ice::Long feedId,
    const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::removeFeedById --> feedId = " << feedId << " " << current.con->toString());
}
void FeedInterfaceI::removeFeedBySource(Ice::Long source, Ice::Int stype,
    const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::removeFeedBySource --> ^^^ source = " << source << "stype = " << stype << " " << current.con->toString());

//  int r =  xce::feed::FeedIndexRedisClient::instance().RemoveIndex(source, stype);
//  MCE_INFO("FeedInterfaceI::removeFeedBySource  --> ^^^ r:" << r);

}

void FeedInterfaceI::removeFeedBySourceHS(Ice::Long source, Ice::Int stype,
    const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::removeFeedBySourceHS --> ^^^ source = " << source << "stype = " << stype << " " << current.con->toString());

//  int r =  xce::feed::FeedIndexRedisClient::instance().RemoveIndex(source, stype);
//  MCE_INFO("FeedInterfaceI::removeFeedBySourceHS  --> ^^^ r:" << r);
}

void FeedInterfaceI::removeFeedByParent(Ice::Long parent, Ice::Int ptype,
    const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::removeFeedByParent --> parent = " << parent << ",ptype = " << ptype << " " << current.con->toString());
}

void FeedInterfaceI::removeFeed(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::removeFeed --> source = " << source << ",stype = " << stype << ",actor = " << actor << " " << current.con->toString());
}

void FeedInterfaceI::removeFeedReply(Ice::Long source, Ice::Int stype,
    Ice::Int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply,
    const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::removeFeedReply --> source = " << source << ",stype = " << stype
      << ",actor = " << actor << ",removeReplyId = "<< removeReplyId << " " << current.con->toString());
}

void FeedInterfaceI::removeFeedReplyById(int stype, Ice::Long feedid,
    Ice::Long removeReplyId, const FeedReplyPtr& reply,
    const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::removeFeedReplyById --> stype = " << stype << ",feedid = " << feedid << ",removeReplyId = " << removeReplyId << " " << current.con->toString());
}
void FeedInterfaceI::readNews(Ice::Int user, Ice::Long feed,
    const Ice::Current& current) {

  MCE_INFO("FeedInterfaceI::readNews --> user:"<<user << " feed:"<< feed << " " << current.con->toString());

}

void FeedInterfaceI::readAllNews(Ice::Int user, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::readAllNews --> user = " << user << " " << current.con->toString());

}

void FeedInterfaceI::readMini(Ice::Int user, Ice::Long feed,
    const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::readMini --> user:"<<user << " feedid:"<<feed << " " << current.con->toString());
}

void FeedInterfaceI::readAllMini(Ice::Int user, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::readAllMini --> user:"<<user << " " << current.con->toString());
}

void FeedInterfaceI::dispatch(const FeedSeedPtr& seed,
    const Ice::Current& current) {

  if(!seed) {
    return ;
  }
  MCE_INFO("FeedInterfaceI::dispatch --> feedid:" << seed->feed << " stype:" << (seed->type&0xFFFF));
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

//    int r =  xce::feed::FeedIndexRedisClient::instance().AddIndex(index);
//   MCE_INFO("FeedInterfaceI::dispatch --> ^^^ r:" << r);

}
void FeedInterfaceI::dispatchEDM(const FeedSeedPtr& seed,
    const Ice::Current& current) {
  if (seed) {
    MCE_INFO("FeedInterfaceI::dispatchEDM --> feedid:" << seed->feed << " source:" << seed->source  << " " << current.con->toString());
  }
}

void FeedInterfaceI::dispatchWithReply(const FeedSeedPtr& seed,
    const FeedReplyPtr& reply, const Ice::Current& current) {
	MCE_INFO("FeedInterfaceI::dispatchWithReply --> feedid:" << seed->feed << " " << current.con->toString());
}

void FeedInterfaceI::addFeedReply(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const FeedReplyPtr& reply, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::addFeedReply --> add reply " << source << " " << stype << " " << actor<< " " << current.con->toString());
/*  std::vector<FeedIndexRedisClient::IndexItem> value;
 int   r =  xce::feed::FeedIndexRedisClient::instance().GetIndex(source,stype, value);

  if(r && value.size()) {
    MCE_INFO("FeedInterfaceI::addFeedReply -- ^^^ r:" << r << " size:" << value.size() << " feedid:" << value[0].feedid << " source:" << value[0].source << " stype:" << value[0].stype << " acotr:" << value[0].actor);
  } else {
    MCE_WARN("FeedInterfaceI::addFeedReply -- ^^^ r:" << r << " size:" << value.size());
  }
*/
}

void FeedInterfaceI::addFeedReplyById(int stype, Ice::Long feedid,
    const FeedReplyPtr& reply, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::addFeedReplyById --> stype = " << stype << ",feedid = " << feedid << " " << current.con->toString());
}

void FeedInterfaceI::addFeedReplyWithConfig(Ice::Long source, Ice::Int stype,
    Ice::Int actor, const FeedReplyPtr& reply,
    const map<string, string> & config, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::addFeedReplyWithConfig --> add reply " << source << " " << stype << " " << actor<< " " << current.con->toString());
}

void FeedInterfaceI::dispatchAgain(Ice::Long source, int stype, int actor,
    int toid, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::dispatchAgain --> source = " << source << ",stype = " << stype << ",actor = " << actor << ",toid = " << toid << " " << current.con->toString());
}



void FeedInterfaceI::ReplaceXML(Ice::Long source, int stype, int actor,
    const string & xml, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::ReplaceXML --> source = " << source << ",stype = " << stype << ",actor = " << actor << ", "<< current.con->toString());
}//TODO

void FeedInterfaceI::DirectedSend(Ice::Long source, int stype, int actor,
    const vector<int> & targets, const Ice::Current& current) {

  MCE_INFO("FeedInterfaceI::DirectedSend --> source = " << source << ",stype = " << stype << ",actor = " << actor << " targets size:" << targets.size() <<  ", "<< current.con->toString());
}


void FeedInterfaceI::MarkFeed(int uid, Ice::Long feedid, Ice::Long source,
    int stype, int actor, bool mark, const Ice::Current& current) {
  MCE_INFO("FeedInterfaceI::MarkFeed --> user:"<< uid << " feedid:"<<feedid
  		<< " fid:" << feedid << " source:" << source << " stype:" << stype << " actor:" << actor
  		<< " mark:" << mark
  		<< " " << current.con->toString());
}

FeedItemSeq FeedInterfaceI::GetOwnFeeds(const MyUtil::IntSeq & userids, int begin, int limit, 
      const MyUtil::IntSeq & types, const Ice::Current& current) {
  ostringstream oss;
  oss << "userid_size: " << userids.size() << " begin:" << begin
    << " limit:" << limit << " types_size:" << types.size() << " callcount:"<< getownfeeds_callcount_ ;
  InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  if(userids.empty()){
    return FeedItemSeq();
  }

  if (getownfeeds_callcount_ > kCallCountMax) {
    MCE_WARN("FeedInterfaceI::GetOwnFeeds. too many calls --> call_count:" << getownfeeds_callcount_)
    return FeedItemSeq();
  }
  MyUtil::IntSeq types_filter = types;
  if (types_filter.empty()) {
    types_filter = getFixedType();
  }

  {
    IceUtil::Mutex::Lock lock(mutex_);
    ++getownfeeds_callcount_;
  }
  FeedMetaSeq metas;
  try {
    metas = FeedItemCacheByUseridReplicaAdapter::instance().getByTypesWithFilterSimple(userids, begin, limit, types_filter, StrSeq());
    MCE_INFO("FeedInterfaceI::GetOwnFeeds. get from FeedItemCacheByUserid success --> feedmeta_size:" << metas.size())
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedInterfaceI::GetOwnFeedids. -->  error:"<< e);  
    return FeedItemSeq();
  }
  {
    IceUtil::Mutex::Lock lock(mutex_);
    --getownfeeds_callcount_;
  }

  FeedItemSeq fitems;
  fitems.reserve(metas.size());
  FeedItem item;
  for(FeedMetaSeq::iterator it = metas.begin(); it != metas.end(); ++it) {
    item.feed = it->feedid;
    item.merge = it->mergeid;
    item.type = it->type;
    item.time = it->time;
    item.weight = it->weight & 0x00ffffff;//高8位清零
    item.actor = it->userid;
    fitems.push_back(item); 
  }
  return fitems;
}

FeedContentDict FeedInterfaceI::GetFeedDict(const MyUtil::LongSeq & feedIds, const Ice::Current& current) {
  ostringstream oss;
  oss << "feedIds_size: " << feedIds.size() << " call_count:" << getfeeddict_callcount_;
  InvokeClient id = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  if(feedIds.empty()){
    return FeedContentDict();
  }

  if (getfeeddict_callcount_ > kCallCountMax) {
    MCE_WARN("FeedInterfaceI::GetFeedDict. too many calls --> call_count:" << getfeeddict_callcount_)
    return FeedContentDict();
  }
  {
    IceUtil::Mutex::Lock lock(mutex_);
    ++getfeeddict_callcount_;
  }
  FeedContentDict dict;
  try {
    dict = FeedMemcClient::instance().GetFeedDict(feedIds);
    //MCE_INFO("FeedInterface::getFeedDict. FeedMemcClient.getFeedDict --> " << " ids:" << feedIds.size() << " res:" << dict.size() ); 
  } catch(Ice::Exception & e) {
    MCE_WARN("FeedInterface::GetFeedDict --> " << e);
    return FeedContentDict();
  }
  {
    IceUtil::Mutex::Lock lock(mutex_);
    --getfeeddict_callcount_;
  }
  return dict;
}

MyUtil::IntSeq FeedInterfaceI::getFixedType() {
  MyUtil::IntSeq result;
  int type_size = sizeof(TYPE)/sizeof(TYPE[0]);
  result.reserve(type_size);
  for (int i = 0; i < type_size; i++) {
    result.push_back(TYPE[i]);
  }
  return result;
}


