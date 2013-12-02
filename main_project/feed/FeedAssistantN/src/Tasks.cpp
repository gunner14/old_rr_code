/*
 * Tasks.cpp
 *
 *  Created on: Dec 24, 2009
 *      Author: antonio
 */

#include "Tasks.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp> 
#include "DbHelper.h"
#include "FeedAssistantI.h"
#include "FeedContentAdapter.h"
#include "FeedMiniAdapter.h"
#include "PerformanceStat.h"
#include "FeedDispatcherReplicaAdapter.h"
#include "FeedAssistantNReplicaAdapter.h"
#include "FeedInteractionNAdapter.h"
#include "../../FeedMemcProxy/client/feed_interaction_client_n.h"
#include "FeedMemcProxy/client/feed_memc_client.h"
#include "FeedMemcProxy/client/feed_index_memc_client.h"
#include "feed/Degrade/src/Degrade.h" 
#include "FeedItemFacadeReplicaAdapter.h"
#include "FeedGroupAdapter.h"
#include "FeedNewsAdapter.h"
#include "pugixml.hpp"
#include "Common.h"

using namespace pugi;
using namespace xce::feed;
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

  ////MyUtil::LongSeq feedIds


//////////////////////////////////////////////////////////////////////////////////////////
void RemoveFeedByTimeTask::handle() {
  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = DbHelper::instance().getFeedIndexByTime(_source, _stype, _actor, _time);
  if (!indexSeq.size()) {
    MCE_WARN("RemoveFeedByTimeTask::handle --> get index fail. source:" << _source << " stype:" << _stype << " actor:" << _actor << " time:" << _time);
    return;
  } else {
    MCE_INFO("RemoveFeedByTimeTask::handle --> get index sucess. source:" << _source << " stype:" << _stype << " actor:" << _actor << " time:" << _time << " indexSeq size:" << indexSeq.size());
  }
  MyUtil::LongSeq ids;
  for(FeedIndexSeq::const_iterator it = indexSeq.begin(); it != indexSeq.end(); it++) {
    ids.push_back((*it)->feed);
    DbHelper::instance().removeFeedMini(_actor, _stype, (*it)->miniMerge);
  }

  MyUtil::LongSeq oldids, newids;
  DbHelper::instance().SplitFeedidsForDb(ids, oldids, newids);

  DbHelper::instance().removeFeedIndex(ids);
  DbHelper::instance().removeFeedContent(oldids);
  DbHelper::instance().removeFeedZoneContent(newids);

  IndexCache::instance().removeFeedByIds(ids);

  try { 
    FIndexMemcClient::instance().RemoveFIndex(_source, _stype); 
  } catch (std::exception& e) { 
    MCE_WARN("RemoveFeedByTimeTask::handle --> call Memc err." << e.what()); 
  } catch (...) { 
    MCE_WARN("RemoveFeedByTimeTask::handle --> call Memc err, unknown"); 
  }

  
  for(FeedIndexSeq::const_iterator it = indexSeq.begin(); it != indexSeq.end(); it++) {
    try {
      bool res = FeedMemcClient::instance().SetEmptyFeed((*it)->feed);
      if (!res) {
        MCE_WARN("RemoveFeedByTimeTask:: --> FeedMemcClient SetEmptyFeed failed  feedid:" << (*it)->feed);
      }
    } catch (std::exception& e) {
      MCE_WARN("RemoveFeedByTimeTask::handle --> call Memc err, " << e.what() << " feedid:" << (*it)->feed);
    } catch (...) {
      MCE_WARN("RemoveFeedByTimeTask::handle --> call Memc err, unknown" << " feedid:" << (*it)->feed);
    }
  }

  try {
    FeedAssistantNReplicaAdapter::instance().cacheSync(_source, _stype, _actor);
  } catch (Ice::Exception e) {
    MCE_WARN("RemoveFeedByTimeTask::handle --> call FeedAssistantNI::cacheSync err:"<<e);
  }
 
  try {
    if ((DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedItemFacade"))) {
      for (int i=0; i < ids.size(); ++i) {
        FeedItemFacadeReplicaAdapter::instance().deleteFeed(_actor,ids[i]);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("RemoveFeedByTimeTask::handle --> call FeedItemFacade err:"<<e<<" source:"<<_source<<" stype:"<<_stype<<" actor:"<<_actor);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmTime(timeuse);
  MCE_INFO("RemoveFeedByTimeTask::handle --> source:" << _source << " stype:" << _stype << " actor:" << _actor << " time;" << _time << " seq:" << indexSeq.size() << " time use:" << timeuse);

}
void RemoveFeedByIdTask::handle() {
  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexPtr index = DbHelper::instance().getFeedIndex(_feedId);
  if (!index) {
    MCE_WARN("RemoveFeedByIdTask::handle --> get index fail. feedId = " << _feedId);
    return;
  } else {
    MCE_INFO("RemoveFeedByIdTask::handle --> get index sucess. feedId = " << _feedId << ", source = " << index-> source << ",stype = " << index->stype << ",actor = " << index->actor);
  }
  MyUtil::LongSeq ids;
  ids.push_back(_feedId);

  MyUtil::LongSeq oldids, newids;
  DbHelper::instance().SplitFeedidsForDb(ids, oldids, newids);

  DbHelper::instance().removeFeedIndex(ids);
  DbHelper::instance().removeFeedContent(oldids);
  DbHelper::instance().removeFeedZoneContent(newids);
  DbHelper::instance().removeFeedMini(index->actor, index->stype,
      index->miniMerge);

  IndexCache::instance().removeFeedByIds(ids);

  try { 
    FIndexMemcClient::instance().RemoveFIndex(index->source, index->stype); 
  } catch (std::exception& e) { 
    MCE_WARN("RemoveFeedByIdTask::handle --> call Memc err." << e.what()); 
  } catch (...) { 
    MCE_WARN("RemoveFeedByIdTask::handle --> call Memc err, unknown"); 
  }



  try {
    bool res = FeedMemcClient::instance().SetEmptyFeed(_feedId);
    if (!res) {
      MCE_WARN("RemoveFeedByIdTask:: --> FeedMemcClient SetEmptyFeed failed feedid:"<<_feedId);
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedByIdTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedByIdTask::handle --> call Memc err, unknown");
  }

  try {
    if ((DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedItemFacade"))) {
      for (int i=0; i < ids.size(); ++i) {
        FeedItemFacadeReplicaAdapter::instance().deleteFeed(index->actor,ids[i]);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("RemoveFeedById::handle --> call FeedItemFacade err:"<<e<<" feedid:"<<_feedId);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmId(timeuse);



}

void RemoveFeedBySourceStypeTask::handle() {

  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = DbHelper::instance().getFeedIndexBySource(_stype,
      _source);
  if (indexSeq.empty()) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle. FeedIndex empty source: " << _source << " stype: " << _stype);
    return;
  }else{
    MCE_INFO("RemoveFeedBySourceStypeTask::handle.get index size:" << indexSeq.size() << " " << indexSeq.at(0)->feed <<" source: " << _source << " stype: " << _stype);
  }
  MyUtil::LongSeq ids;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ids.push_back(indexSeq.at(i)->feed);
  }

  FIndexMemcClient::FIValue index_vec;
  try {
    index_vec = FIndexMemcClient::instance().GetFIndex(_source, _stype);
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err." << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err, unknown");
  }

  try {
    FIndexMemcClient::instance().RemoveFIndex(_source, _stype);
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err." << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err, unknown");
  }

  MyUtil::LongSeq oldids, newids;
  DbHelper::instance().SplitFeedidsForDb(ids, oldids, newids);

  DbHelper::instance().removeFeedIndex(ids);
  DbHelper::instance().removeFeedContent(oldids);
  DbHelper::instance().removeFeedZoneContent(newids);
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    DbHelper::instance().removeFeedMini(indexSeq.at(i)->actor,
        indexSeq.at(i)->stype, indexSeq.at(i)->miniMerge);
  }
  IndexCache::instance().removeFeedByIds(ids);

  try {
    for (size_t i = 0; i < ids.size(); i++) {
      bool res = FeedMemcClient::instance().SetEmptyFeed(ids.at(i));
      if (!res) {
        MCE_INFO("RemoveFeedBySourceStypeTask --> FeedMemcClient SetEmptyFeed failed source: " << _source << " stype: " << _stype);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err, " << e.what() << " source: " << _source << " stype: " << _stype);
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call Memc err, unknown" << " source: " << _source << " stype: " << _stype);
  }

  try {
    if ((DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedItemFacade"))) {
      for (int i=0; i < indexSeq.size(); ++i) {
        FeedItemFacadeReplicaAdapter::instance().deleteFeed(indexSeq[i]->actor,indexSeq[i]->feed);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("RemoveFeedBySourceStypeTask::handle --> call FeedItemFacade err:"<<e<<" source:"<<_source<<" stype:"<<_stype);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmSourceStype(timeuse);
}


void RemoveFeedBySourceStypeHSTask::handle() {

  timeval beg, end;
  gettimeofday(&beg, NULL);

  MyUtil::LongSeq ids;

  FIndexMemcClient::FIValue index_vec;
  try {
    index_vec = FIndexMemcClient::instance().GetFIndex(_source, _stype);
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeHSTask::handle --> call Memc err." << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeHSTask::handle --> call Memc err, unknown");
  }

  if(index_vec.empty()) {
    FeedIndexSeq indexSeq = DbHelper::instance().getFeedIndexBySource(_stype,
        _source);
    if (indexSeq.empty()) {
      MCE_WARN("RemoveFeedBySourceStypeHSTask::handle get FeedIndex from db empty source: " << _source << " stype: " << _stype);
      return;
    }
    for(FeedIndexSeq::const_iterator it = indexSeq.begin(); it != indexSeq.end(); it++) {
      FIndexMemcClient::MemcValueItem mvi = {
        actor: (*it)->actor, 
        feedid: (*it)->feed,
        miniMerge: (*it)->miniMerge };
      index_vec.push_back(mvi);
    }
  }

  for(FIndexMemcClient::FIValue::const_iterator it = index_vec.begin(); it != index_vec.end(); it++) {
    ids.push_back(it->feedid);
  }


  try {
    FIndexMemcClient::instance().RemoveFIndex(_source, _stype);
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeHSTask::handle --> call Memc err." << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeHSTask::handle --> call Memc err, unknown");
  }


  MyUtil::LongSeq oldids, newids;
  DbHelper::instance().SplitFeedidsForDb(ids, oldids, newids);

  DbHelper::instance().removeFeedIndex(ids);
  DbHelper::instance().removeFeedContent(oldids);
  DbHelper::instance().removeFeedZoneContent(newids);

  for(FIndexMemcClient::FIValue::const_iterator it = index_vec.begin(); it != index_vec.end(); it++) {
    DbHelper::instance().removeFeedMini(it->actor, _stype, it->miniMerge);
  }

  IndexCache::instance().removeFeedByIds(ids);
  try {
    for (size_t i = 0; i < ids.size(); i++) {
      bool res = FeedMemcClient::instance().SetEmptyFeed(ids.at(i));
      if (!res) {
        MCE_WARN("RemoveFeedBySourceStypeHSTask --> FeedMemcClient SetEmptyFeed failed source: " << _source << " stype: " << _stype);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedBySourceStypeHSTask::handle --> call Memc err, " << e.what() << " source: " << _source << " stype: " << _stype);
  } catch (...) {
    MCE_WARN("RemoveFeedBySourceStypeHSTask::handle --> call Memc err, unknown" << " source: " << _source << " stype: " << _stype);
  }

  try {
    if ((DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedItemFacade"))) {
      for(FIndexMemcClient::FIValue::const_iterator it = index_vec.begin(); it != index_vec.end(); ++it) {
        FeedItemFacadeReplicaAdapter::instance().deleteFeed(it->actor,it->feedid);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("RemoveFeedBySourceStypeHSTask::handle --> call FeedItemFacade err:"<<e<<" source:"<<_source<<" stype:"<<_stype);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmSourceStype(timeuse);
}

void RemoveFeedBySourceStypeActorTask::handle() {

  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = DbHelper::instance().getFeedIndex(_stype, _source,
      _actor);
  if (indexSeq.empty()) {
    MCE_INFO("RemoveFeedBySourceStypeActorTask::handle -->  getFeedIndex result empty ,stype = " << _stype << ",source = " << _source << ",actor = " << _actor);
    return;
  } else {
    MCE_INFO("RemoveFeedBySourceStypeActorTask::handle -->  getFeedIndex result sucess, size = " << indexSeq.size()
        << ",stype = " << _stype << ",source = " << _source << ",actor = " << _actor);
  }
  MyUtil::LongSeq ids;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ids.push_back(indexSeq.at(i)->feed);
  }

  MyUtil::LongSeq oldids, newids;
  DbHelper::instance().SplitFeedidsForDb(ids, oldids, newids);

  DbHelper::instance().removeFeedIndex(ids);
  DbHelper::instance().removeFeedContent(oldids);
  DbHelper::instance().removeFeedZoneContent(newids);
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    DbHelper::instance().removeFeedMini(indexSeq.at(i)->actor,
        indexSeq.at(i)->stype, indexSeq.at(i)->miniMerge);
  }
  IndexCache::instance().removeFeedByIds(ids);

  try {
    for (size_t i = 0; i < ids.size(); i++) {
      MCE_INFO("RemoveFeedBySourceStypeActorTask::handle --> FeedMemcClient SetEmptyFeed feed:" << ids.at(i));
      bool res = FeedMemcClient::instance().SetEmptyFeed(ids.at(i));
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
    if ((DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedItemFacade"))) {
      for (int i=0; i < indexSeq.size(); ++i) {
        FeedItemFacadeReplicaAdapter::instance().deleteFeed(_actor,indexSeq[i]->feed);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("RemoveFeedBySourceStypeActorTask::handle --> call FeedItemFacade err:"<<e<<" source:"<<_source<<" stype:"<<_stype<<" actor:"<<_actor);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmSourceStypeActor(timeuse);
}

void RemoveFeedByParentTask::handle() {
  timeval beg, end;
  gettimeofday(&beg, NULL);

  FeedIndexSeq indexSeq = DbHelper::instance().getFeedIndexByPsource(_ptype,
      _psource);
  if (indexSeq.empty()) {
    return;
  }
  MyUtil::LongSeq ids;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    ids.push_back(indexSeq.at(i)->feed);
  }

  MyUtil::LongSeq oldids, newids;
  DbHelper::instance().SplitFeedidsForDb(ids, oldids, newids);

  DbHelper::instance().removeFeedIndex(ids);
  DbHelper::instance().removeFeedContent(oldids);
  DbHelper::instance().removeFeedZoneContent(newids);
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    DbHelper::instance().removeFeedMini(indexSeq.at(i)->actor,
        indexSeq.at(i)->stype, indexSeq.at(i)->miniMerge);

    try {
      FIndexMemcClient::instance().RemoveFIndex(indexSeq.at(i)->source, indexSeq.at(i)->stype);
    } catch (std::exception& e) {
      MCE_WARN("RemoveFeedByParentTask::handle --> call Memc err." << e.what());
    } catch (...) {
      MCE_WARN("RemoveFeedByParentTask::handle --> call Memc err, unknown");
    }

  }
  IndexCache::instance().removeFeedByIds(ids);

  try {
    for (size_t i = 0; i < ids.size(); i++) {
      bool res = FeedMemcClient::instance().SetEmptyFeed(ids.at(i));
      if (!res) {
        MCE_INFO("RemoveFeedByParentTask::handle --> FeedMemcClient SetEmptyFeed failed feedid:"<<ids.at(i));
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("RemoveFeedByParent::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("RemoveFeedByParent::handle --> call Memc err, unknown");
  }

  try {
    if ((DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedItemFacade"))) {
      for (int i=0; i < indexSeq.size(); ++i) {
        FeedItemFacadeReplicaAdapter::instance().deleteFeed(indexSeq[i]->actor,indexSeq[i]->feed);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("RemoveFeedByParentTask::handle --> call FeedItemFacade err:"<<e<<" psource:"<<_psource<<" ptype:"<<_ptype);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incRmParent(timeuse);
}

void ReadMiniTask::handle() {
}

void ReadNewsTask::handle() {
}


void FeedDispatchTask::handle() {//dispatch 和 dispatchWithReply共同调用

  timeval tv_start, tv_end;
  gettimeofday(&tv_start, NULL);

  if (!_seed) {
    MCE_WARN("FeedDispatchTask::handle --> seed is null");
    return;
  }
  MCE_INFO("FeedDispatchTask::handle --> feed="<<_seed->feed<<"   time="<<_seed->time);

  //for a bug , 701 and 709 set publish_new and publish_mini to 1
  //zhi.lin:  what bug?? for stop publishing app_photo to mini,I comment this line : _seed->extraProps["publish_mini"] = "1";
  int stype = _seed->type & 0xffff;
  if (stype == 701 || stype == 709) {
    _seed->extraProps["publish_news"] = "1";
    //_seed->extraProps["publish_mini"] = "1";       
  }
  FeedConfigPtr config = new FeedConfig();
  config->expr = _seed->extraProps["expr"];
  config->updateTime = _seed->extraProps["update_time"] == "1";
  config->sendConfig = _seed->extraProps.find("send_config")
      != _seed->extraProps.end();
  config->sendNewsConfig = _seed->extraProps["publish_news"] == "1";
  config->sendMiniConfig = _seed->extraProps["publish_mini"] == "1";

  bool saving = _seed->extraProps["save_db"] != "0";
  ConfigCache::instance().Add(_seed->feed, _seed->extraProps);

  FeedIndexPtr index = new FeedIndex;
  index->feed = _seed->feed;
  index->newsMerge = _seed->newsMerge;
  index->miniMerge = _seed-> miniMerge;
  index->source = _seed->source;
  index->psource = _seed->psource;
  index->actor = _seed->actor;
  index->time = _seed->time;
  index->stype = _seed->type & 0xffff;
  index->ptype = _seed->ptype;

  IndexCache::instance().add(index);

  bool isNormal = FeedAssistantI::instance().isNormalFeedType(index->stype);
  if(isNormal) {
    try {
      bool suc = FIndexMemcClient::instance().AddFIndex(index->source, index->stype, index->actor, index->feed, index->miniMerge);
      if(!suc) {
        MCE_WARN("FeedDispatchTask::handle --> AddFIndex error.source:" << index->source << " stype:" << index->stype << " actor:" << index->actor << " feed:" << index->feed << " miniMerge:" << index->miniMerge);
      }
    } catch (std::exception& e) {
      MCE_WARN("FeedDispatchTask::handle --> call Memc err." << e.what());
    } catch (...) {
      MCE_WARN("FeedDispatchTaskTask::handle --> call Memc err, unknown");
    }

  } else {
    try {
      bool suc = FIndexMemcClient::instance().AppendFIndex(index->source, index->stype, index->actor, index->feed, index->miniMerge);
      if(!suc) {
        MCE_WARN("FeedDispatchTask::handle --> AppendFIndex no index."<< index->source << " stype:" << index->stype << " actor:" << index->actor << " feed:" << index->feed << " miniMerge:" << index->miniMerge);
      }
    } catch (std::exception& e) {
      MCE_WARN("FeedDispatchTask::handle --> call Memc err." << e.what());
    } catch (...) {
      MCE_WARN("FeedDispatchTask::handle --> call Memc err, unknown");
    }

  }

  timeval s1, s2, e1, e2;
  gettimeofday(&s1, NULL);
  try {
    DbHelper::instance().addFeedIndex(index);
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchTask::handle --> addFeedIndex feed:" << index->feed << " err, " << e.what());
  }
  gettimeofday(&e1, NULL);

  //  To content
  gettimeofday(&s2, NULL);
  try {
    if (saving && (config->sendNewsConfig || config->sendMiniConfig
        || _seed->extraProps.find("togroup") != _seed->extraProps.end())) {//togroup肯定要存db
      if (DbHelper::instance().isNewFeedidForDb(_seed->feed)) {
        DbHelper::instance().addZoneFeedContent(_seed);
      } else {
        DbHelper::instance().addFeedContent(_seed);
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchTask::handle --> addFeedContent feed:" << index->feed << " err, " << e.what());
  }
  gettimeofday(&e2, NULL);

  float t1 = 1000000 * (e1.tv_sec - s1.tv_sec) + e1.tv_usec - s1.tv_usec;
  float t2 = 1000000 * (e2.tv_sec - s2.tv_sec) + e2.tv_usec - s2.tv_usec;
  t1 /= 1000;
  t2 /= 1000;
  MCE_INFO("FeedDispatchTask::handle --> db cost index:" << t1 << " content:" << t2);

  FeedDataPtr data = new FeedData();
  data->feed = _seed->feed;
  data->source = _seed->source;
  data->actor = _seed->actor;
  data->type = _seed->type;
  data->xml = _seed->xml;
  data->time = _seed->time;
   
  if (_seed->extraProps.find("base_weight_id") != _seed->extraProps.end()) {
    try{
      unsigned int baseWeightId  = boost::lexical_cast<int>(
          _seed->extraProps["base_weight_id"]);
      data->weight = (baseWeightId << 16) & 0x00ff0000;
    } catch (std::exception & e) {
      MCE_WARN("FeedAssistantN::dispatch --> cast base_weight_id err:"<<e.what() << " feed:" << _seed->feed <<",actor"<< _seed->actor << " stype:" << (_seed->type&0xFFFF));
    }
  }

  try {
    FeedContentPtr content = new FeedContent();
    content->data = data;
    bool res = FeedMemcClient::instance().SetFeed(content);
    if (!res) {
      MCE_WARN("FeedDispatchTask::handle --> FeedMemcClient SetEmptyFeed failed feed:"<<data->feed);
    }
  } catch (std::exception& e) {
    MCE_WARN("FeedDispatchTask::handle --> call Memc err, " << e.what());
  } catch (...) {
    MCE_WARN("FeedDispatchTask::handle --> call Memc err, unknown");
  }

  MCE_INFO("FeedDispatchTask::handle --> fid:" << _seed->feed << ", newsMerge:"<<_seed->newsMerge<<", miniMerge:"<<_seed->miniMerge
      << ", source:"<<_seed->source<<", psource:"<<_seed->psource<<", actor:"<<_seed->actor<<", time:"<<_seed->time
      << ", stype:"<<index->stype<< ", ptype:"<<_seed->ptype << ", seed->type:"<< _seed->type << ", news_merge_type:"
      << int((_seed->type >> 30)& 0x00000003 )<< ", mini_merge_type:"<< int((_seed->type & 0x3FFFFFFF) >> 28) << ", weight:" << _seed->baseWeight
      << ", updateTime:"<<config->updateTime << ", send_config:"<<config->sendConfig<<", news_publish:"
      <<config->sendNewsConfig <<", mini_publish:"<< config->sendMiniConfig << ", expr:"<< config->expr << " saving:" << saving);

  // To Mini DB
  if (config->sendMiniConfig) {
    try{
      DbHelper::instance().addFeedMini(_seed);
    }catch(std::exception & e){

      MCE_WARN("FeedDispatchTask::handle --> insert minifeed db , line:" << __LINE__ << ", err:" << e.what());
    }
  }

  if (_seed->extraProps.find("togroup") != _seed->extraProps.end()) {
    int groupid = 0;
    try {
      groupid = boost::lexical_cast<int>(_seed->extraProps["togroup"]);
    } catch (...) {
      MCE_WARN("FeedDispatchTask::handle --> lexical cast err, " << _seed->extraProps["togroup"]);
    }
    try {
      MCE_INFO("FeedDispatchTask::handle --> send to group, actor:" << _seed->actor << " feed:" <<_seed->feed << " group:" << groupid);
      DbHelper::instance().addFeedGroup(groupid, _seed);
    } catch (...) {
      MCE_WARN("FeedDispatchTask::handle --> add to db err");
    }
    FeedItem item;
    MyUtil::Int2IntMap id2Weight;
    item.feed = _seed->feed;
    item.type = _seed->type;
    item.time = _seed->time;
    item.weight = _seed->baseWeight;
    item.merge = _seed->miniMerge;
    item.actor = _seed->actor;
    id2Weight[groupid] = item.weight;
    try {
      if (DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedGroup")) {
        FeedGroupAdapter::instance().addFeed(item, config, id2Weight);
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedDispatchTask::handle --> send to group, actor:" << _seed->actor << " feed:" <<_seed->feed << " group:" << groupid << " err, " << e);
    }
  }

  if (config->sendNewsConfig || config->sendMiniConfig) {
    try {
      FeedDispatcherReplicaAdapter::instance().dispatch(_seed, config);
    } catch (Ice::Exception & e) {
      MCE_WARN("ICE_Exception!! err e: "<<e<<"File: "<<__FILE__<<"Line: "<<__LINE__);
    }
  } else {
    MCE_WARN("FeedDispatchTask::handle --> not dispatch  feed:"<<_seed->feed << ", news_config:"<<config->sendNewsConfig<<", mini_config:"<<config->sendMiniConfig);
  }

  //延迟发送reply部分，针对DispatchWithReply
  if (_reply) {
    ReplyLogger::instance().AddReply(_seed->feed,_seed->source,_seed->type & 0xffff,_seed->actor,_reply);
    ReplyDataPtr replyData = new ReplyData();
    replyData->source = _seed->source;
    replyData->stype = _seed->type & 0xffff;
    replyData->actor = _seed->actor;
    replyData->replyCount = _reply->count;
    replyData->time = _seed->time;//(int)time(NULL);


    replyData->xml = (!(_reply->newReply.empty()) ? _reply->newReply
        : _reply->oldReply);
    replyData->replyId = (!(_reply->newReply.empty()) ? _reply->newReplyId
        : _reply->oldReplyId);

    try {
      MyUtil::LongSeq feedIds;
      feedIds.push_back(_seed->feed);
      FeedReplyBufferTimer::instance().changeReply(feedIds, _reply);
    } catch (Ice::Exception& e) {
      MCE_WARN("ReplyDispatchTask::handle --> call FeedContentAdapter replaceFeedReply  err:"<<e);
    }
    try {
      FeedContentPtr content = FeedMemcClient::instance().GetFeed(_seed->feed);
      if (content) {
        content->reply = _reply;
        bool res = FeedMemcClient::instance().SetFeed(content);
        if (!res) {
          MCE_WARN("FeedDispatchTask::handle() --> FeedMemcClient SetFeed err, " << _seed->feed);
        }
      } else {
        MCE_WARN("FeedDispatchTask::handle() --> FeedMemcClient GetFeed err, " << _seed->feed);
      }
    } catch (std::exception& e) {
      MCE_WARN("FeedDispatchTask::handle --> call Memc err, " << e.what());
    } catch (...) {
      MCE_WARN("FeedDispatchTask::handle --> call Memc err, unknown");
    }

    TaskManager::instance().schedule(new ReplyDelayDispatchTimer(replyData,
        config));
  }
  gettimeofday(&tv_end, NULL);
  float timeuse = 1000000 * (tv_end.tv_sec - tv_start.tv_sec) + tv_end.tv_usec
      - tv_start.tv_usec;
  timeuse /= 1000;
  PerformanceStat::instance().incDispatch(timeuse);
  MCE_INFO("FeedDispatchTask::handle --> cost:" << timeuse);

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

  iter = props.find("send_config");
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
void ReplyDispatchTask::handle() { //FeedAssistantI::addFeedReply调用
  MCE_INFO("ReplyDispatchTask::handle --> " << _actor);

  timeval beg, end;
  gettimeofday(&beg, NULL);
  FeedIndexSeq indexSeq = IndexCache::instance().getFeedIndexSeq(_source,
      _stype, _actor);
  MyUtil::LongSeq feedIds;
  MyUtil::Long2IntMap interactions;
  for (size_t i = 0; i < indexSeq.size(); ++i) {
    feedIds.push_back(indexSeq.at(i)->feed);
    interactions[indexSeq.at(i)->feed] = 1;
  }

  // index memcache
  FIndexMemcClient::FIValue index_vec;
  try {
    index_vec = FIndexMemcClient::instance().GetFIndex(_source, _stype);
  } catch (std::exception& e) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err." << e.what());
  } catch (...) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
  }

  TimeStat ts;
  if (indexSeq.empty() || index_vec.empty()) { ////////

    indexSeq = DbHelper::instance().getFeedIndex(_stype, _source, _actor);
    if (indexSeq.empty()) {
      indexSeq = DbHelper::instance().getFeedIndexBC(_stype, _source, _actor);
      if(indexSeq.empty()) {
         MCE_WARN("ReplyDispatchTask::handle ---> can not get feed, source:"<<_source<<" actor:"<<_actor << " stype:"<<_stype);
         return;
      }
    }
    for (size_t h = 0; h < indexSeq.size(); ++h) {
      feedIds.push_back(indexSeq.at(h)->feed);
      interactions[indexSeq.at(h)->feed] = 1;
    }
    IndexCache::instance().add(indexSeq);

    if(index_vec.empty()) {
      MCE_INFO("ReplyDispatchTask::handle --> add indexs to memc. source:" << _source << " stype:" << _stype << " size:" << indexSeq.size());
      for(FeedIndexSeq::const_iterator it = indexSeq.begin(); it != indexSeq.end(); it++) {
        if(it == indexSeq.begin()) { 
          try {
            bool suc = FIndexMemcClient::instance().AddFIndex(_source, _stype, _actor, (*it)->feed, (*it)->miniMerge);
            if(!suc) {
              MCE_WARN("ReplyDispatchTask::handle --> AddFIndex error.source:" << _source << " stype:" << _stype << " actor:" << _actor << " feed:" << (*it)->feed << " miniMerge:" << (*it)->miniMerge);
              break;
            }
          } catch (std::exception& e) {
            MCE_WARN("ReplyDispatchTask::handle --> call Memc err." << e.what());
            break;
          } catch (...) {
            MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
            break;
          }
          continue;
        }

        bool suc = false;
        try {
          suc = FIndexMemcClient::instance().AppendFIndex(_source, _stype, _actor, (*it)->feed, (*it)->miniMerge);
          if(!suc) {
            MCE_WARN("FeedDispatchTask::handle --> AppendFIndex error."<< _source << " stype:" << _stype << " actor:" << _actor << " feed:" << (*it)->feed << " miniMerge:" << (*it)->miniMerge);
          }
        } catch (std::exception& e) {
          MCE_WARN("ReplyDispatchTask::handle --> call Memc err." << e.what());
          suc = false;
          
        } catch (...) {
          MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
          suc = false;
        }

        if(!suc) {
          try {
            FIndexMemcClient::instance().RemoveFIndex(_source, _stype);
          } catch (std::exception& e) {
            MCE_WARN("ReplyDispatchTask::handle --> call Memc err." << e.what());
          } catch (...) {
            MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
          }
          break;
        }

      }
    }
  }
  if (feedIds.empty()) {
    MCE_WARN("ReplayDispatchTask::handle --> can not find feed id,  source:"<<_source<<" actor:"<<_actor << " stype:"<<_stype);
    return;
  }

  ReplyLogger::instance().AddReply(indexSeq.at(0)->feed,_source,_stype,_actor,_reply);
  MCE_INFO("ReplyDispatchTask::handle get index " << _actor << " cost" << ts.getTime());

  try {
    FeedReplyBufferTimer::instance().changeReply(feedIds, _reply);
  } catch (Ice::Exception& e) {
    MCE_WARN("ReplyDispatchTask::handle --> call FeedContentAdapter replaceFeedReply  err:"<<e);
  }

  try {
    for (size_t i = 0; i < feedIds.size(); i++) {
      FeedContentPtr content =
          FeedMemcClient::instance().GetFeed(feedIds.at(i));
      if (content) {
        content->reply = _reply;
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

  long feedid = feedIds.at(0);
  InteractionUploader::instance().UploadReply(indexSeq.at(0), _reply->count);
  FeedConfigPtr config = new FeedConfig();

  ts.reset();
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

  MCE_INFO("ReplyDispatchTask::handle ---> get expr, feed:" << feedIds.at(0) << " source:"<<_source<<" actor:"<<_actor << " stype:"<<_stype<< " exprsize:" << props.size() << " from_db:" << from_db);

  if(!SetSomeConfig(indexSeq.at(0),config,props)){
    return;
  }

  if (_reply->newReply.empty() && _reply->oldReply.empty()) {
    MCE_INFO("ReplyDispatchTask::handle --> xml empty actor:"
        << _actor << " source:" << _source << " stype:" << _stype
        << " newReply:" << _reply->newReply << " oldReply:" << _reply->oldReply
        << " newReplyId:" << _reply->newReplyId << " oldReplyId:" << _reply->oldReplyId << " count:" << _reply->count);
  }

  ReplyDataPtr replyData = new ReplyData();
  replyData->source = _source;
  replyData->stype = _stype;
  replyData->actor = _actor;
  replyData->replyCount = _reply->count;

  replyData->xml = (!(_reply->newReply.empty()) ? _reply->newReply
      : _reply->oldReply);
  replyData->replyId = (!(_reply->newReply.empty()) ? _reply->newReplyId
      : _reply->oldReplyId);
  replyData->time = (int) time(NULL);

  try {
    FeedDispatcherReplicaAdapter::instance().dispatchReply(replyData, config);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedAssistantI::addFeedReply --> call FeedDispatcherReplicaAdapter dispatchReply  err:"<<e);
  }

  gettimeofday(&end, NULL);
  float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
  timeuse/=1000;
  PerformanceStat::instance().incReply(timeuse);
}

//---------------------------------------------------------------------------

void ReplyDispatchWithConfigTask::handle() {
  MCE_INFO("ReplyDispatchWithConfigTask::handle --> "
      "source:" <<_source << " stype:" << _stype <<" actor:" << _actor);
  if (!IndexLocator::LocateFids(_source, _stype, _actor, fids_)
      || fids_.empty()) {
    MCE_WARN("ReplyDispatchWithConfigTask::handle. Locate fids fail");
    return;
  }
  if (1) {//调老接口
    TaskManager::instance().execute(new ReplyDispatchTask(_source, _stype,
        _actor, _reply));
  } else {
    SetInteraction();
    MemcHandler::UpdateReply(fids_, _reply);
    SetReplyBufferTimer();
    SendToFeedDispatcher();
  }
  RefleshMiniGroupTime();
}

int ReplyDispatchWithConfigTask::ParseMiniGroupId() {
  map<string, string>::iterator i = config_.find("minigroupid");
  if (i == config_.end()) {
    MCE_DEBUG("ReplyDispatchWithConfigTask::ParseMiniGroupId. No key:minigroupid" );
    return 0;
  } else {
    try {
      MCE_DEBUG("Key = " << i->second);
      return boost::lexical_cast<long>(i->second);
    } catch (boost::bad_lexical_cast & e) {
      MCE_INFO("ReplyDispatchWithConfigTask::ParseMiniGroupId" << e.what() << ",minigroupid:" << i->second);
      return 0;
    }
  }
}

bool ReplyDispatchWithConfigTask::SetFeedMemc() {
  try {
    for (size_t i = 0; i < fids_.size(); i++) {
      //if(feedIds.at(i)%10 < 3){
      FeedContentPtr content = FeedMemcClient::instance().GetFeed(fids_.at(i));
      if (content) {
        content->reply = _reply;
        bool res = FeedMemcClient::instance().SetFeed(content);
        if (!res) {
          MCE_INFO("ReplyDispatchTask::handle() --> FeedMemcClient SetFeed FAIED!!! line: "<<__LINE__);
          return false;
        } else {
          return true;
        }
      } else {
        MCE_INFO("ReplyDispatchTask::handle() --> FeedMemcClient GetFeed FAIED!!! Can not add reply to memcache line: "<<__LINE__);
        return false;
      }
    }
  } catch (std::exception& e) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, " << e.what());

  } catch (...) {
    MCE_WARN("ReplyDispatchTask::handle --> call Memc err, unknown");
  }
  return false;
}

bool ReplyDispatchWithConfigTask::SetReplyBufferTimer() {
  try {
    FeedReplyBufferTimer::instance().changeReply(fids_, _reply);
    return true;
  } catch (Ice::Exception& e) {
    MCE_INFO("ICE_Exception!! err e: "<<e<<"File: "<<__FILE__<<"Line: "<<__LINE__);
    MCE_WARN("ReplyDispatchTask::handle --> call FeedContentAdapter replaceFeedReply  err:"<<e);
    return false;
  }
}
bool ReplyDispatchWithConfigTask::SendToFeedDispatcher() {
  FeedConfigPtr config = FeedConfigLocator::Locate(fids_.at(0));
  if (!GetReplyData()) {
    return false;
  }
  try {
    FeedDispatcherReplicaAdapter::instance().dispatchReply(reply_data_, config);
    return true;
  } catch (Ice::Exception& e) {
    MCE_INFO("ICE_Exception!! err e: "<<e<<"File: "<<__FILE__<<"Line: "<<__LINE__);
    MCE_WARN("FeedAssistantI::addFeedReply --> call FeedDispatcherReplicaAdapter dispatchReply  err:"<<e);
    return false;
  }
}
ReplyDataPtr ReplyDispatchWithConfigTask::GetReplyData() {
  reply_data_ = ReplyDataBuilder::Build(_source, _stype, _actor, _reply);
  return reply_data_;
}

bool ReplyDispatchWithConfigTask::SetInteraction() {
  MyUtil::Long2IntMap interactions;
  for (size_t i = 0; i < fids_.size(); ++i) {
    interactions[fids_.at(i)] = 1;
  }
//  try {
//    FeedInteractionNAdapter::instance().IncBatch(INTERACTION_REPLY,
//        interactions);
//    return true;
//  } catch (Ice::Exception& e) {
//    MCE_INFO("ICE_Exception!! err e: "<<e<<"File: "<<__FILE__<<"Line: "<<__LINE__);
//    MCE_WARN("ReplyDispatchTask::SetInteraction ---> call FeedInteraction err, " << e);
//  }
  //FeedInteractionMemcClient::instance().IncRelplyCounts(interactions);
  return false;
}

bool ReplyDispatchWithConfigTask::RefleshMiniGroupTime() {
  if (config_.empty()) {
    MCE_WARN("ReplyDispatchWithConfigTask::RefleshMiniGroupTime. empty config");
    return false;
  }
  int gid = ParseMiniGroupId();
  if (!gid) {
    MCE_WARN("ReplyDispatchWithConfigTask::RefleshMiniGroupTime. fail Parse minigroupid");
    return false;
  }
  try {
    if (DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedGroup")) {
      FeedGroupAdapter::instance().RefreshFeed(gid, fids_.at(0), time(NULL));
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("ReplyDispatchWithConfigTask::RefleshMiniGroupTime. Fail RefreshFeed:"<<e);
  }

  try {
    if(!_reply){
      MCE_INFO("ReplyDispatchWithConfigTask::RefleshMiniGroupTime. _reply empty. " );
      return true;
    }
    int replier = 0;
    if(!_reply->newReply.empty()){
      replier = ReplyXmlParser::instance().ParseReplier(_reply->newReply);
    }else  if(!_reply->oldReply.empty()){
      replier = ReplyXmlParser::instance().ParseReplier(_reply->oldReply);
    }else{
      MCE_WARN("ReplyDispatchWithConfigTask::RefleshMiniGroupTime. new and old reply xml empty. " );
      return false;
    }
    if(!replier){
      MCE_INFO("ReplyDispatchWithConfigTask::RefleshMiniGroupTime. replier empty. " );
      return true;
    } 
    if (DegradeManager::instance().IsEnable("FeedAssistantN::module_FeedGroup")) {
      FeedGroupAdapter::instance().ReplyFeed(gid, fids_.at(0), time(NULL),_actor,replier);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("ReplyDispatchWithConfigTask::RefleshMiniGroupTime. Fail RefreshFeed:"<<e);
  }catch (std::exception& e) {
    MCE_WARN("ReplyDispatchWithConfigTask::RefleshMiniGroupTime  err, " << e.what());
  }catch(...){
    MCE_WARN("ReplyDispatchWithConfigTask::RefleshMiniGroupTime  err ...");
  }
  return true;
}

void ReplyDelayDispatchTimer::handle() {
  try {
    if (_reply->xml.empty()) {
      MCE_WARN("ReplyDelayDispatchTask::handle --> xml empty" << _reply->actor << " " << _reply->source << " " << _reply->stype);
    }
    FeedDispatcherReplicaAdapter::instance().dispatchReply(_reply, _config);
  } catch (Ice::Exception& e) {
    MCE_INFO("ICE_Exception!! err e: "<<e<<"File: "<<__FILE__<<"Line: "<<__LINE__);
    MCE_WARN("ReplyDelayDispatchTimer::handle --> call FeedDispatcherReplicaAdapter dispatchReply  err:"<<e);
  }
}

void ReplaceXMLTask::handle() {
  FeedIndexSeq indexSeq = IndexCache::instance().getFeedIndexSeq(_source,
      _stype, _actor);
  MCE_INFO("ReplaceXMLTask::handle --> get from IndexCache, indexSeq size = " << indexSeq.size());
  MyUtil::LongSeq feedIds;
  if (indexSeq.empty()) {
    indexSeq = DbHelper::instance().getFeedIndex(_stype, _source, _actor);
    if (indexSeq.empty()) {
      MCE_INFO("ReplaceXMLTask::handle ---> can not get feed index , source:"<<_source<<" actor:"<<_actor << " stype:"<<_stype);
      return;
    } else {
      IndexCache::instance().add(indexSeq);//放入indexcache
      MCE_INFO("ReplaceXMLTask::handle ---> get feed index from db sucess, source:"<<_source<<" actor:"<<_actor << " stype:"<<_stype << " ,index seq size = " << indexSeq.size());
    }
  }
  for (size_t h = 0; h < indexSeq.size(); ++h) {
    feedIds.push_back(indexSeq.at(h)->feed);
  }

  MyUtil::LongSeq oldids, newids;
  DbHelper::instance().SplitFeedidsForDb(feedIds, oldids, newids);

  try {
    DbHelper::instance().ReplaceXML(oldids, _xml);
    DbHelper::instance().ReplaceZoneXML(newids, _xml);
  } catch (Ice::Exception& e) {
    MCE_WARN("ReplaceXMLTask::handle --> call DbHelper ReplaceXML  err:"<<e);
  }
  try {
    for (size_t i = 0; i < feedIds.size(); i++) {
      FeedContentPtr content =
          FeedMemcClient::instance().GetFeed(feedIds.at(i));
      if (content) {
        content->data->xml = _xml;
        bool res = FeedMemcClient::instance().SetFeed(content);
        if (!res) {
          MCE_WARN("ReplaceXMLTask::handle() --> FeedMemcClient SetFeed failed feed:"<<content->data->feed);
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

void FeedReplyBufferTimer::changeReply(vector<Ice::Long> feeds,
    const FeedReplyPtr& reply) {
  if (!reply) {
    return;
  }
  for (unsigned i = 0; i < feeds.size(); i++) {
    if (feeds.at(i) > 0) {
      IceUtil::Mutex::Lock lock(mutex_);
      replys_[feeds.at(i)] = reply;
    }
  }
}

void FeedReplyBufferTimer::handle() {
  map<Ice::Long, FeedReplyPtr> buffer;
  {
    IceUtil::Mutex::Lock lock(mutex_);
    replys_.swap(buffer);
  }
  MCE_INFO("FeedReplyBufferTimer::handle --> reply size:" << buffer.size());
  if (buffer.empty()) {
    return;
  }
  try {
    map<Ice::Long, FeedReplyPtr> oldbf, newbf;
    for(map<Ice::Long, FeedReplyPtr>::const_iterator it = buffer.begin(); it != buffer.end(); ++it) {
      if (DbHelper::instance().isNewFeedidForDb(it->first)) {
        newbf.insert(make_pair(it->first, it->second));
      } else {
        oldbf.insert(make_pair(it->first, it->second));
      }
    }
    DbHelper::instance().UpdateReply(oldbf);
    DbHelper::instance().UpdateZoneReply(newbf);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedReplyBufferTimer::handle --> call DbHelper err:"<<e);
  }
}
