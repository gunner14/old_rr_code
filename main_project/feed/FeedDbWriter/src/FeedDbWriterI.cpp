/*
 * FeedDbWriterI.cpp
 *
 */

#include "FeedDbWriterI.h"
#include "ServiceI.h"
#include "DbHelper.h"
#include "TimeLineFeedReplicaAdapter.h"
#include "FeedItemFacadeReplicaAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "FeedPipe.h"
#include <Ice/Exception.h>
#include <exception>
#include <unistd.h>
using namespace com::xiaonei::xce;
using namespace xce::feed;
using namespace ::xce::serverstate; 
using namespace std;
using namespace boost;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedDbWriterI::instance(),
      service.createIdentity(kFeedDbWriter, ""));
   MyUtil::ThreadPoolConfig dbw(10,100);
   MyUtil::ThreadPoolConfig tpc(4,4);
   TaskManager::instance().config(kTaskDbWriterLevel,dbw);
   TaskManager::instance().config(kTaskRemoveFeedLevel,tpc);
   TaskManager::instance().scheduleRepeated(&FeedReplyBufferTimer::instance());

   PipePool::instance().initialize(new PipeFactoryI);
   DB_INSTANCE = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedDBInstance", "feed_db");
   //------------------For HA------------------ 
   int mod = service.getCommunicator()->getProperties()->getPropertyAsInt(
         "FeedDbWriter.Mod");
   int interval = 5; 
   ServerStateSubscriber::instance().initialize("ControllerFeedDbWriterR",&FeedDbWriterI::instance(), mod, interval, new XceFeedControllerChannel());
  
}

void FeedDbWriterI::FeedDispatch2DB(const FeedSeedPtr &seed,const FeedReplyPtr &reply,const Ice::Current & current) {
  if(!seed) {
    return;
  }
  MCE_INFO("FeedDbWriterI::FeedDispatch2DB --> source:"<<seed->source<<" stype"<<(seed->type & 0xffff)<<" actor:"<<seed->actor);
  TaskManager::instance().execute(new AddFeedTask(seed));
 //----------------IF REPLY NOT NULL  UPDATE TO DB
  if (reply ) {
    MyUtil::LongSeq feeds;
    feeds.push_back(seed->feed);
    UpdateReply2DB(feeds,reply);
  }
}
void FeedDbWriterI::FeedDispatchEDM2DB(const FeedSeedPtr & seed ,const Ice::Current & current) {
  if(!seed) {
    return;
  }
  MCE_INFO("FeedDbWriterI::FeedDispatchEDM2DB --> source:"<<seed->source<<" stype"<<(seed->type & 0xffff)<<" actor:"<<seed->actor);
  seed->extraProps["save_db"] = "1";
  seed->extraProps["publish_news"] = "1";
  seed->extraProps["publish_mini"] = "0";
  map<string,string>::iterator it = seed->extraProps.find("togroup");
  if( it != seed->extraProps.end()) {
    seed->extraProps.erase(it);
  }
  TaskManager::instance().execute(new AddFeedTask(seed));
}
void FeedDbWriterI::AddFeedSchool2DB(int schoolid,const FeedSeedPtr & seed,const Ice::Current &current) {
  if (!seed) {
    return;
  }
  MCE_INFO("FeedDbWriterI::AddFeedSchool2DB --> schoolid:"<<schoolid<<" feedid:"<<seed->feed);
  DbHelper::instance().addFeedSchool(schoolid,seed);
}

void FeedDbWriterI::UpdateReply2DB(const MyUtil::LongSeq& feeds,const FeedReplyPtr & reply,const Ice::Current & current) {
  if (feeds.empty() || !reply) {
    MCE_WARN("FeedDbWriter::UpdateReply2DB --> param is empty");
    return;
  }
  MCE_INFO("FeedDbWriterI::UpdateReply2DB --> feeds size:"<<feeds.size()<<" first feed:"<<feeds[0]);
  FeedReplyBufferTimer::instance().ChangeReply(feeds,reply);
}

