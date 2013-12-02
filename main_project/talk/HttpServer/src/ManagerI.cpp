#include "ManagerI.h"
#include "IceLogger.h"
#include "MessageType.h"
#include "Request.h"
#include "Server.h"
#include "ClientBuffer.h"
#include "SessionHeartBeat.h"
#include "XmppTools.h"

using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;
using namespace talk::http;
using namespace MyUtil;
using namespace com::xiaonei::talk;
/* brief: 根据消息体携带的发送相关数据，完成对用户数据的推送
 * mseq: 需要发送的消息序列
 * return: void
 */
void ManagerI::deliver(const MessageSeq& mseq, const Ice::Current&) {
	//MCE_INFO("ManagerI::deliver --> deliver a message to jsclient size=" << mseq.size());
	for (size_t i = 0; i < mseq.size(); ++i) {
    MCE_INFO("ManagerI::deliver-->toJid:" << jidToString(mseq.at(i)->to)
             << " type:" << mseq.at(i)->type << " key:" <<mseq.at(i)->msgKey);
		ClientPoolManager::instance().getClientPool(mseq.at(i)->to->index).pushMsg(mseq.at(i)->to->index, mseq.at(i));
	}
}

/* brief: 判断给定的index是否在该接入副本中存在连接副本
 * index: 连接的映射编号
 * return: 存在返回true否则false
 */
bool ManagerI::indexExist(Ice::Long index, const Ice::Current&){
	return Server::instance().indexExist(index);
}
/* brief: 判断给定的index是否在该接入副本中的用户连接池有合法缓存，
 * 因为该接入服务是基于http协议的，comet long-polling方式实现
 * 对比indexExist，是完全从socket角度，而该接口是从逻辑角度的连接
 * index: 连接的映射编号
 * return: 存在返回true否则false
 */
bool ManagerI::sessionExist(Ice::Long index, const Ice::Current&){
  JidPtr j = ClientPoolManager::instance().getClientPool(index).getJid(index);
  return j == 0 ? false : true;
}


void ManagerI::kick(const JidPtr& jid, const Ice::Current&){

}

/* brief: 统计接口，获取http协议返回值统计
 * return: 统计信息
 */
DeliverStatList ManagerI::GetDeliverStat(const Ice::Current&) {
  return Server::instance().getDeliverStat();
}
/* brief: 统计接口，清除统计缓存
 * return: void
 */
void ManagerI::ClearDeliverStat(const Ice::Current&) {
  Server::instance().clearDeliverStat();
}


/* brief: 获取心跳参数
 * return: 心跳参数结构
 */
HeartBeatConfPtr ManagerI::getHeartBeatConf(const Ice::Current&) {
  return SessionHeartBeatPool::instance().get_thread_conf();
  return new HeartBeatConf;

}
/* brief: 设置心跳参数
 * conf: 心跳参数结构
 * return: void
 */
void ManagerI::setHeartBeatConf(const HeartBeatConfPtr &conf, const Ice::Current&) {
  SessionHeartBeatPool::instance().set_heartbeat_conf(conf);
}

/* brief: 心跳线程控制
 * return: void
 */
void ManagerI::opeHeartBeatThread(int flag, const Ice::Current&) {
  if (0 == flag) {
    SessionHeartBeatPool::instance().stop_thread();
  } else {
    SessionHeartBeatPool::instance().start_thread();
  }
}
