#include "ServiceI.h"
#include <boost/lexical_cast.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "FeedAssistantNReplicaAdapter.h"
#include "FeedCacheLogicReplicaAdapter.h"
#include "FeedNewsReplicaAdapter.h"
#include "FeedMiniReplicaAdapter.h"
#include "FeedItemFacadeReplicaAdapter.h"
#include "FeedSinkAdapter.h"
#include "FeedConsumerI.h"
#include "RFeed.h"
#include <sstream>
#include <map>


using namespace xce::feed;

using namespace MyUtil;

using namespace std;



void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedConsumerI::instance(), service.createIdentity("M", ""));

//  //---------------HA---------------//
//  
//  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt(
//      "FeedCacheLogic.Interval", 5);
//  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerFeedCacheLogicR",
//      &FeedCacheLogicI::instance(), mod, interval, new XceFeedControllerChannel());
//  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
  activemq::library::ActiveMQCPP::initializeLibrary();
   

}


void DispatchConsumer::onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {

  MCE_INFO("DispatchConsumer::onMessage --> message size:" << receiveMessages.size());
  std::vector<FeedDispatchProto> proto_vec = vec2proto(receiveMessages); 


  for(std::vector<FeedDispatchProto>::const_iterator it = proto_vec.begin(); it != proto_vec.end(); it++) {
    const FeedDispatchProto& fsp = *it;
    const ::xce::feed::MapProto& mp =  fsp.extraprops();
    ::MyUtil::Str2StrMap  icesmap;

    int prop_size = mp.items_size();

    for(int i=0; i< prop_size;i++) {
      const ::xce::feed::KeyValueProto& kv =  mp.items(i);
      MCE_INFO("key:" << kv.key() << " value:" << kv.value());
      icesmap.insert(make_pair(kv.key(), kv.value()));
    }

    xce::feed::FeedSeedPtr  seed = new  FeedSeed((Ice::Long)fsp.feed(), (Ice::Long)fsp.minimerge(), (Ice::Long)fsp.newsmerge(), (Ice::Long)fsp.source(), (Ice::Long)fsp.psource(), (Ice::Int)fsp.actor(), (Ice::Int)fsp.type(), (Ice::Int)fsp.ptype(), (Ice::Int)fsp.time(), (Ice::Int)fsp.baseweight(),fsp.xml(), icesmap);
    DispatchOP op = fsp.op();
    switch (op) {

      case DISPATCH:
        {
          try {
            FeedAssistantNReplicaAdapter::instance().dispatch(seed);
          } catch (Ice::Exception& e) {
            MCE_WARN("DispatchConsumer::onMessage  --> dispatch feed:" << seed->feed << " err, " << e);
          }

          try {
            FeedItemFacadeReplicaAdapter::instance().put(seed);
          } catch (Ice::Exception& e) {
            MCE_WARN("DispatchConsumer::onMessage  --> dispatch. call ItemFacade error. feed:" << seed->feed << " err, " << e);
          }  
          break;
        }
      case DISPATCH_WITH_REPLY:
        {
          xce::feed::FeedReplyPtr  reply = new  FeedReply((Ice::Long)fsp.oldreplyid(), fsp.oldreply(), (Ice::Long)fsp.newreplyid(), fsp.newreply(), (Ice::Long)fsp.count());

          MCE_INFO("DispatchConsumer::onMessage --> dispatch feed:" << fsp.feed() << " oldreplyid:" << fsp.oldreplyid() << " oldreply:" << fsp.oldreply() << " newreplyid:" << fsp.newreplyid() << " newreply:" << fsp.newreply() << " count:" << fsp.count());
          try {
            FeedAssistantNReplicaAdapter::instance().dispatchWithReply(seed, reply);
          } catch (Ice::Exception& e) {
            MCE_WARN("DispatchConsumer::onMessage  --> dispatchWitReply.call FeedAssitantN error. feed:" << seed->feed << " newreplyid:" << fsp.newreplyid() << " oldreplyid:" << fsp.oldreply() << " err, " << e);
          }  

          try {
            FeedItemFacadeReplicaAdapter::instance().put(seed);
          } catch (Ice::Exception& e) {
            MCE_WARN("DispatchConsumer::onMessage  --> dispatchWitReply.call ItemFacade error. feed:" << seed->feed << " newreplyid:" << fsp.newreplyid() << " oldreplyid:" << fsp.oldreply() << " err, " << e);
          }  

          break;
        }

      case DISPATCH_EDM:
        {
          try {
            FeedAssistantNReplicaAdapter::instance().dispatchEDM(seed);
          } catch (Ice::Exception& e) {
            MCE_WARN("DispatchConsumer::onMessage  --> dispatchEDM erro. source:" << seed->source);                                                                                    
          }  
          break;

        }
      default:
        MCE_WARN("DispatchConsumer::onMessage --> dispatch unknown op:" << fsp.op()  << " feed:" << fsp.feed());

  } 
    MCE_INFO("DispatchConsumer::onMessage --> dispatch op:" << fsp.op() << " feed:" << fsp.feed() << "mm:" << fsp.minimerge() << " nm:" << fsp.newsmerge() << " s:" << fsp.source() << " ps:" << fsp.psource() << " actor:" << fsp.actor() << " type:" << fsp.type() << " ptype:" <<  fsp.ptype() << " time:" << fsp.time() << " bw:" << fsp.baseweight() << " xml:" << fsp.xml());


  }
}


