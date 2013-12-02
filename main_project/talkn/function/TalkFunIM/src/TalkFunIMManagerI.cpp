#include "TalkFunIMManagerI.h"
#include "XmppTools.h"
#include "IMTaskManagerLevel.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "MessageAnalyst.h"
#include "FunIMBuddy.h"
#include "FunIMState.h"

using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::function::im;

void MyUtil::initialize() {
  ServiceI &service = ServiceI::instance();
  service.getAdapter()->add(&TalkFunIMManagerI::instance(), service.createIdentity("M", ""));

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkFunIM", &TalkFunIMManagerI::instance(), mod, interval, new XceFeedControllerChannel());
  MCE_INFO("MyUtil::initialize. mod" << mod << " interval:" << interval);

  // 给遇见业务最多开5个线程进行处理，默认开启1个
  TaskManager::instance().config(TaskLevel::TASK_MEET, ThreadPoolConfig(1, 5));

  MessageAnalyst::instance().InitAnalystTable();
  TaskManager::instance().scheduleRepeated(&(state::NotifyWebOfflineForProcessUserDoingChanged::instance()));
  TaskManager::instance().scheduleRepeated(&(state::KickUserTimer::instance()));
}
/* brief: TalkProto协议通知接口，从
 * paras: 协议解析成的key-value map
 * return: void
 */
void TalkFunIMManagerI::notify(const Str2StrMap &paras, const Ice::Current &current) {
  Str2StrMap::const_iterator i;
  MessageAnalyst::instance().Analyse(paras);
}

/* brief: 向人人桌面发送好友申请通知
 * id: 申请信息id 
 * from: from用户id
 * to: to用户id
 * status: from用户状态
 * return: void
 */
void TalkFunIMManagerI::presenceSubscribe(const string& id, Ice::Int from, Ice::Int to, 
  const string& status, const Ice::Current& current) {
  buddy::FunIMBuddy::instance().NotifyClientAddBuddyRequest(id, from, to, status, "");
}

/* brief: 向人人桌面发送好友申请通知(附言)
 * id: 申请信息id 
 * from: from用户id
 * to: to用户id
 * status: from用户状态
 * postscript: 附言
 * return: void
 */

void TalkFunIMManagerI::presenceSubscribeWithPostscript(const string& id, Ice::Int from, 
  Ice::Int to, const string& status, const string& postscript, const Ice::Current& current) {
  buddy::FunIMBuddy::instance().NotifyClientAddBuddyRequest(id, from, to, status, postscript);
}


/* brief: 向人人桌面发送同意好友申请通知
 * id: 申请信息id 
 * from: from用户id
 * to: to用户id
 * return: void
 */
void TalkFunIMManagerI::presenceSubscribed(const string& id, Ice::Int from, Ice::Int to, 
  const Ice::Current& current) {
  buddy::FunIMBuddy::instance().NotifyClientAddBuddyAccept(id, from, to);
}

/* brief: 向人人桌面发送拒绝好友申请通知
 * id: 申请信息id 
 * from: from用户id
 * to: to用户id
 * return: void
 */
void TalkFunIMManagerI::presenceUnsubscribed(const string& id, Ice::Int from, Ice::Int to, 
  const Ice::Current& current) {
  buddy::FunIMBuddy::instance().NotifyClientAddBuddyReject(id, from, to);
}

/* brief: UserLogic调用通知人人桌面自己状态更改
 * (人人桌面的状态修改调用开放平台的接口)
 * userId: 用户id
 * return: void
 */
void TalkFunIMManagerI::webPresence(Ice::Int userId, const Ice::Current& current) {
  state::FunIMState::instance().NotifyWebOnlineForProcessUserDoingChanged(userId);
}


/* brief: 向新上线人人桌面用户发送在线好友列表和离线消息
 * jid: 上线用户Jid
 * show: 用户在线状态(在线，忙碌...)
 * ver: 客户端版本参数
 * subver: 客户端版本参数
 * ext: 客户端版本参数
 * return: void
 */
void TalkFunIMManagerI::presence(const JidPtr& jid, const string& show, const string& ver, const string& subver, 
  const string& ext, const Ice::Current& current) {
  state::FunIMState::instance().SendOnlineBuddyAndOfflineMessageToClient(jid, show, ver, subver, ext, "");
}

/* brief: 向新上线人人桌面用户发送在线好友列表和离线消息(新版本参数)
 * jid: 上线用户Jid
 * show: 用户在线状态(在线，忙碌...)
 * ver: 客户端版本参数
 * subver: 客户端版本参数
 * ext: 客户端版本参数
 * secver: 客户端版本参数
 * return: void
 */
void TalkFunIMManagerI::presenceWithSecver(const JidPtr& jid, const string& show, const string& ver, const string& subver,
  const string& ext, const string& secver, const Ice::Current& current) {
  state::FunIMState::instance().SendOnlineBuddyAndOfflineMessageToClient(jid, show, ver, subver, ext, secver);
}

/* brief: 改版后的下线通知接口，同时负责向在线系统发下线
 * jid: actor jid
 * return: void
 */
void TalkFunIMManagerI::offline(const JidPtr& jid, const Ice::Current& current) {
  state::FunIMState::instance().ProcessUserOffline(jid);
}

/* brief: User调用，用户某些资料修改后需将其踢下线，重新登录
 * userId: 用户id
 * userStatus: 判断是否需踢下线参数
 * return: void
 */
void TalkFunIMManagerI::userStatusChanged(int userId, int userStatus, const Ice::Current& current){
  state::FunIMState::instance().KickUserWhenHisConfigChanged(userId, userStatus);
}

/* brief: 准备替换OnlineCenter中的presence,除人人桌面外终端登录调用
 * jid: 上线用户Jid
 * stat: 用户状态
 * ver: 版本信息(空)
 * ext: 版本信息(描述endpoint)
 * return: void
 */
void TalkFunIMManagerI::OnlineCenterPresence(const JidPtr& jid, OnlineStat stat, const string& ver, 
  const string& ext, const Ice::Current& current) {
}
