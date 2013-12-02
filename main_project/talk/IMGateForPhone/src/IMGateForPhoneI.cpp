#include <boost/lexical_cast.hpp>
#include "IMGateForPhoneI.h"
#include "OnlineCenterAdapter.h"
#include "MessageMsgHandlerAdapter.h"
#include "PresenceMsgHandlerAdapter.h"
#include "IMGateForPhoneAdapter.h"
#include "MessageHolderAdapter.h"
#include "IMStormAdapter.h"
#include "XmppTools.h"
#include "pugixml.hpp"
#include <string>
#include <iostream>
#include "TalkFunStateAdapter.h"
#include "TalkFunPhoneStateAdapter.h"
#include "HeartBeatThread.h"

using namespace com::xiaonei::talk::fun::state;
using namespace pugi;
using namespace com::xiaonei::talk::gate;
using namespace xce::talk;
using namespace com::xiaonei::talk::common;
using namespace talk::online::adapter;
using namespace com::xiaonei::talk::util;
using namespace talk::adapter;
using namespace std;
using namespace MyUtil;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&IMGateForPhoneManagerI::instance(), service.createIdentity("M", ""));
	IMGateForPhoneManagerI::instance().ServerEnable();
}


/*说明：3G向IM发送聊天消息
 *参数1：手机用户
 *参数2：接受人用户id
 *参数3：聊天消息
 */
