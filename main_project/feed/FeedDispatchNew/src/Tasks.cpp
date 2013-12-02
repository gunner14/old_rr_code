#include <exception>
#include <stdexcept>
#include <sys/resource.h>
#include <ostream>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include "Tasks.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp> 
#include "pugixml.hpp"
#include "util/cpp/String.h"
#include "Date.h"
#include "DbHelper.h"
#include "TaskManager.h"
#include "PerformanceStat.h"
#include "FeedInteractionNAdapter.h"
#include "FeedMemcProxy/client/feed_interaction_client_n.h"
#include "FeedMemcProxy/client/feed_memc_client.h"
#include "FeedMemcProxy/client/feed_index_memc_client.h"
#include "FeedMemcProxy/client/feed_memc_client.h"
#include "feed/FeedRedisClient/src/FeedIndex/FeedIndexRedisClient.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "UserNetworkAdapter.h"
#include "FeedSchoolAdapter.h"
#include "FeedGroupAdapter.h"
#include "FeedNewsReplicaAdapter.h"
#include "FeedDbWriterReplicaAdapter.h"
#include "PhotoDataGatheringAdapter.h"
#include "FeedNaiveBayesReplicaAdapter.h"
#include "FeedMiniReplicaAdapter.h"
#include "FeedConfigAdapter.h"
#include "WTransitAdapter.h"
#include "FriendRankCacheAdapter.h"
#include "FeedDispatchNewI.h"
#include "UserStateAdapter.h"
#include "UserConfigAdapter.h"
#include "UserFeeds.h"
#include "Common.h"
#include "FeedDeliver.h"
#include "ExpressionCache.h"

//---------------
using namespace pugi;
using namespace xce::feed;
using namespace mop::hi::svc::adapter;
using namespace xce::adapter::userstate;
using namespace xce::socialgraph;
using namespace boost;

void ConfigCache::Add(const Ice::Long feed_id,
    const map<string, string> & config) {
  ItemPtr item = new Item();
  item->feed_id = feed_id;
  item->config = config;

  IceUtil::RWRecMutex::WLock lock(mutex_);
  SIndex & sindex = pool_.get<seqtag> ();
  std::pair<SIndex::iterator, bool> p = sindex.push_front(item);
  if (!p.second) { //失败
    //MCE_INFO("ConfigCache::Add --> feed_id = " << feed_id << ",insert fail,already exist");
  } else { //成功
    //MCE_INFO("ConfigCache::Add --> feed_id = " << feed_id << ",insert success,Kick ...");
    Kick();
  }
}

map<string, string> ConfigCache::Get(const Ice::Long feed_id) {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  FIndex & findex = pool_.get<feedtag> ();
  FIndex::iterator fit = findex.find(feed_id);
  if (fit != findex.end()) {
    //MCE_DEBUG("ConfigCache::Get --> feed_id = " << feed_id << ",get success , " << PrintHelper());
    return (*fit)->config;
  } else {
    //MCE_DEBUG("ConfigCache::Get --> feed_id = " << feed_id << ",miss in cache , "<< PrintHelper());
    return map<string, string> ();
  }

}

string ConfigCache::PrintHelper() {
  SIndex & sindex = pool_.get<seqtag> ();
  SIndex::iterator sit = sindex.begin();
  ostringstream oss;
  oss << "ConfigCache feedids = (";
  int max = 10, count = 0;
  for (; sit != sindex.end(); ++sit) {
    oss << (*sit)->feed_id << ",";
    ++count;
    if (count >= max) {
      oss << " ... ";
      break;
    }
  }
  oss << ")";
  return oss.str();
}

void ConfigCache::Kick() { //不加锁
  //MCE_INFO("ConfigCache::Kick --> before kick,cache size = " << pool_.size());
  while (pool_.size() > (unsigned) kSize_) {
    pool_.pop_back();
  }
  //MCE_INFO("ConfigCache::Kick --> after  kick,cache size = " << pool_.size());
}

size_t ConfigCache::Size() {
  IceUtil::RWRecMutex::RLock lock(mutex_);
  return pool_.size();
}

