#include "TalkFunMucManagerI.h"
#include "XmppTools.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "MessageAnalyst.h"
#include "PhotoUrlTools.h"
#include "TalkFunUtil.h"
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::function::muc;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(new TalkFunMucManagerI, service.createIdentity("M", ""));
	
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkFunMuc", &TalkFunMucManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
  PhotoUrlHelper::instance().StartReloadTimer();
  TaskManager::instance().scheduleRepeated(new HourTimer());
}

/*说明: 群聊服务调用接口
 *参数: 传入群聊服务的数据 
 */
void TalkFunMucManagerI::notify(const Str2StrMap& paras, const Ice::Current& current) {
  for (Str2StrMap::const_iterator iter=paras.begin(); iter!=paras.end(); iter++)
    MCE_DEBUG("TalkFunMucManagerI::notify-->   iter:" << iter->first << "   second:" << iter->second);
  MessageAnalyst::instance().handle(paras);
  return;
}