void ReplyConsumer::onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {

  MCE_INFO("ReplyConsumer::onMessage --> message size:" << receiveMessages.size());
  std::vector<FeedReplyProto> proto_vec = vec2proto(receiveMessages); 

  for(std::vector<FeedReplyProto>::const_iterator it = proto_vec.begin(); it != proto_vec.end(); it++) {
    const FeedReplyProto & frp = *it;

    xce::feed::FeedReplyPtr  reply = new  FeedReply((Ice::Long)frp.oldreplyid(), frp.oldreply(), (Ice::Long)frp.newreplyid(), frp.newreply(), (Ice::Long)frp.count());

    ReplyOP op = frp.op();
    switch(op) {
      case ADD_REPLY:
        {
          try {
            FeedAssistantNReplicaAdapter::instance().addFeedReply(frp.source(), frp.stype(), frp.actor(), reply);
          } catch (Ice::Exception& e) {
            MCE_WARN("ReplyConsumer::onMessage --> addFeedReply source:" << frp.source() << " stype:" << frp.stype() << " actor:" << frp.actor() << " err, " << e);
          }

          MCE_INFO("ReplyConsumer::onMessage --> op:" << frp.op() << " source:" << frp.source() << " stype:" << frp.stype() << " actor:" << frp.actor() << " oldReplyId:" << frp.oldreplyid() << " oldReply:" << frp.oldreply() << " newReplyId" << frp.newreplyid() << " newReply:" << frp.newreply() << " count:" <<  frp.count());
          break;
        }
      case ADD_REPLY_BY_ID:
        {
          try {
            FeedAssistantNReplicaAdapter::instance().addFeedReplyById(frp.stype(), frp.feedid(), reply);
          } catch (Ice::Exception& e) {
            MCE_WARN("ReplyConsumer::onMessage --> addFeedReplyById  stype:" << frp.stype() << " feedid:" << frp.feedid() << " err, " << e);
          }

          MCE_INFO("ReplyConsumer::onMessage --> op:" << frp.op() << " stype:" << frp.stype() << " feedid:" << frp.feedid() << " oldReplyId:" << frp.oldreplyid() << " oldReply:" << frp.oldreply() << " newReplyId" << frp.newreplyid() << " newReply:" << frp.newreply() << " count:" <<  frp.count());
          break;
        }
      case ADD_REPLY_WITH_CONFIG:
        {
          const ::xce::feed::MapProto& mp =  frp.config();
          ::MyUtil::Str2StrMap  config;

          int prop_size = mp.items_size();

          for(int i=0; i< prop_size;i++) {
            const ::xce::feed::KeyValueProto& kv =  mp.items(i);
            MCE_INFO("ReplyConsumer::onMessage --> addReplyWithConfig key:" << kv.key() << " value:" << kv.value());
            config.insert(make_pair(kv.key(), kv.value()));
          }

          try {
            FeedAssistantNReplicaAdapter::instance().addFeedReplyWithConfig(frp.source(), frp.stype(), frp.actor(), reply, config);
          } catch (Ice::Exception& e) {
            MCE_WARN("ReplyConsumer::onMessage --> addFeedReplyWithConfig source:" << frp.source() << " stype:" << frp.stype() << " actor:" << frp.actor() << " err, " << e);
          }

          MCE_INFO("ReplyConsumer::onMessage --> op:" << frp.op() << " source:" << frp.source() << " stype:" << frp.stype() << " actor:" << frp.actor() << " oldReplyId:" << frp.oldreplyid() << " oldReply:" << frp.oldreply() << " newReplyId" << frp.newreplyid() << " newReply:" << frp.newreply() << " count:" <<  frp.count() << " config size:" << config.size());
          break;
        }
      default:
        MCE_WARN("ReplyConsumer::onMessage --> unknown reply op:" << frp.op() << " source:" << frp.source() << " stype:" << frp.stype() << " actor:" << frp.actor());
    }



  }

}