void InteractionUploader::UploadReply(const FeedIndexPtr & idx,int r){
  ostringstream os;
  os << " fid:" << idx->feed << " stype:" << idx->stype << " actor:" << idx->actor << " source:" << idx->source;
  try{
    set<long> fids;
    long fid = idx->feed;
    fids.insert(fid);

    FeedInterMap intermap;
    FeedInterPtr inter;
    FeedInteractionMemcClientN::instance().GetFeedInterMap(fids,intermap);

    FeedInterMap::iterator it = intermap.find(fid);
    if(it != intermap.end() )
      inter = it->second;
    bool exist = inter;
    int oldreply = -1,oldrange = -1;
    if(!inter){
      inter = shared_ptr<FeedInter>(new FeedInter);
    } else {
      oldreply = inter->GetReply();
      oldrange = inter->GetRange();
    }

    inter->SetReply(r);
    FeedInteractionMemcClientN::instance().SetFeedInter(fid,inter);
    MCE_INFO("InteractionUploader::UploadReply. index " << os.str() << " exist:" << exist
        << " oldreply:" << oldreply
        << " oldrange:" << oldrange
        << " reply:" << r );
  }catch (Ice::Exception& e) {
    MCE_WARN("InteractionUploader::UploadReply. index " << os.str() << " reply:" << r << " err:" << e);
  } catch (std::exception& e) {
    MCE_WARN("InteractionUploader::UploadReply. index " << os.str() << " reply:" << r << " err:" << e.what());
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
//////////////////////////////////////////////////////////////////////////////////////////
void RemoveFeedByTimeTask::handle() {
  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = DbHelper::instance().getFeedIndexByTime(source_, stype_, actor_, _time);
  if (!indexSeq.size()) {
    MCE_INFO("RemoveFeedByTimeTask::handle --> get index fail. source:" << source_ << " stype:" << stype_ << " actor:" << actor_ << " time:" << _time);
    return;
  } else {
    MCE_INFO("RemoveFeedByTimeTask::handle --> get index sucess. source:" << source_ << " stype:" << stype_ << " actor:" << actor_ << " time:" << _time << " indexSeq size:" << indexSeq.size());
  }
  //----------
  try {
    FeedDbWriterReplicaAdapter::instance().RemoveFeedFromDB(indexSeq);
  } catch (Ice::Exception& e ) {
    MCE_INFO("RemoveFeedByTimeTask::handle call FeedDbWriter::ReomveFeedFromDB err e: "<<e<<" source: "<<source_<<" stype: "<<stype_<<" actor:"<<actor_);
  }

  for(FeedIndexSeq::const_iterator it = indexSeq.begin(); it != indexSeq.end(); it++) {
    try {
      MCE_INFO("RemoveFeedByTimeTask:: --> FeedMemcClient SetEmptyFeed feed:" << (*it)->feed);
      bool res =  FeedMemcClient::instance().SetEmptyFeed((*it)->feed);
      if (!res) {
        MCE_INFO("RemoveFeedByTimeTask:: --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__ << " feedid:" << (*it)->feed);
      }
    } catch (std::exception& e) {
      MCE_WARN("RemoveFeedByTimeTask::handle --> call Memc err, " << e.what() << " feedid:" << (*it)->feed);
    } catch (...) {
      MCE_WARN("RemoveFeedByTimeTask::handle --> call Memc err, unknown" << " feedid:" << (*it)->feed);
    }
  }

  try {
    FeedDispatchNewI::instance().CacheSync(source_, stype_, actor_);
  } catch (Ice::Exception e) {
    MCE_INFO("RemoveFeedByTimeTask::handle call FeedDispatchNewI::CacheSync err e: "<<e<<" source: "<<source_<<" stype: "<<stype_<<" actor:"<<actor_);
  }
  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmTime(timeuse);
  MCE_INFO("RemoveFeedByTimeTask::handle --> source:" << source_ << " stype:" << stype_ << " actor:" << actor_ << " time;" << _time << " seq:" << indexSeq.size() << " time use:" << timeuse);
}
void RemoveFeedByIdTask::handle() {
  timeval beg, end;
  gettimeofday(&beg, NULL);

  //FeedIndexPtr index = FeedDispatchNewI::instance().GetFeedIndexById(_feedId);
  FeedIndexPtr index = DbHelper::instance().getFeedIndex(_feedId);
  if (!index) {
    MCE_INFO("RemoveFeedByIdTask::handle --> get index fail. feedId = " << _feedId);
    return;
  } else {
    MCE_INFO("RemoveFeedByIdTask::handle --> get index sucess. feedId = " << _feedId << ", source = " << index-> source << ",stype = " << index->stype << ",actor = " << index->actor);
  }
  FeedIndexSeq indexSeq;
  indexSeq.push_back(index);
  //----------
  try {
    FeedDbWriterReplicaAdapter::instance().RemoveFeedFromDB(indexSeq);
  } catch (Ice::Exception& e ) {
    MCE_WARN("RemoveFeedByIdTask::handle call FeedDbWriter::ReomveFeedFromDB err e: "<<e<<" source: "<<index->source<<" stype: "<<index->stype<<" actor:"<<index->actor<<" feed:"<<_feedId);
  }
  
  try {
    MCE_INFO("RemoveFeedByIdTask:: --> FeedMemcClient SetEmptyFeed feed:" << _feedId);
    bool res = FeedMemcClient::instance().SetEmptyFeed(_feedId);
    if (!res) {
      MCE_INFO("RemoveFeedByIdTask:: --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__);
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedByIdTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedByIdTask::handle --> call Memc err, unknown");
  }

  try {
    FeedDispatchNewI::instance().CacheSync(index->source, index->stype,
        index->actor);
  } catch (Ice::Exception e) {
    MCE_WARN("RemoveFeedByIdTask::handle call FeedDispatchNewI::CacheSync err e: "<<e<<" source: "<<index->source<<" stype: "<<index->stype<<" actor:"<<index->actor<<" feed:"<<_feedId);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmId(timeuse);

}

void RemoveFeedBySourceStypeTask::handle() {

  timeval beg, end;
  gettimeofday(&beg, NULL);
  
  FeedIndexSeq indexSeq = FeedDispatchNewI::instance().GetFeedIndex(source_,stype_,-1);
  if (indexSeq.empty()) {
    MCE_INFO("RemoveFeedBySourceStypeTask::handle. FeedIndex empty source: " << source_ << " stype: " << stype_);
    return;
  }else{
    MCE_INFO("RemoveFeedBySourceStypeTask::handle.get index size:" << indexSeq.size() << " " << indexSeq.at(0)->feed <<" source: " << source_ << " stype: " << stype_);
  }
  MyUtil::LongSeq ids;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ids.push_back(indexSeq.at(i)->feed);
  }
  //----------
  try {
    FeedDbWriterReplicaAdapter::instance().RemoveFeedFromDB(indexSeq);
  } catch (Ice::Exception& e ) {
    MCE_INFO("RemoveFeedBySourceStypeTask::handle call FeedDbWriter::ReomveFeedFromSourceStypeDB err e: "<<e<<" source: "<<source_<<" stype: "<<stype_);
  }
  
  try {
    for (size_t i = 0; i < ids.size(); i++) {
      bool res =  FeedMemcClient::instance().SetEmptyFeed(ids.at(i));
      if (!res) {
        MCE_INFO("RemoveFeedBySourceStypeTask --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__ << " source: " << source_ << " stype: " << stype_);
      }
      MCE_INFO("RemoveFeedBySourceStypeTask --> FeedMemcClient SetEmptyFeed feed:" << ids.at(i) << " source: " << source_ << " stype: " << stype_);
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err, " << e.what() << " source: " << source_ << " stype: " << stype_);
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err, unknown" << " source: " << source_ << " stype: " << stype_);
  }

  map<string, FeedIndexPtr> tmp;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ostringstream os;
    os << indexSeq.at(i)->source << "," << indexSeq.at(i)->stype << ",";
    tmp[os.str()] = indexSeq.at(i);
  }
  for (map<string, FeedIndexPtr>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    try {
      FeedDispatchNewI::instance().CacheSync(it->second->source,
          it->second->stype, it->second->actor);
    } catch (Ice::Exception & e) {
      MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call FeedDispatchNewI::CacheSync source:"<<it->second->source << ", stype:"<< it->second->stype << ", actor:"<< it->second->actor <<", err:"<<e);
    }
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmSourceStype(timeuse);
}

void RemoveFeedBySourceStypeActorTask::handle() {

  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = FeedDispatchNewI::instance().GetFeedIndex(source_,stype_,actor_);
  if (indexSeq.empty()) {
    MCE_INFO("RemoveFeedBySourceStypeActorTask::handle -->  getFeedIndex result empty ,stype = " << stype_ << ",source = " << source_ << ",actor = " << actor_);
    return;
  } else {
    MCE_INFO("RemoveFeedBySourceStypeActorTask::handle -->  getFeedIndex result sucess, size = " << indexSeq.size()
        << ",stype = " << stype_ << ",source = " << source_ << ",actor = " << actor_);
  }
  MyUtil::LongSeq ids;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ids.push_back(indexSeq.at(i)->feed);
  }
  //----------
  try {
    FeedDbWriterReplicaAdapter::instance().RemoveFeedFromDB(indexSeq);
  } catch (Ice::Exception& e ) {
    MCE_INFO("RemoveFeedBySouceStypeActorTask::handle call FeedDbWriter::ReomveFeedFromDB err e: "<<e<<" source: "<<source_<<" stype: "<<stype_<<" actor:"<<actor_);
  }
  
  try {
    for (size_t i = 0; i < ids.size(); i++) {
      MCE_INFO("RemoveFeedBySourceStypeActorTask::handle --> FeedMemcClient SetEmptyFeed feed:" << ids.at(i));
      bool res =  FeedMemcClient::instance().SetEmptyFeed(ids.at(i));
      if (!res) {
        MCE_INFO("RemoveFeedBySourceStypeActorTask::handle --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeActorTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeActorTask::handle --> call Memc err, unknown");
  }

  try {
    FeedDispatchNewI::instance().CacheSync(source_, stype_, actor_);
  } catch (Ice::Exception & e) {
    MCE_WARN("RemoveFeedBySouceStypeActorTask::handle call FeedDispatchNewI::CacheSync err e: "<<e<<" source: "<<source_<<" stype: "<<stype_<<" actor:"<<actor_);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmSourceStypeActor(timeuse);
}

void RemoveFeedByParentTask::handle() {
  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = FeedDispatchNewI::instance().GetFeedIndex(psource_,ptype_,-1);
  if (indexSeq.empty()) {
    return;
  }
  MyUtil::LongSeq ids;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ids.push_back(indexSeq.at(i)->feed);
  }
  //----------
  try {
    FeedDbWriterReplicaAdapter::instance().RemoveFeedFromDB(indexSeq);
  } catch (Ice::Exception& e ) {
    MCE_INFO("RemoveFeedByParentTask::handle call FeedDbWriter::ReomveFeedFromDB err e: "<<e<<" source: "<<psource_<<" stype: "<<ptype_);
  }

  try {
    for (size_t i = 0; i < ids.size(); i++) {
      MCE_INFO("RemoveFeedByParentTask --> FeedMemcClient SetEmptyFeed feed:" << ids.at(i));
      bool res =  FeedMemcClient::instance().SetEmptyFeed(ids.at(i));
      if (!res) {
        MCE_INFO("RemoveFeedByParentTask::handle --> FeedMemcClient SetEmptyFeed FAIED!!! line"<<__LINE__);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedByParent::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedByParent::handle --> call Memc err, unknown");
  }

  map<string, FeedIndexPtr> tmp;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ostringstream os;
    os << indexSeq.at(i)->source << "," << indexSeq.at(i)->stype << ","
      << indexSeq.at(i)->actor;
    tmp[os.str()] = indexSeq.at(i);
  }
  for (map<string, FeedIndexPtr>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    try {
      FeedDispatchNewI::instance().CacheSync(it->second->source,
          it->second->stype, it->second->actor);
    } catch (Ice::Exception & e) {
      MCE_WARN("RemoveFeedByParentTask::handle call FeedDispatchNewI::CacheSync err e: "<<e<<" source: "<<psource_<<" stype: "<<ptype_);
    }
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmParent(timeuse);
}

void FeedDispatchTask::CheckSend2School(const FeedConfigPtr& config) {
  if ((seed_->type & 0xffff) / 100 != 1) {
    return;
  }
  NetworkBigObjectNPtr info;
  timeval begin, end;
  gettimeofday(&begin, NULL);
  try {
    info = UserNetworkAdapter::instance().getBigObjectN(seed_->actor);
  } catch (Ice::Exception& e) {
    MCE_INFO("ICE_Exception!! err e: "<<e<<"File: "<<__FILE__<<"Line: "<<__LINE__);
    MCE_WARN("FeedDispatchTask::CheckSend2School --> call UserNetwork err, " << seed_->actor << " " << e);
  }
  gettimeofday(&end, NULL);
  float timeuse = 1000000 * (end.tv_sec - begin.tv_sec) + end.tv_usec
    - begin.tv_usec;
  timeuse /= 1000;
  try {
    for (size_t i = 0; i < info->highschools.size(); i++) {
      Date d = Date(time(NULL));
      if (d.year() - info->highschools.at(i)->enrollYear < 3) {
        MCE_INFO("FeedDispatchTask::CheckSend2School --> actor:" << seed_->actor << " highschoolid:" << info->highschools.at(i)->HighSchoolId << " cost:" << timeuse);
        FeedItem item;
        MyUtil::Int2IntMap id2Weight;
        item.feed = seed_->feed;
        item.type = seed_->type;
        item.time = seed_->time;
        item.weight = seed_->baseWeight;
        item.merge = seed_->miniMerge;
        item.actor = seed_->actor;
        id2Weight[info->highschools.at(i)->HighSchoolId] = item.weight;
        FeedSchoolAdapter::instance().addFeed(item, config, id2Weight);
      }
    }
    for (size_t i = 0; i < info->universitys.size(); i++) {
      Date d = Date(time(NULL));
      if (d.year() - info->universitys.at(i)->enrollYear < 4) {
        MCE_INFO("FeedDispatchTask::CheckSend2School --> actor:" << seed_->actor << " universityid:" << info->universitys.at(i)->universityId << " cost:" << timeuse);
        FeedItem item;
        MyUtil::Int2IntMap id2Weight;
        item.feed = seed_->feed;
        item.type = seed_->type;
        item.time = seed_->time;
        item.weight = seed_->baseWeight;
        item.merge = seed_->miniMerge;
        item.actor = seed_->actor;
        id2Weight[info->universitys.at(i)->universityId] = item.weight;
        FeedSchoolAdapter::instance().addFeed(item, config, id2Weight);
      }
    }
  } catch (Ice::Exception& e) {
    MCE_INFO("ICE_Exception!! err e: "<<e<<"File: "<<__FILE__<<"Line: "<<__LINE__);
    MCE_WARN("FeedDispatchTask::CheckSend2School --> err, " << e);
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchTask::CheckSend2School --> write to db err, " << e.what());
  } catch (...) {
    MCE_WARN("FeedDispatchTask::CheckSend2School --> write to db err");
  }
}

void FeedDispatchTask::MakeFeedConfig(FeedConfigPtr &config) {
  if (!config) {
    return;
  }
  config_->expr = seed_->extraProps["expr"];
  config_->updateTime = seed_->extraProps["update_time"] == "1";
  config_->sendConfig = seed_->extraProps.find("sendconfig_")
    != seed_->extraProps.end();
  config_->sendNewsConfig = seed_->extraProps["publish_news"] == "1";
  config_->sendMiniConfig = seed_->extraProps["publish_mini"] == "1";
  ConfigCache::instance().Add(seed_->feed, seed_->extraProps);

}
void FeedDispatchTask::MakeFeedIndex(FeedIndexPtr & index) {
  if (!index) {
    return;
  }
  index->feed = seed_->feed;
  index->newsMerge = seed_->newsMerge;
  index->miniMerge = seed_-> miniMerge;
  index->source = seed_->source;
  index->psource = seed_->psource;
  index->actor = seed_->actor;
  index->time = seed_->time;
  index->stype = seed_->type & 0xffff;
  index->ptype = seed_->ptype;

  int r = xce::feed::FeedIndexRedisClient::instance().AddIndex(index);   
  if (!r) {
    MCE_WARN("FeedDispatchTask::MakeFeedIndex --> add index to chace failed source:"<<index->source<<" stype:"<<index->stype<<" actor:"<<index->actor);
  }

}
void FeedDispatchTask::MakeFeedData(const FeedConfigPtr & config,FeedDataPtr & data) {
  if (!config || !data) {
    return;
  }
  bool saving = seed_->extraProps["save_db"] != "0";
  data->feed = seed_->feed;
  data->source = seed_->source;
  data->actor = seed_->actor;
  data->type = seed_->type;
  data->xml = seed_->xml;
  data->time = seed_->time;

  unsigned int baseWeightId = 0;
  if (seed_->extraProps.find("base_weight_id") != seed_->extraProps.end()) {
    try {
      baseWeightId = boost::lexical_cast<int>(
          seed_->extraProps["base_weight_id"]);
    } catch (std::exception & e) {
      MCE_WARN("FeedDispatchTask::MakeFeedData --> cast base_weight_id err:"<<e.what() << " feed:" << seed_->feed <<",actor"<< seed_->actor << " stype:" << (seed_->type&0xFFFF));
    }
  }
  data->weight = (baseWeightId << 16) & 0x00ff0000;
  try {
    FeedContentPtr content = new FeedContent();
    content->data = data;
    bool res =  FeedMemcClient::instance().SetFeed(content);
    if (!res) {
      MCE_WARN("FeedDispatchTask::handle --> FeedMemcClient SetEmptyFeed FAIED!!! fid:"<< seed_->feed<<" line"<<__LINE__);
    }
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("FeedDispatchTask::handle --> call Memc err, unknown");
  }

  MCE_INFO("FeedDispatchTask::handle --> fid:" << seed_->feed << ", newsMerge:"<<seed_->newsMerge<<", miniMerge:"<<seed_->miniMerge
      << ", source:"<<seed_->source<<", psource:"<<seed_->psource<<", actor:"<<seed_->actor<<", time:"<<seed_->time
      << ", stype:"<<(seed_->type &0xFFFF)<< ", ptype:"<<seed_->ptype << ", seed->type:"<< seed_->type << ", news_merge_type:"
      << int((seed_->type >> 30)& 0x00000003 )<< ", mini_merge_type:"<< int((seed_->type & 0x3FFFFFFF) >> 28) << ", weight:" << seed_->baseWeight
      << ", updateTime:"<<config->updateTime << ", sendconfig_:"<<config->sendConfig<<", news_publish:"
      <<config->sendNewsConfig <<", mini_publish:"<< config->sendMiniConfig << ", expr:"<< config->expr << " saving:" << saving);
}
void FeedDispatchTask::Deliver2Group(const FeedConfigPtr &config) {
  if (!config) {
    return ;
  }
  int groupid = 0;
  try {
    groupid = boost::lexical_cast<int>(seed_->extraProps["togroup"]);
  } catch (...) {
    MCE_WARN("FeedDispatchTask::handle --> lexical cast err, " << seed_->extraProps["togroup"]);
  }
  FeedItem item;
  MyUtil::Int2IntMap id2Weight;
  item.feed = seed_->feed;
  item.type = seed_->type;
  item.time = seed_->time;
  item.weight = seed_->baseWeight;
  item.merge = seed_->miniMerge;
  item.actor = seed_->actor;
  id2Weight[groupid] = item.weight;
  try {
    FeedGroupAdapter::instance().addFeed(item, config, id2Weight);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedDispatchTask::handle --> send to group, actor:" << seed_->actor << " feed:" <<seed_->feed << " group:" << groupid << " err, " << e);
  }

}
void FeedDispatchTask::UpdateReply2Memc(ReplyDataPtr & replyData) {
  if (!replyData) {
    return;
  }
  ReplyLogger::instance().AddReply(seed_->feed,seed_->source,seed_->type & 0xffff,seed_->actor,reply_);
  replyData->source = seed_->source;
  replyData->stype = seed_->type & 0xffff;
  replyData->actor = seed_->actor;
  replyData->replyCount = reply_->count;
  replyData->time = seed_->time;//(int)time(NULL);
  replyData->xml = (!(reply_->newReply.empty()) ? reply_->newReply
      : reply_->oldReply);
  replyData->replyId = (!(reply_->newReply.empty()) ? reply_->newReplyId
      : reply_->oldReplyId);
  try {
    FeedContentPtr content = FeedMemcClient::instance().GetFeed(seed_->feed);
    if (content) {
      content->reply = reply_;
      bool res =  FeedMemcClient::instance().SetFeed(content);
      if (!res) {
        MCE_WARN("FeedDispatchTask::handle() --> FeedMemcClient SetFeed err, " << seed_->feed);
      }
    } else {
      MCE_WARN("FeedDispatchTask::handle() --> FeedMemcClient GetFeed err, " << seed_->feed);
    }
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("FeedDispatchTask::handle --> call Memc err, unknown");
  }
}
void FeedDispatchTask::HandlePhoto(){
 int stype = seed_->type & 0xFFFF;
  if (stype == 702) {// 圈人
    MCE_INFO("FeedDispatchTask::handle -->event: tag user,  stype: "<<stype<<" type: "<<seed_->type<<" actor: "<< seed_->actor<<" line: "<<__LINE__);
    Ice::Long photoid = seed_->source;
    int tagid = seed_->actor;
    if (photoid > 0) {
      try {
        PhotoDataGatheringAdapter::instance().IncTagCount(photoid, tagid);
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedDispatchTask::handle -->tag user  err:"<<e);
      }
    } else {
      MCE_INFO("FeedDispatchTask::handle -->event: tag user FAILED!! Can not find photoid: "<<stype<<" type: "<<seed_->type<<" actor: "<< seed_->actor<<" line: "<<__LINE__);
    }

  } else if (stype == 708) {//照片第一次被回复
    Ice::Long photoid = seed_->source;
    if (photoid >= 0) {
      MCE_INFO("FeedDispatchTask::handle -->event: photo is replyed first time,  stype: "<<stype<<" type: "<<seed_->type<<" actor: "<< seed_->actor<<" line: "<<__LINE__);
      try {
        PhotoDataGatheringAdapter::instance().IncCommentCount(photoid);
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedDispatchTask::handle -->event photo is replyed first time  err:"<<e);
      }

    } else {
      MCE_INFO("FeedDispatchTask::handle -->FAILED!! event: photo is replyed first time,  stype: "<<stype<<" actor: "<< seed_->actor<<" line: "<<__LINE__);
    }
  }
}
void FeedDispatchTask::WriteDataToMemc() {//dispatch 和 dispatchWithReply共同调用

  timeval tv_start, tv_end;
  gettimeofday(&tv_start, NULL);
  TimeStat ts_step;

  if (!seed_) {
    MCE_INFO("FeedDispatchTask::WriteDataToMemc --> seed is null");
    return;
  }
  MCE_INFO("FeedDispatchTask::WriteDataToMemc --> source:"<<seed_->source<<" feed="<<seed_->feed<<" time="<<seed_->time);

  int stype = seed_->type & 0xffff;
  if (stype == 701 || stype == 709) {
    seed_->extraProps["publish_news"] = "1";
    seed_->extraProps["publish_mini"] = "1";
  }
  //------------------FeedConfig ,向cache添加
  config_ = new FeedConfig();
  MakeFeedConfig(config_);
  float config_cost = ts_step.getTime(true);
  //-------------------构造FeedIndex向redis添加
  FeedIndexPtr index = new FeedIndex;
  MakeFeedIndex(index);
  float index_cost = ts_step.getTime(true);
  //----------------构造FeedData,新鲜事本体,写入memcached
  FeedDataPtr data = new FeedData();
  MakeFeedData(config_,data);
  float data_cost = ts_step.getTime(true);
  //--------------- 
  if ( config_ && config_->sendMiniConfig) {
    CheckSend2School(config_);
  }
  float school_cost = ts_step.getTime(true);
  //---------------
  if (seed_->extraProps.find("togroup") != seed_->extraProps.end()) {
    Deliver2Group(config_);
  }
  float group_cost = ts_step.getTime(true);
  //-----------------图片相关的处理
  HandlePhoto();
  //-----------------
  gettimeofday(&tv_end, NULL);
  float timeuse = 1000000 * (tv_end.tv_sec - tv_start.tv_sec) + tv_end.tv_usec
    - tv_start.tv_usec;
  timeuse /= 1000;
  PerformanceStat::instance().incDispatch(timeuse);
  MCE_INFO("FeedDispatchTask::WriteDataToMemc --> time cost makeConfig:"<<config_cost<<" makeIndex:"<<index_cost<<" makeData:"<<data_cost<<" sendSchool:"<<school_cost<<" toGtoup" << group_cost<<" totalCost:"<< timeuse);
}
int FeedDispatchTask::GetAppId() {
  MyUtil::Str2StrMap::iterator i = seed_->extraProps.find("AppId");
  if(i == seed_->extraProps.end()){
    return 0;
  }
  int app_id = 0;
  try{
    app_id = lexical_cast<int>(i->second);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedDispatchTask::GetAppId exception :" <<e);
    return 0;
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchTask::GetAppId exception :" << e.what());
    return 0;
  }
  return app_id;

}

map<int,int> FeedDispatchTask::GetBayesScores(int actor,const set<int> & target,int stype){
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
      MCE_WARN("FeedDispatchTask::GetBayesScores  call Bayes Exception"
          << " fid:" << seed_->feed << " actor:" << seed_->actor
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
          MCE_WARN("FeedDispatchTask::GetBayesScores  call Bayes Exception"
              << " fid:" << seed_->feed << " actor:" << seed_->actor
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

void FeedDispatchTask::DispatchFeed() {
  DispatchCost cost;

  TimeStat t1,t2;
  int stype = seed_->type & 0x0000ffff;
  int bigtype = stype / 100;//
  int appid = seed_->newsMerge;//对于特定的type，newsMerge代表appid

  if(stype == 8190){ //对于81XX的新鲜事，psource为appid
    appid = GetAppId();
  }

  if (config_->sendConfig) { //使用数据库里，用户的设置
    try {
      bool p = ConfigManagerAdapter::instance().GetSendConfig(//现在的SendConfig中只需要查询news的设置
          seed_->actor, bigtype, appid);
      if (!p) {
        config_->sendNewsConfig = p;
        config_->sendMiniConfig = p;//根据产品的需求，miniconfig和newsconfig相等
      }
      MCE_INFO("FeedDispatchTask::DispatchFeed. call FeedConfig source:" << seed_->source << " type:" << seed_->type << " actor:" << seed_->actor << " sendNewsConfig = " << config_->sendNewsConfig << ", p:"<<p << " appid:" << appid);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedDispatchTask::DispatchFeed ConfigManagerAdapter::instance().GetSendConfig().uid:<< "<< seed_->actor << " fid:" << seed_->feed << "exception "<<e);
    }
  }
  cost.send_cfg_ = t1.getTime(true);

  set<int> target;
  if (config_->sendNewsConfig) { //需要发送News
    MCE_DEBUG("FeedDispatchTask::DispatchFeed --> sendNewsConfig = true");
    string oldexpr = config_->expr;
    try {
      target = ExprCache::instance().Expr2Ids(config_->expr, seed_->actor);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedDispatchTask::DispatchFeed --> ExprCache::instance().Expr2Ids() --> exception :" <<e);
    } catch (std::exception& e) {
      MCE_WARN("FeedDispatchTask::DispatchFeed --> ExprCache::instance().Expr2Ids() --> exception :" << e.what());
    }
    cost.expr_ = t1.getTime(true);
    if (config_->sendMiniConfig) {
      int stype = seed_->type & 0xFFFF;
      if (!(stype >= 2901 && stype <= 2904)) {
        target.insert(seed_->actor);
      }
    }
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
    ::MyUtil::Str2StrMap::iterator expt = seed_->extraProps.find("MachineDispatch");
    if ( (expt != seed_->extraProps.end()) && expt->second == "addMiniSiteFeed" ) {
      isMachineDispatch = true;
      MCE_INFO("FeedDispatcherTask::DispatchFeed --> addMiniSiteFeed dispach feed actor:"<<seed_->actor<<" feedid:"<<seed_->feed<<" skip ConfigManager::FilterRecvConfig()  ");
    }
    /*******************************/
    for (vector<int>::iterator vit = in.begin(); vit < in.end()&& (!isMachineDispatch); vit
        += kMaxSize, ++i) {
      MCE_DEBUG("FeedDispatchTask::DispatchFeed--> INVOKE --> ConfigManagerAdapter::instance().FilterRecvConfig --> part :" << i);
      partIn.assign(vit, vit + kMaxSize < in.end() ? vit + kMaxSize : in.end());
      try {
        partOut = ConfigManagerAdapter::instance().FilterRecvConfig(partIn,
            bigtype, appid);
        not_recv_cnt += partOut.size();
        for (vector<int>::iterator vit = partOut.begin(); vit != partOut.end(); ++vit) {
          target.erase(*vit);
        }
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedDispatchTask::DispatchFeed --> Invoke --> ConfigManagerAdapter::instance().uid : "<< seed_->actor << " fid:" << seed_->feed <<" exception "<<e);
      }
    }
    cost.recv_cfg_ = t1.getTime(true);
    MCE_INFO("FeedDispatchTask::DispatchFeed --> feedId = " << seed_->feed << ",actor = " << seed_->actor <<
        ",expression = " << config_->expr << ", filter ids size = " << not_recv_cnt << ", result size : " << target.size()
        << " appid:" << appid);
  } else {
    MCE_INFO("FeedDispatchTask::DispatchFeed sendNewsConfig is false");
  }
  TimeStat tb;
  int limitThreshold = 100000;
  map<int,int> scores;
  if (target.size() < limitThreshold) {
    scores = GetBayesScores(seed_->actor, target, stype);
  } else {
    TaskManager::instance().execute(new DispatchBigFansSizeFeedTask(seed_,config_,target));
    return;
  }
  MCE_INFO("FeedDispatchTask::DispatchFeed call bayes. bayescost:" << tb.getTime());

  FeedDeliver::instance().deliver(seed_, config_, target,scores,cost);
  cost.total_ = t2.getTime();
  InteractionUploader::instance().UploadRange(seed_->feed,target.size());
  MCE_INFO("FeedDispatchTask::DispatchFeed fid:"<< seed_->feed <<" source:" << seed_->source << " type:" << stype << " actor:" << seed_->actor
      <<",expr:"<<config_->expr<<",target.size:"<<target.size() 
      << " #cost# " << cost.ToString())
}

void DispatchBigFansSizeFeedTask::handle() {
  map<int, int> scores;
  vector<int> target_v;
  timeval begin,end;
  gettimeofday(&begin,NULL); 
  int i = 1;
  for (set<int>::iterator it = target_.begin(); it != target_.end(); ++it,++i) {
    target_v.push_back(*it);
    map<int,int> scoresPart;
    if (target_v.size() == 5000 || i == target_.size()) {
      try {
       scoresPart = FeedNaiveBayesReplicaAdapter::instance().GetScoreDict(seed_->actor,
           target_v,seed_->type & 0xffff);
      } catch (Ice::Exception& e) {
        MCE_WARN("DispatchBigFansSizeFeedTask::GetBayesScores  call Bayes Exception"
            << " fid:" << seed_->feed << " actor:" << seed_->actor
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
  FeedDeliver::instance().deliver(seed_, config_, target_,scores,cost);
  InteractionUploader::instance().UploadRange(seed_->feed,target_.size());

  gettimeofday(&begin,NULL); 
  float tc2 = (begin.tv_sec - end.tv_sec) * 1000 + (begin.tv_usec - end.tv_usec)*1.0 / 1000;
  MCE_INFO("DispatchBigFansSizeFeedTask::handle --> source:"<<seed_->source<<" stype:"<<(seed_->type & 0xffff)<<" actor:"<<
      seed_->actor<<" target size:"<<target_.size()<<" caculate bayes score cost:"<<tc1<<" deliver cost:"<<tc2);
}


void FeedDispatchTask::handle() {
  ::MyUtil::Str2StrMap::iterator expt = seed_->extraProps.find("MachineDispatch"); 
  if( expt == seed_->extraProps.end()) {//非导入新鲜事,非EDM新鲜事
     WriteDataToMemc();
  }
  //---------------计算分发表达式,向各个cahce服务推送此新鲜事
  if (config_->sendNewsConfig || config_->sendMiniConfig) {
    DispatchFeed();
  }
  //--------------延迟发送reply部分，针对DispatchWithReply
  if (reply_) {
    ReplyDataPtr replyData = new ReplyData();
    UpdateReply2Memc(replyData);
    TaskManager::instance().schedule(new ReplyDelayDispatchTimer(replyData,
          config_));
  }
}
void DispatchReplyToIM::handle() {
  if (reply_->actor > 600000000 && reply_->actor < 700000000) {
    MCE_INFO("DispatchReplyToIM::handle. not send page reply to IM. source:" << reply_->source << " stype:" << reply_->source << " actor:" << reply_->actor);
    return;
  }
  //计算表达式
  set<int> target;
  try {
    target = ExprCache::instance().Expr2Ids(config_->expr, reply_->actor);
  } catch (Ice::Exception& e) {
    MCE_WARN("DispatchReplyToIM::handle --> ExprCache::instance().Expr2Ids() --> exception :" <<e);
  } catch (std::exception& e) {
    MCE_WARN("DispatchReplyToIM::handle --> ExprCache::instance().Expr2Ids() --> exception :" << e.what());
  }
  MCE_INFO("DispatchReplyToIM::handle --> expr:" << config_->expr << " size:" << target.size());

  MyUtil::IntSeq to_ids;
  to_ids.push_back(reply_->actor);
  for (set<int>::iterator it = target.begin(); it != target.end();) {

    to_ids.push_back(*it++);
    if (to_ids.size() == 40000 || it == target.end()) {
      try {
        talk::adapter::WTransitAdapter::instance().deliverFeedReply(reply_,
            to_ids);
        MCE_INFO("DispatchReplyToIM::handle --> Reply to IM, actor:"<< reply_->actor << ",source:" <<reply_->source << ",stype:"<< reply_->stype << ", toid.size:"<<to_ids.size());
        to_ids.clear();
      } catch (Ice::Exception & e) {
        MCE_WARN("DispatchReplyToIM::handle --> call WTransit err:"<<e);
        to_ids.clear();
      }
    }
  }
}


bool ReplyDispatchTask::SetSomeConfig(FeedIndexPtr & idx,FeedConfigPtr & config,map<string, string> & props){
  ostringstream os,propstring;
  os << " fid:" << idx->feed << " stype:" << idx->stype << " actor:" << idx->actor << " source:" << idx->source;

  propstring << "(";
  typedef map<string, string>::value_type vt;
  using namespace boost;
  BOOST_FOREACH(vt & p, props){
    propstring << " " << p.first << ":" << p.second;
  }
  propstring << ")";

  map<string, string>::iterator iter;
  iter = props.find("expr");
  if (iter == props.end()) { //没有找到表达式
    MCE_WARN("ReplyDispatchTask::SetSomeConfig.index "<< os.str()
        << " propstring:" << propstring.str()
        << " prop miss: expr");
    return false;
  }
  config->expr = iter->second;

  iter = props.find("publish_mini");
  if (iter == props.end()) {
    config->sendMiniConfig = 0;
  }else{
    config->sendMiniConfig = (iter->second.compare("1") == 0);
  }

  iter = props.find("publish_news");
  if (iter == props.end()) {
    config->sendNewsConfig = 0;
  }else{
    config->sendNewsConfig = (iter->second.compare("1") == 0);
  }

  iter = props.find("sendconfig_");
  if (iter == props.end()) {
    config->sendConfig = 0;
  }else{
    config->sendConfig = (iter->second.compare("1") == 0);
  }

  iter = props.find("update_time");
  if (iter == props.end()) {
    config->updateTime = 0;
  }else{
    config->updateTime = (iter->second.compare("1") == 0);
  }

  MCE_INFO("ReplyDispatchTask::SetSomeConfig.index "<< os.str()
      << " sendNewsConfig:" << config->sendNewsConfig
      << " sendMiniConfig:" << config->sendMiniConfig
      << " sendConfig:" << config->sendConfig
      << " updatetime:" << config->updateTime
      << " propstring:" << propstring.str()
      )
    return true;
}
void ReplyDispatchTask::handle() { //FeedDispatchNewI::addFeedReply调用
  MCE_INFO("ReplyDispatchTask::handle --> " << actor_);
  TimeStat ts;
  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = FeedDispatchNewI::instance().GetFeedIndex(source_,stype_,actor_); 
  if (indexSeq.empty()) {
    MCE_INFO("ReplyDispatchTask::handle --> get index return empty source:"<<source_<<" stype:"<<stype_<<" actor:"<<actor_);
    return;
  }
  MyUtil::LongSeq feedIds;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    feedIds.push_back(indexSeq.at(i)->feed);
  }

  ReplyLogger::instance().AddReply(indexSeq.at(0)->feed,source_,stype_,actor_,reply_);
  MCE_INFO("ReplyDispatchTask::handle --> get index " << actor_ << " cost" << ts.getTime(true));

  try {
    FeedDbWriterReplicaAdapter::instance().UpdateReply2DB(feedIds,reply_);
  } catch (Ice::Exception& e) {
    MCE_INFO("ReplyDispatchTask::handle --> call FeedDbWriter::UpdateReply2DB err:"<<e);
  }
  try {
    for (size_t i = 0; i < feedIds.size(); i++) {
      FeedContentPtr content =
      FeedMemcClient::instance().GetFeed(feedIds.at(i));
      if (content) {
        content->reply = reply_;
        bool res = FeedMemcClient::instance().SetFeed(content);
        if (!res) {
          MCE_WARN("ReplyDispatchTask::handle() --> FeedMemcClient SetFeed err, " << feedIds.at(i));
        }
      } else {
        MCE_WARN("ReplyDispatchTask::handle() --> FeedMemcClient GetFeed err, " << feedIds.at(i));
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
  }
  MCE_INFO(" ReplyDispatchTask::handle -->set memcached " << actor_ << " cost" << ts.getTime(true));
  
  long feedid = feedIds.at(0);
  InteractionUploader::instance().UploadReply(indexSeq.at(0), reply_->count);
  FeedConfigPtr config = new FeedConfig();

  bool from_db = false;
  map<string, string> props = ConfigCache::instance().Get(feedid);
  if (props.empty()) {
    from_db = true;
    if (DbHelper::instance().isNewFeedidForDb(feedid)) {
      props = DbHelper::instance().getFeedZoneConfigProp(feedid);
    } else {
      props = DbHelper::instance().getFeedConfigProp(feedid);
    }
  }  
  MCE_INFO("ReplyDispatchTask::handle --> get expr: " << props["expr"] << " actor: " << actor_ << " cost: " << ts.getTime(true));

  MCE_INFO("ReplyDispatchTask::handle --> get expr, feed:" << feedIds.at(0) << " source:"<<source_<<" actor:"<<actor_ << " stype:"<<stype_<< " exprsize:" << props.size() << " from_db:" << from_db);

  if(!SetSomeConfig(indexSeq.at(0),config,props)){
    return;
  }

  if (reply_->newReply.empty() && reply_->oldReply.empty()) {
    MCE_INFO("ReplyDispatchTask::handle --> xml empty actor:"
        << actor_ << " source:" << source_ << " stype:" << stype_
        << " newReply:" << reply_->newReply << " oldReply:" << reply_->oldReply
        << " newReplyId:" << reply_->newReplyId << " oldReplyId:" << reply_->oldReplyId << " count:" << reply_->count);
  }

  ReplyDataPtr replyData = new ReplyData();
  replyData->source = source_;
  replyData->stype = stype_;
  replyData->actor = actor_;
  replyData->replyCount = reply_->count;

  replyData->xml = (!(reply_->newReply.empty()) ? reply_->newReply
      : reply_->oldReply);
  replyData->replyId = (!(reply_->newReply.empty()) ? reply_->newReplyId
      : reply_->oldReplyId);
  replyData->time = (int) time(NULL);

  MCE_INFO("ReplyDispatchTask::handle--> dispatch " << replyData->actor << " " << replyData->source << " " << replyData->stype);

  DispatchReplyToIM(replyData,config).handle();

  //Photo issue
  if (replyData->stype == 701 || replyData->stype == 709) {
    Ice::Long photoid = replyData->source;
    if (photoid >= 0) {
      MCE_INFO("ReplyDispatchTask::handle  --> IncCommentCount photo id: "<<photoid<<" line: "<<__LINE__);
      try {
        PhotoDataGatheringAdapter::instance().IncCommentCount(photoid);
      } catch (Ice::Exception& e) {
        MCE_WARN("ReplyDispatchTask::handle -->event photo is replyed first time  err:"<<e);
      }
    } else {
      MCE_INFO("ReplyDispatchTask::handle  --> can not find photo id:  line: "<<__LINE__);
    }
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incReply(timeuse);
}

//---------------------------------------------------------------------------

void ReplyDelayDispatchTimer::handle() {
  if (reply_->xml.empty()) {
    MCE_WARN("ReplyDelayDispatchTask::handle --> xml empty" << reply_->actor << " " << reply_->source << " " << reply_->stype);
  }
  DispatchReplyToIM(reply_,config_).handle();
}

void ReplaceXMLTask::handle() {
  FeedIndexSeq indexSeq = FeedDispatchNewI::instance().GetFeedIndex(source_,stype_,actor_);
  if (indexSeq.empty()) {
    MCE_WARN("ReplaceXMLTask::handle --> get index return empty source:"<<source_<<" stype:"<<stype_<<" actor:"<<actor_);
    return;
  }
  MyUtil::LongSeq feedIds;
  for (size_t h = 0; h < indexSeq.size(); ++h) {
    feedIds.push_back(indexSeq.at(h)->feed);
  }
  try{
    FeedDbWriterReplicaAdapter::instance().ReplaceFeedXML2DB(feedIds,_xml);
    MCE_INFO("ReplaceXMLTask::handle --> call FeedDbWriter first feedid:"<<feedIds[0]);
  } catch (Ice::Exception & e) {
    MCE_WARN("ReplaceXMLTask::handle --> FeedDbWriter::ReplaceFeedXML2DB err:"<<e);
  }
  try {
    for (size_t i = 0; i < feedIds.size(); i++) {
      FeedContentPtr content =
        FeedMemcClient::instance().GetFeed(feedIds.at(i));
      if (content) {
        content->data->xml = _xml;
        bool res =  FeedMemcClient::instance().SetFeed(content);
        if (!res) {
          MCE_INFO("ReplaceXMLTask::handle() --> FeedMemcClient SetFeed FAIED!!! line: "<<__LINE__);
        }
      } else {
        MCE_INFO("ReplaceXMLTask::handle() --> FeedMemcClient GetFeed FAIED!!! Can not add reply to memcache line: "<<__LINE__);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("ReplaceXMLTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("ReplaceXMLTask::handle --> call Memc err, unknown");
  }
}