void FeedDbWriterI::RemoveFeedFromDB(const FeedIndexSeq & indexSeq,const Ice::Current & current) {
   MyUtil::LongSeq feedIds,miniMerges;
  if(indexSeq.empty()) {
    MCE_WARN("FeedDbWriter::RemoveFeedFromDB -->  indexSeq empty");
    return;
  }
  ostringstream oactor,osource,ostype;
  oactor<<"[";
  ostype<<"[";
  osource<<"[";
  for(FeedIndexSeq::const_iterator it = indexSeq.begin(); it != indexSeq.end(); it++) {
    oactor<<" "<<(*it)->actor;
    ostype<<" "<<(*it)->stype;
    osource<<" "<<(*it)->source;
  }
  oactor<<" ]";
  ostype<<" ]";
  osource<<" ]";
  MCE_INFO("FeedDbWriter::RemoveFeedFromDB --> actors:"<<oactor.str()<<" stypes:"<<ostype.str()<<" sources:"<<osource.str()<<" indexSeq size:"<<indexSeq.size());

  RemoveFeedFromDBKeyPtr  rmFeedDbKey = new RemoveFeedFromDBKey();
  rmFeedDbKey->indexSeq = indexSeq;
  PipePool::instance().push("FeedPipe", 0, rmFeedDbKey);
  //feedindex duoble writer  add code at 2012-11-12
  for (FeedIndexSeq::const_iterator it = indexSeq.begin(); it != indexSeq.end(); it++) {
    DbHelper::instance().removeFeedIndexBysource((*it)->source,(*it)->stype,(*it)->actor);
  }
  //----------临时策略,RedisClient可用后改到FeedDispatchNew服务中
  try {
    for (size_t i = 0; i < indexSeq.size(); ++i) {
      FeedItemFacadeReplicaAdapter::instance().deleteFeed(indexSeq[i]->actor,indexSeq[i]->feed);
    }
  } catch (Ice::Exception &e ) {
    MCE_WARN("FeedDbWriter::RemoveFeedFromDB --> call FeedItemFacade err:"<<e);
  }
}
void FeedDbWriterI::ReplaceFeedXML2DB(const MyUtil::LongSeq& feedIds, const string& xml,const Ice::Current & current ) {
  if ( feedIds.empty()) {
     MCE_INFO("FeedDbWriter::ReplaceFeedXML2DB --> feedIds empty");
    return;
  }
  MCE_INFO("FeedDbWriter::ReplaceFeedXML2DB --> first feedid:"<<feedIds[0]<<" feedids size:"<<feedIds.size());
  MyUtil::LongSeq oldIds,newIds;
  DbHelper::instance().SplitFeedidsForDb(feedIds, oldIds, newIds);
  DbHelper::instance().ReplaceXML(oldIds,xml);
  DbHelper::instance().ReplaceZoneXML(newIds,xml);
}
void AddFeedTask::handle() {
  timeval begin,step1,step2,step3,step4;
  gettimeofday(&begin,NULL);

  FeedIndexPtr index = new FeedIndex;
  index->feed = seed_->feed;
  index->newsMerge = seed_->newsMerge;
  index->miniMerge = seed_-> miniMerge;
  index->source = seed_->source;
  index->psource = seed_->psource;
  index->actor = seed_->actor;
  index->time = seed_->time;
  index->stype = seed_->type & 0xffff;
  index->ptype = seed_->ptype;
   
 //-----------------INSERT FeedIndex TO DB
  DbHelper::instance().addFeedIndex(index);
  DbHelper::instance().addFeedIndexBysource(index);
  gettimeofday(&step1,NULL);
  //-----------------INSERT FeedSeed TO DB OF Content
  bool save_db   = seed_->extraProps.find("save_db") != seed_->extraProps.end();
  bool send_news = seed_->extraProps.find("publish_news") != seed_->extraProps.end();
  bool send_mini = seed_->extraProps.find("publish_mini") != seed_->extraProps.end();
  bool to_group  = seed_->extraProps.find("togroup") != seed_->extraProps.end();
  if ( (save_db && send_news ) || send_mini || to_group) {
    if (DbHelper::instance().isNewFeedidForDb(seed_->feed)) {
      DbHelper::instance().addZoneFeedContent(seed_);
    } else {
      DbHelper::instance().addFeedContent(seed_);
    }
  }
  gettimeofday(&step2,NULL);
  //-----------------INSERT FeedSeed TO DB OF Mini
  if (seed_->extraProps["publish_mini"] == "1") {
    DbHelper::instance().addFeedMini(seed_);
  }
  gettimeofday(&step3,NULL);
  //------------------INSERT FeedSeed TO DB OF Group 
  if( seed_->extraProps.find("togroup") != seed_->extraProps.end()) {
    int groupid = 0;
    try {
      groupid = boost::lexical_cast<int>(seed_->extraProps["togroup"]);
    } catch (...) {
      MCE_WARN("AddFeedTask::handle --> lexical cast err, " << seed_->extraProps["togroup"]);
    }
    MCE_INFO("AddFeedTask::handle --> send to group, actor:" << seed_->actor << " feed:" <<seed_->feed << " group:" << groupid);
    DbHelper::instance().addFeedGroup(groupid, seed_);
  }
  gettimeofday(&step4,NULL);
  float t1,t2,t3,t4;
  t1 = (step1.tv_usec - begin.tv_usec)*1.0/1000 + (step1.tv_sec - begin.tv_sec)*1000;
  t2 = (step2.tv_usec - step1.tv_usec)*1.0/1000 + (step2.tv_sec - step1.tv_sec)*1000;
  t3 = (step3.tv_usec - step2.tv_usec)*1.0/1000 + (step3.tv_sec - step2.tv_sec)*1000;
  t4 = (step4.tv_usec - step3.tv_usec)*1.0/1000 + (step4.tv_sec - step3.tv_sec)*1000;
  MCE_INFO("AddFeedTask::handle -->feed:"<<seed_->feed<<" Insert FeedIndex:"<<t1<<" Insert FeedSeed To Content:"<<t2<<" Insert FeedSeed to Mini:"<<t3<<" Insert FeeSee to Group:"<<t4<<"  Done");
}