void ReadConsumer::onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {

  MCE_INFO("ReadConsumer::onMessage --> message size:" << receiveMessages.size());
  std::vector<ReadProto> proto_vec = vec2proto(receiveMessages); 

  for(std::vector<ReadProto>::const_iterator it = proto_vec.begin(); it != proto_vec.end(); it++) {
    const ReadProto & rp = *it;
    switch(rp.op()) {
      case READ_NEWS:
        try{
          FeedNewsReplicaAdapter::instance().setReadById(rp.user(), rp.feedid());
        }catch(Ice::Exception& e){
          MCE_WARN("ReadConsumer::onMessage --> call FeedNews setReadById err:" << e << " user:" << rp.user() << " feed:" << rp.feedid());
        }

        try{
          FeedCacheLogicReplicaAdapter::instance().remove(rp.user(), rp.feedid(), true);
        }catch(Ice::Exception& e){
          MCE_WARN("ReadConsumer::onMessage --> call FeedCacheLogic remove err:" << e << " user:" << rp.user() << " feed:" << rp.feedid());
        }

//        try{
//          FeedSinkAdapter::instance().SetReadById(rp.user(), rp.feedid());
//        }catch(Ice::Exception& e){
//          MCE_WARN("ReadConsumer::onMessage --> call FeedSink err:" << e << " user:" << rp.user() << " feed:" << rp.feedid());
//        }
        break;

      case READ_ALL_NEWS:
        try{
          FeedNewsReplicaAdapter::instance().setReadAll(rp.user());
        }catch(Ice::Exception& e){
          MCE_WARN("ReadConsumer::onMessage --> call FeedNews setReadAll err:" << e << " user:" << rp.user());
        }
        try{
          FeedCacheLogicReplicaAdapter::instance().removeAll(rp.user(), true);
        }catch(Ice::Exception& e){
          MCE_WARN("ReadConsumer::onMessage --> call FeedCacheLogic removeAll err:" << e << " user:" << rp.user());
        }

//        try{
//          FeedSinkAdapter::instance().SetReadAll(rp.user());
//        }catch(Ice::Exception& e){
//          MCE_WARN("ReadConsumer::onMessage --> call FeedSink setReadAll err:" << e << " user:" << rp.user());
//        }
        break;
      case READ_MINI:
        try{
          FeedMiniReplicaAdapter::instance().setReadById(rp.user(), rp.feedid());
        }catch(Ice::Exception& e){
          MCE_WARN("ReadConsumer::onMessage --> call FeedMini setReadbyId err:" << e << " user:" << rp.user() << " feed:" << rp.feedid());
        }
        break;
      case READ_ALL_MINI:
        try{
          FeedMiniReplicaAdapter::instance().setReadAll(rp.user());
        }catch(Ice::Exception& e){
          MCE_WARN("ReadConsumer::onMessage --> call FeedMini setReadAll err:" << e << " user:" << rp.user());
        }
        break;
      default:
        MCE_WARN("ReadConsumer::onMessage --> unknown read op:" << rp.op() << " user:" << rp.user() << " feedid:" << rp.feedid());
    }

    MCE_INFO("ReadConsumer::onMessage --> user:" << rp.user() << " feed:" << rp.feedid() << " op:" << rp.op());
  }

}

