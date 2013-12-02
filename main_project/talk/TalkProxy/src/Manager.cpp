#include "Manager.h"
#include "Server.h"

#include "ConnectionQuery.h"
#include "XmppTools.h"
#include <sys/resource.h>
#include "Action.h"
#include "MessageHolderAdapter.h"
#include "MessageType.h"
#include <util/cpp/String.h>

using namespace com::xiaonei::talk;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::util;
using namespace ::talk::adapter;

//---------------------------------------------------------------------------
/* brief: 直接向给定的jid投递消息
 * jid: 目标jid 
 * msg: 投递的消息
 * return: void
 */
void Manager::express(const JidPtr& jid, const string& msg, const Ice::Current&) {
	MCE_DEBUG("to:"<<jidToString(jid)<<" msg:"<<msg);
	Server::instance().deliver(jid, msg);
}

/* brief: 根据消息体携带的发送相关数据，完成对用户数据的推送
 * mseq: 需要发送的消息序列
 * return: void
 */
void Manager::deliver(const common::MessageSeq& mseq, const Ice::Current&) {
	for (common::MessageSeq::const_iterator it = mseq.begin(); it != mseq.end(); ++it) {
		MCE_DEBUG("Manager::deliver --> toJid:"<<jidToString((*it)->to)<<"   msg:"<<(*it)->msg);
		if(Server::instance().deliver((*it)->to, (*it)->msg)){
			if((*it)->type == MESSAGE && (*it)->msgKey!=-1){
				MessageHolderAdapter::instance().send((*it)->to->userId, (*it)->msgKey);
			}
		}
	}
}

/* brief: 内部实现逻辑比较简单，怀疑该接口已经废弃了，没有实际作用
 */
bool Manager::verify(const JidPtr& jid, const string& ticket,
		const Ice::Current&) {
	return Server::instance().verify(jid, ticket);
}

bool Manager::checkTicket(const JidPtr& jid, const string& ticket,
		const Ice::Current&) {
	return Server::instance().verify(jid, ticket);
}

string Manager::GetTicket(const JidPtr& jid,
        const Ice::Current&){
	return Server::instance().GetTicket(jid);
}

void Manager::leave(const JidPtr& jid, const Ice::Current&) {
	//Server::instance().leave(jid);
}
void Manager::relogin(const Ice::Current&) {

}

void Manager::broadcast(const string& xmlMsg, const Ice::Current&) {

}

/* brief: 同sessionExist接口
 */
bool Manager::indexExist(Ice::Long index, const Ice::Current&){
	return Server::instance().indexExist(index);
}

void Manager::kick(const JidPtr& jid, const Ice::Current&){
	MCE_INFO("Manager::kick --> " << jidToString(jid));
	return Server::instance().kick(jid);
}
/* brief: 判断给定的index是否在该接入副本中存在连接
 * index: 连接的映射编号
 * return: 存在返回true否则false
 */
bool Manager::sessionExist(Ice::Long index, const Ice::Current&){
  return Server::instance().indexExist(index);
}

/* brief: 纯虚接口实现，无作用，编译通过
 */
DeliverStatList Manager::GetDeliverStat(const Ice::Current&) {
  return DeliverStatList();
}
void Manager::ClearDeliverStat(const Ice::Current&) {

}

/* brief: 获取心跳参数
 * return: 心跳参数结构
 */
HeartBeatConfPtr Manager::getHeartBeatConf(const Ice::Current&) {
  return SessionHeartBeatPool::instance().get_thread_conf();

}
/* brief: 设置心跳参数
 * conf: 心跳参数结构
 * return: void
 */
void Manager::setHeartBeatConf(const HeartBeatConfPtr &conf, const Ice::Current&) {
  SessionHeartBeatPool::instance().set_heartbeat_conf(conf);
}
/* brief: 心跳线程控制
 * return: void
 */
void Manager::opeHeartBeatThread(int flag, const Ice::Current&) {
  if (0 == flag) {
    SessionHeartBeatPool::instance().stop_thread();
  } else {
    SessionHeartBeatPool::instance().start_thread();
  }
}
