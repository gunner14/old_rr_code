#include "FeedStatManagerI.h"
#include "ServiceI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h" 
using namespace xce::feed;
using namespace ::xce::serverstate;

void MyUtil::initialize(){
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&FeedStatManagerI::instance(), service.createIdentity("FSM", ""));
  FeedStatManagerI::instance().init();
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt(
      "FeedStatLog.Mod");
  int interval = 5;
  ServerStateSubscriber::instance().initialize("ControllerFeedStatLogR",&FeedStatManagerI::instance(), mod, interval, new XceFeedControllerChannel());
}

void FeedStatManagerI::dispatchRecord(Ice::Int stype, const string& logmsg, const Ice::Current&){
  if(!logmsg.empty()){
    MCE_INFO(logmsg);
  }
  recorderPtrSeq[stype % RECORDER_SIZE]->addDispatch(stype);
}

void FeedStatManagerI::replyRecord(Ice::Int stype, const string& logmsg, const Ice::Current&){
  if(!logmsg.empty()){
    MCE_INFO(logmsg);
  }
  recorderPtrSeq[stype % RECORDER_SIZE]->addReply(stype);
}

void FeedStatManagerI::deliverRecord(Ice::Int stype, const string& logmsg, Ice::Int count, const Ice::Current&){
  if(!logmsg.empty()){
    MCE_INFO(logmsg);
  }
  recorderPtrSeq[stype % RECORDER_SIZE]->addDeliver(stype, count);
}