void RemoveConsumer::onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {

  MCE_INFO("RemoveConsumer::onMessage --> message size:" << receiveMessages.size());
  std::vector<RemoveFeedProto> proto_vec = vec2proto(receiveMessages); 

  for(std::vector<RemoveFeedProto>::const_iterator it = proto_vec.begin(); it != proto_vec.end(); it++) {
    const RemoveFeedProto & rfp = *it;
    
    RemoveOP op = rfp.op();
    switch(op) {
      case REMOVE_BY_SOURCE: 
        {
          try {
            FeedAssistantNReplicaAdapter::instance().removeFeedBySource(rfp.source(), rfp.type());
          } catch (Ice::Exception& e) {
            MCE_WARN("RemoveConsumer::onMessage --> removeFeedBySource source:" << rfp.source() << " type:" << rfp.type()  << " e:" << e);
          }
          MCE_INFO("RemoveConsumer::onMessage --> removeFeedBySource source:" << rfp.source() << " type:" << rfp.type());
          break;
        }
      case REMOVE_BY_SOURCE_HS: 
        {
          try {
            FeedAssistantNReplicaAdapter::instance().removeFeedBySourceHS(rfp.source(), rfp.type());
          } catch (Ice::Exception& e) {
            MCE_WARN("RemoveConsumer::onMessage --> removeFeedBySourceHS source:" << rfp.source() << " type:" << rfp.type()  << " e:" << e);
          }
          MCE_INFO("RemoveConsumer::onMessage --> removeFeedBySourceHS source:" << rfp.source() << " type:" << rfp.type());
          break;
        }

      case REMOVE_BY_PARENT: 
        {
          try {
            FeedAssistantNReplicaAdapter::instance().removeFeedByParent(rfp.source(), rfp.type());
          } catch (Ice::Exception& e) {
            MCE_WARN("RemoveConsumer::onMessage --> removeFeedBySourceHS psource:" << rfp.source() << " ptype:" << rfp.type()  << " e:" << e);
          }
          MCE_INFO("RemoveConsumer::onMessage --> removeFeedBySourceHS psource:" << rfp.source() << " ptype:" << rfp.type());
          break;

        }
      case REMOVE_FEED: 
        {
          try {
            FeedAssistantNReplicaAdapter::instance().removeFeed(rfp.source(), rfp.type(), rfp.actor());
          } catch (Ice::Exception& e) {
            MCE_WARN("RemoveConsumer::onMessage --> removeFeed source:" << rfp.source() << " type:" << rfp.type() << " actor:" << rfp.actor()  << " e:" << e);
          }
          MCE_INFO("RemoveConsumer::onMessage --> removeFeed source:" << rfp.source() << " type:" << rfp.type() << " actor:" << rfp.actor());
          break;
        }
      case REMOVE_REPLY: 
        {
          xce::feed::FeedReplyPtr  reply = new  FeedReply((Ice::Long)rfp.oldreplyid(), rfp.oldreply(), (Ice::Long)rfp.newreplyid(), rfp.newreply(), (Ice::Long)rfp.count());  
          try {
            FeedAssistantNReplicaAdapter::instance().removeFeedReply(rfp.source(), rfp.type(), rfp.actor(), rfp.replyid(), reply);
          } catch (Ice::Exception& e) {
            MCE_WARN("RemoveConsumer::onMessage --> removeFeedReply source:" << rfp.source() << " type:" << rfp.type() << " actor:" << rfp.actor()  << " e:" << e);
          }
          MCE_INFO("RemoveConsumer::onMessage --> removeFeedReply source:" << rfp.source() << " type:" << rfp.type() << " actor:" << rfp.actor());
          break;

        }
      case REMOVE_REPLY_BY_ID: 
        {
          xce::feed::FeedReplyPtr  reply = new  FeedReply((Ice::Long)rfp.oldreplyid(), rfp.oldreply(), (Ice::Long)rfp.newreplyid(), rfp.newreply(), (Ice::Long)rfp.count());  
          try {
            FeedAssistantNReplicaAdapter::instance().removeFeedReplyById(rfp.type(), rfp.feedid(), rfp.replyid(), reply);
          } catch (Ice::Exception& e) {
            MCE_WARN("RemoveConsumer::onMessage --> removeFeedReplyById type:" << rfp.type() << " feedid:" << rfp.feedid() << " replyid:" << rfp.replyid() << " e:" << e);
          }
          MCE_INFO("RemoveConsumer::onMessage --> removeFeedReplyById  type:" << rfp.type() << " feedid:" << rfp.feedid() << " replyid:" << rfp.replyid());
          break;
        }
      case REMOVE_BY_ID: 
        {
          try {
            FeedAssistantNReplicaAdapter::instance().removeFeedById(rfp.feedid());
          } catch (Ice::Exception& e) {
            MCE_WARN("RemoveConsumer::onMessage --> removeFeedById feedid:" << rfp.feedid() << " e:" << e);
          }
            MCE_INFO("RemoveConsumer::onMessage --> removeFeedById feedid:" << rfp.feedid());
          break;
        }
      default:
        MCE_WARN("RemoveConsumer::onMessage --> unknown op:" << rfp.op());

    }
  }
}

void OtherFeedConsumer::onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {

  MCE_INFO("OtherFeedConsumer::onMessage --> message size:" << receiveMessages.size());
}

Ice::Int FeedConsumerI::get(const Ice::Current& current) {
  MCE_INFO("FeedConsumerI::get");
  //new DispatchConsumer("UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181","bj_test1", "feed");

//t->start();

  

}


