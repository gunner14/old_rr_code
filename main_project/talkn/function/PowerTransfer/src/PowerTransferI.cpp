#include "PowerTransferI.h"
#include "MessageType.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "TalkFunStateAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "TalkProtoAdapter.h"
#include "TalkUtil/src/pugixml.hpp"
#include "TalkUtil/src/XmppTools.h"
#include "UrlCode.h"
#include "FeedTransfer.h"

using namespace com::xiaonei::talk::function;
using namespace com::xiaonei::talk::fun::state;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;
using namespace ::xce::feed;
using namespace ::com::xiaonei::xce;
using namespace com::xiaonei::talk::function::transfer;
using namespace com::xiaonei::talk::proto;
using namespace pugi;
using namespace talk::http;
using namespace ::xce::talk;

void MyUtil::initialize() {
  ServiceI::instance().getAdapter()->add(&PowerTransferI::instance(), ServiceI::instance().createIdentity("M", ""));
  InitMonitorLogger(ServiceI::instance().getName(), "monitor", "../log/" + ServiceI::instance().getName() + "/monitor/monitor_log", "INFO");

  ServiceI& service = ServiceI::instance();
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerPowerTransfer", &PowerTransferI::instance(), mod, interval, new XceFeedControllerChannel());
  
  PowerTransferI::instance().DeliverTypeSetInit();

  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
}

void PowerTransferI::notify(const Str2StrMap& paras, const Ice::Current& current) {

}
/* brief : 推送回复
 */
void PowerTransferI::deliverFeedReply(const ReplyDataPtr& replyData, const MyUtil::IntSeq &target, 
  const Ice::Current& current) {
  const string FuncName = "PowerTransferI::deliverFeedReply";
  TimeStatN ts;

  if(!replyData)
    return;

  TaskManager::instance().execute(new DeliverFeedReplyTask(replyData, target));

  MCE_INFO(FuncName << " actor: " << replyData->actor <<" source: " << replyData->source 
    << " stype: " << replyData->stype << " replyId: " << replyData->replyId);

  FunStatManager::instance().Stat(FuncName, ts.getTime(), false);
}
/* brief : 推送新鲜事
 */
void PowerTransferI::deliverFeedWithFocusfriend(const FeedSeedPtr& feed, const MyUtil::IntSeq& target,
  const MyUtil::IntSeq& focusfriendSeq, const Ice::Current& ){
  const string FuncName = "PowerTransferI::deliverFeedWithFocusfriend";
  TimeStatN ts;

  if(!feed)
    return;

  if(feed->xml.empty() || !IsDeliverType(feed->type & 0xFFFF) || target.empty())
    return;

  TaskManager::instance().execute(new DeliverFeedTask(feed, target, focusfriendSeq));

  FeedSend2PIMStorm(feed, target, focusfriendSeq);

  MCE_INFO(FuncName << " actor: " << feed->actor <<" source: " << feed->source 
    << " stype: " << (feed->type & 0xFFFF) << " id: " << feed->feed);

  FunStatManager::instance().Stat(FuncName, ts.getTime(), false);
}
/* brief : 推送给3G
 */
void PowerTransferI::FeedSend2PIMStorm(const FeedSeedPtr& feed, const MyUtil::IntSeq& target,
  const MyUtil::IntSeq& focusfriendSeq) {
  try {
    GFeedTargetSeq    gobjs;
    GFeedTarget gf;
    gobjs.push_back(gf);
    gobjs.at(0).feedid = feed->feed;
    gobjs.at(0).type = feed->type & 0xFFFF;
    gobjs.at(0).actor = feed->actor;
    gobjs.at(0).target = target;
    gobjs.at(0).focusfriend = focusfriendSeq;
    IMStormAdapter::instance().deliverfeeds(gobjs);
  } catch(Ice::Exception& e) {
    MCE_WARN("FeedPipe::handle --> IMStormAdapter::deliverfeeds error : " << e.what());
  } catch(...) {
    MCE_WARN("FeedPipe::handle --> IMStormAdapter::deliverfeeds error : unknow exception");
  }
}