void IMGateForPhoneManagerI::SendMessage(int phoneuserid, int touserid, const string& message, const Ice::Current&){
	TimeStatN ts;
	{
		IceUtil::RWRecMutex::RLock lock(_rwmutex);
		if(!_serverenable){
			MCE_WARN("IMGateForPhoneManagerI::PhoneUserOffline --> server is disable now!");
      FunStatManager::instance().Stat("IMGateForPhoneManagerI.SendMessage.disable", ts.getTime(), (ts.getTime() >= 250.0));	
			return;
		}
	}
	IceUtil::RWRecMutex::RLock lock(_rwmutex);
	JidPtr fromjid = new Jid();
	fromjid->userId = phoneuserid;
	ostringstream os;
	//os << "PIMStorm" << (phoneuserid % (int)(IMGateForPhoneAdapter::instance().getCluster()));
	os << "PIMStorm" << phoneuserid % IMStormAdapter::instance().getCluster();
	fromjid->endpoint = os.str();
	fromjid->index = 2;
	os.str("");	
	xml_document doc;
	xml_node messageNode = doc.append_child();
	messageNode.set_name("message");
	os << touserid << "@talk.renren.com";
	messageNode.append_attribute("to").set_value(os.str().c_str());
	messageNode.append_attribute("type").set_value("chat");
	xml_node bodyNode= messageNode.append_child();
	bodyNode.set_name("body");
	bodyNode.append_child(node_pcdata).set_value(message.c_str());
	//os << "<message to=\"" << touserid << "@talk.renren.com\" type=\"chat\"><body>" << message << "</body></message>";
	try{
		MCE_INFO("IMGateForPhoneManagerI::SendMessage --> phoneuserid = " << phoneuserid << " touserid = " << touserid << " message = " << message << " xml:" << doc.xml());
		MessageMsgHandlerAdapter::instance().message(fromjid, doc.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::SendMessage -->MessageMsgHandlerAdapter::message --> error : " << e);
	}

	FunStatManager::instance().Stat("IMGateForPhoneManagerI.SendMessage.ok", ts.getTime(), (ts.getTime() >= 250.0));	
}

/*说明：3G向IM发送聊天消息,含msgkey
 *参数1：手机用户
 *参数2：接受人用户id
 *参数3：聊天消息id
 *参数4：聊天消息
 */
void IMGateForPhoneManagerI::SendMessageWithMsgkey(int phoneuserid, int touserid, long msgkey, const string& message, const Ice::Current& current) {
  TimeStatN ts;
	{
		IceUtil::RWRecMutex::RLock lock(_rwmutex);
		if(!_serverenable){
			MCE_WARN("IMGateForPhoneManagerI::SendMessageWithMsgkey --> server is disable now!");
      FunStatManager::instance().Stat("IMGateForPhoneManagerI.SendMessage.disable", ts.getTime(), (ts.getTime() >= 250.0));	
			return;
		}
	}
	IceUtil::RWRecMutex::RLock lock(_rwmutex);
	JidPtr fromjid = new Jid();
	fromjid->userId = phoneuserid;
	ostringstream os;
	//os << "PIMStorm" << (phoneuserid % (int)(IMGateForPhoneAdapter::instance().getCluster()));
	os << "PIMStorm" << phoneuserid % IMStormAdapter::instance().getCluster();
	fromjid->endpoint = os.str();
	fromjid->index = 2;
	os.str("");	
	xml_document doc;
	xml_node messageNode = doc.append_child();
	messageNode.set_name("message");
	os << touserid << "@talk.renren.com";
	messageNode.append_attribute("to").set_value(os.str().c_str());
	messageNode.append_attribute("type").set_value("chat");
	xml_node msgkeyNode= messageNode.append_child();
	msgkeyNode.set_name("msgkey");
  try {
	  msgkeyNode.append_child(node_pcdata).set_value((boost::lexical_cast<string>(msgkey)).c_str());
  } catch(...) {
	  MCE_WARN("IMGateForPhoneManagerI::SendMessageWithMsgkey --> msgkey lexical_cast error! " << msgkey);
  }
	xml_node bodyNode= messageNode.append_child();
	bodyNode.set_name("body");
	bodyNode.append_child(node_pcdata).set_value(message.c_str());
	//os << "<message to=\"" << touserid << "@talk.renren.com\" type=\"chat\"><body>" << message << "</body></message>";
	try{
		MCE_INFO("IMGateForPhoneManagerI::SendMessageWithMsgkey --> phoneuserid = " << phoneuserid << " touserid = " << touserid << " msgkey =" << msgkey << " message = " << message << " xml:" << doc.xml());
		MessageMsgHandlerAdapter::instance().message(fromjid, doc.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::SendMessage -->MessageMsgHandlerAdapter::message --> error : " << e);
	}

	FunStatManager::instance().Stat("IMGateForPhoneManagerI.SendMessage.ok", ts.getTime(), (ts.getTime() >= 250.0));	

}

/*说明：3G定时发送在线id,进行心跳
 *参数1：验证的id
 */
void IMGateForPhoneManagerI::HeartBeat(const IntSeq &seq, const Ice::Current&) {
	TimeStatN ts;

	{
		IceUtil::RWRecMutex::RLock lock(_rwmutex);
		if(!_serverenable){
			MCE_WARN("IMGateForPhoneManagerI::HeartBeat --> server is disable now!");
      FunStatManager::instance().Stat("IMGateForPhoneManagerI::HeartBeat.disable", ts.getTime(), (ts.getTime() >= 250.0));	
			return;
		}
	}


  PhoneHeartBeatThread::instance().add_heart_seq(seq);
  FunStatManager::instance().Stat("IMGateForPhoneManagerI::HeartBeat", ts.getTime(), (ts.getTime() >= 250.0));
}

void IMGateForPhoneManagerI::PhoneTimerOnline(const IntSeq &seq, const Ice::Current&) {
	TimeStatN ts;

	MCE_INFO("IMGateForPhoneManagerI::PhoneTimerOnline --> seq.size = " << seq.size() << " stamp " << time(NULL));
  return;
	{
		IceUtil::RWRecMutex::RLock lock(_rwmutex);
		if(!_serverenable){
			MCE_WARN("IMGateForPhoneManagerI::PhoneTimerOnline --> server is disable now!");
      FunStatManager::instance().Stat("IMGateForPhoneManagerI::PhoneTimerOnline.disable", ts.getTime(), (ts.getTime() >= 250.0));	
			return;
		}
	}

  for (IntSeq::const_iterator it = seq.begin(); it != seq.end(); ++it) {
    if (!PhoneOnlineCheckThread::instance().online(*it)) {
      FunStatManager::instance().Stat("IMGateForPhoneManagerI::PhoneTimerOnlineSplit.beencall", 0, false);
      continue;
    }

    CallOnline(*it);
    FunStatManager::instance().Stat("IMGateForPhoneManagerI::PhoneTimerOnlineSplit.ok", 0, false);

  }

  FunStatManager::instance().Stat("IMGateForPhoneManagerI::PhoneTimerOnline", ts.getTime(), (ts.getTime() >= 250.0));
}

/*说明：3G通知IM用户上线
 *参数1：手机用户
 */
void IMGateForPhoneManagerI::PhoneUserOnline(int phoneuserid, const Ice::Current&){
	TimeStatN ts;
  /*
  PhoneRPCCountThread::instance().upcount_on();
  if (!PhoneRPCCountThread::instance().can_online()) {
    MCE_WARN("IMGateForPhoneManagerI::PhoneUserOnline --> ONLINE CALL COUNT OVERFLOW! phoneuserid " << phoneuserid);
    FunStatManager::instance().Stat("IMGateForPhoneManagerI.PhoneUserOnline.overflow", ts.getTime(), (ts.getTime() >= 250.0));	
    return;
  }
  */
	{
		IceUtil::RWRecMutex::RLock lock(_rwmutex);
		if(!_serverenable){
			MCE_WARN("IMGateForPhoneManagerI::PhoneUserOnline --> server is disable now!");
      FunStatManager::instance().Stat("IMGateForPhoneManagerI.PhoneUserOnline.disable", ts.getTime(), (ts.getTime() >= 250.0));	
			return;
		}
	}

  if (!PhoneOnlineCheckThread::instance().online(phoneuserid)) {
    FunStatManager::instance().Stat("IMGateForPhoneManagerI.PhoneUserOnline.beencall", ts.getTime(), (ts.getTime() >= 250.0));	
    return;
  }


  CallOnline(phoneuserid);

  FunStatManager::instance().Stat("IMGateForPhoneManagerI.PhoneUserOnline.ok", ts.getTime(), (ts.getTime() >= 250.0));
}

/*说明：手机用户上线更新IM在线和获取离线信息
 *参数1：手机用户
 */
void IMGateForPhoneManagerI::CallOnline(int phoneuserid) {
	IceUtil::RWRecMutex::RLock lock(_rwmutex);
	JidPtr fromjid = new Jid();
	fromjid->userId = phoneuserid;
	ostringstream os;
	//os << "PIMStorm" << (phoneuserid % (int)(IMGateForPhoneAdapter::instance().getCluster()));
	os << "PIMStorm" << phoneuserid % IMStormAdapter::instance().getCluster();
	fromjid->endpoint = os.str();
	fromjid->index = 2;
	MCE_INFO("IMGateForPhoneManagerI::CallOnline --> jid = " << jidToString(fromjid));

	try{
		OnlineCenterAdapter::instance().presence(fromjid, ONLINE, "phone", "", Str2StrMap());

	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::CallOnline -->OnlineCenterAdapter::presence--> error : " << e);
	}
  /*
	try{
		AvatarNPtr ava = new AvatarN();
		ava->jid = fromjid;
		ava->ext = "phone";
		ava->ver = "";
		ava->type=MemcGetOnlineType(fromjid->endpoint);
		ava->stat=MemcGetOnlineStat("");
		ava->hearttimestamp = time(NULL);
		TalkFunStateAdapter::instance().AvatarNOnline(ava);
	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::CallOnline-->MemcOnlineLogic userid:" << fromjid->userId << " error:" << e);
	}
  */

	try{
		TalkFunPhoneStateAdapter::instance().Online(phoneuserid);
	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::CallOnline-->TalkFunPhoneStateAdapter.Online userid:" << phoneuserid << " error:" << e);
	}


	try{
		MCE_INFO("IMGateForPhoneManagerI::CallOnline--> loadOfflineMessage jid:" << jidToString(fromjid));
		MessageHolderAdapter::instance().loadOfflineMessage(fromjid);
	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::CallOnline -->MessageHolderAdapter::loadOfflineMessage--> error : " << e);
	}


}


/*说明：手机用户下线更新IM在线
 *参数1：手机用户
 */
void IMGateForPhoneManagerI::PhoneUserOffline(int phoneuserid, const Ice::Current&){
	TimeStatN ts;
  /*
  PhoneRPCCountThread::instance().upcount_off();
  if (!PhoneRPCCountThread::instance().can_offline()) {
    MCE_WARN("IMGateForPhoneManagerI::PhoneUserOffline --> OFFLINE CALL COUNT OVERFLOW! phoneuserid " << phoneuserid);
    FunStatManager::instance().Stat("IMGateForPhoneManagerI.PhoneUserOffline.overflow", ts.getTime(), (ts.getTime() >= 250.0));
    return;
  }
  */


	{
		IceUtil::RWRecMutex::RLock lock(_rwmutex);
		if(!_serverenable){
			MCE_WARN("IMGateForPhoneManagerI::PhoneUserOffline --> server is disable now!");
      FunStatManager::instance().Stat("IMGateForPhoneManagerI.PhoneUserOffline.disable", ts.getTime(), (ts.getTime() >= 250.0));
			return;
		}
	}


  PhoneOnlineCheckThread::instance().offline(phoneuserid);
  CallOffline(phoneuserid);
  FunStatManager::instance().Stat("IMGateForPhoneManagerI.PhoneUserOffline.ok", ts.getTime(), (ts.getTime() >= 250.0));
}

/*说明：手机用户上线更新IM在线和获取离线信息
 *参数1：手机用户
 */
void IMGateForPhoneManagerI::CallOffline(int phoneuserid) {
	MCE_INFO("IMGateForPhoneManagerI::CallOffline --> phoneuserid = " << phoneuserid);

  try {
		TalkFunPhoneStateAdapter::instance().Offline(phoneuserid);
	} catch (Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::CallOnline-->TalkFunPhoneStateAdapter.Offline userid:" << phoneuserid << " error:" << e);
	}


	JidPtr fromjid = new Jid();
	fromjid->userId = phoneuserid;
	ostringstream os;
	//os << "PIMStorm" << (phoneuserid % (int)(IMGateForPhoneAdapter::instance().getCluster()));
	os << "PIMStorm" << phoneuserid % IMStormAdapter::instance().getCluster();
  //  MCE_INFO("IMGateForPhoneManagerI::CallOffline --> endpoint = " << os);
	fromjid->endpoint = os.str();
	fromjid->index = 2;
	try{
		PresenceMsgHandlerAdapter::instance().offline(fromjid);

	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::CallOffline -->PresenceMsgHandlerAdapter::offline--> error : " << e);
	}

}

/*说明：手机用户的消息回执
 *参数1：手机用户
 *参数1：消息id
 */
void IMGateForPhoneManagerI::NotifyMessageSended(int userid, Ice::Long msgKey, const Ice::Current&){
	TimeStatN ts;
	{
		IceUtil::RWRecMutex::RLock lock(_rwmutex);
		if(!_serverenable){
			MCE_WARN("IMGateForPhoneManagerI::NotifyMessageSended--> server is disable now!");
      FunStatManager::instance().Stat("IMGateForPhoneManagerI.NotifyMessageSended.disable", ts.getTime(), (ts.getTime() >= 250.0));
			return;
		}
	}
	try{
	 MessageHolderAdapter::instance().sendFor3G(userid, msgKey);
	 MCE_DEBUG("IMGateForPhoneManagerI::NotifyMessageSended---->MessageHolder::send msgkey " << msgKey);
	}catch(Ice::Exception& e){
	 MCE_WARN("IMGateForPhoneManagerI::NotifyMessageSended-->MessageHolderAdapter::send-->error:" << e);
	}

  FunStatManager::instance().Stat("IMGateForPhoneManagerI.NotifyMessageSended.ok", ts.getTime(), (ts.getTime() >= 250.0));
}
int IMGateForPhoneManagerI::getOfflineMessageCount(int userid, const Ice::Current&){
	TimeStatN ts;
  FunStatManager::instance().Stat("IMGateForPhoneManagerI.getOfflineMessageCount", ts.getTime(), (ts.getTime() >= 250.0));
	return 0;//3G暂时不用这个接口，所以现在直接返回0，之后如果需要用到再打开
	int count = 0;
	try{
		count = MessageHolderAdapter::instance().getOfflineMessageCount(userid);
		MCE_DEBUG("IMGateForPhoneManagerI::getOfflineMessageCount--->MessageHolder::getOfflineMessageCount");
	}catch(Ice::Exception& e){
		MCE_WARN("IMGateForPhoneManagerI::getOfflineMessageCount--->MessageHolder::getOfflineMessageCount--->error: " <<e);
	}
	return count;
}
void IMGateForPhoneManagerI::ServerDisable(const Ice::Current&){
	TimeStatN ts;
	MCE_INFO("IMGateForPhoneManagerI::ServerDisable");
	IceUtil::RWRecMutex::WLock lock(_rwmutex);
	_serverenable = false;

  FunStatManager::instance().Stat("IMGateForPhoneManagerI.ServerDisable", ts.getTime(), (ts.getTime() >= 250.0));

}
void IMGateForPhoneManagerI::ServerEnable(const Ice::Current&){
	TimeStatN ts;
	MCE_INFO("IMGateForPhoneManagerI::ServerEnable");
	IceUtil::RWRecMutex::WLock lock(_rwmutex);
	_serverenable = true;
  FunStatManager::instance().Stat("IMGateForPhoneManagerI.ServerEnable", ts.getTime(), (ts.getTime() >= 250.0));
}


// ==================================================
size_t OnlineCheckSeq::offline(int uid) {
  IceUtil::Mutex::Lock lock(mutex_);
  SetIndex &setidx = cache_.get<tag_users>();

  SetIndex::iterator it = setidx.find(uid);

  if (setidx.end() != it) {
    setidx.erase(it);
  } else {
    MCE_WARN("OnlineCheckSeq::offline-->can not find uid " << uid);
  }

  return setidx.size();

}
bool OnlineCheckSeq::online(int uid) {
  IceUtil::Mutex::Lock lock(mutex_);
  ListIndex &seqidx = cache_.get<tag_seq>();

  t_userPtr pu(new t_user(uid));
  std::pair<ListIndex::iterator, bool> p = cache_.push_front(pu);
  if (!p.second) {
    //    cache_.replace(p.first, pu);
    (*p.first)->set_stamp(time(NULL));
    cache_.relocate(cache_.begin(), p.first);
    MCE_DEBUG("OnlineCheckSeq::online--> relocate size " << seqidx.size()
             << " uid " << uid);

  }

  return p.second;
}

size_t OnlineCheckSeq::get_timeout(IntSeq &seq) {
  IceUtil::Mutex::Lock lock(mutex_);
  ListIndex &seqidx = cache_.get<tag_seq>();
  time_t nt = time(NULL);
  for (; !seqidx.empty();) {
    if (nt - seqidx.back()->stamp() >= timeout_) {
      seq.push_back(seqidx.back()->uid());
      seqidx.pop_back();
    } else {
      break;
    }
  }

  return seqidx.size();

}

void PhoneOnlineCheckThread::run() {
  IntSeq seq;
  size_t pool_size = 0;
  for (;;) {
    seq.clear();
    pool_size = 0;
    for (int i = 0; i < ONLINE_USER_POOL_SIZE; ++i) {
      pool_size += online_pool_[i].get_timeout(seq);
    }

    for (IntSeq::const_iterator it = seq.begin(); it != seq.end(); ++it) {
      IMGateForPhoneManagerI::instance().CallOffline(*it);
    }

    MCE_INFO("PhoneOnlineCheckThread::run --> pool_size size:" << pool_size);
    
    if (!seq.empty())
      MCE_INFO("PhoneOnlineCheckThread::run --> offline size:" << seq.size());

    sleep(DEFAULT_SLEEP_TIME);
  }
}