void RemoveFeedTask::handle() {
  if (indexSeq_.empty()) {
   return;
  }
  MyUtil::LongSeq  oldIds,newIds; 
  MyUtil::LongSeq  feedIds;
  timeval begin,step1,step2;
  gettimeofday(&begin,NULL);
  
  for(FeedIndexSeq::const_iterator it = indexSeq_.begin(); it != indexSeq_.end(); ++it) {
    feedIds.push_back((*it)->feed);
    DbHelper::instance().removeFeedMini((*it)->actor,(*it)->stype, (*it)->miniMerge);
  }
  gettimeofday(&step1,NULL);
  DbHelper::instance().SplitFeedidsForDb(feedIds, oldIds, newIds);
  DbHelper::instance().removeFeedIndex(feedIds);
  DbHelper::instance().removeFeedContent(oldIds);
  DbHelper::instance().removeFeedZoneContent(newIds);
  gettimeofday(&step2,NULL);
  ostringstream os;
  for (size_t i=0 ; i < feedIds.size(); ++i) {
    os<<" "<<feedIds.at(i);
  }
  float t1,t2;
  t1 = (step1.tv_usec - begin.tv_usec)*1.0/1000 + (step1.tv_sec - begin.tv_sec)*1000;
  t2 = (step2.tv_usec - step1.tv_usec)*1.0/1000 + (step2.tv_sec - step1.tv_sec)*1000;
  MCE_INFO("RemoveFeedTask::handle --> remove feedids:"<<os.str()<<" remove FeedMini:"<<t1<<" remove FeedContent:"<<t2);
}
//----------------------------
void FeedReplyBufferTimer::ChangeReply(const MyUtil::LongSeq & feeds,
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



