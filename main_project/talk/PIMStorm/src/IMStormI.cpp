#include "IMStormI.h"
#include "ServiceI.h"
#include "util/cpp/String.h"
#include "UserNameAdapter.h"
#include "pugixml.hpp"
#include "TalkCommon.h"
#include "XmppTools.h"
#include "MessageType.h"
#include <boost/lexical_cast.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::common;
using namespace pugi;
using namespace std;
using namespace xce::talk;
using namespace xce::adapter::username;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&IMStormI::instance(), service.createIdentity(IM_STORM, ""));
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerPIMStorm", &IMStormI::instance(), mod, interval, new TalkChannel());
  TaskManager::instance().scheduleRepeated(&(Notify3GMsgSendedTask::instance())); 
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
}

void IMStormI::notify(const string& name, const Str2StrMap& context, int servercount, int channel, const Ice::Current& current) {
	TimeStatN ts;
	IceUtil::Mutex::Lock lock(*this);
	bool success = false;

	ObserverWithIndexMap::iterator pos = _poolN.find(name);
	if (pos != _poolN.end()) {
		MCE_INFO("IMStormI::notify --> success name = " << name << " context.size = " << context.size() << " ObserverSetMap.size = " << _pool.size());
		ObserverMapN::iterator it = pos->second.begin();
		for(size_t i = 0; it != pos->second.end(); ++it, ++i){
			// The Timer channels for NotifyTimerTask are the range from 0 to 9
			if(SYSTEMCHANNEL == channel || (it->second.servercount == servercount && (channel % it->second.servercount) == it->second.serverindex)){
				TaskManager::instance().execute(new NotifyTask(i % kMaximumTimerTaskChannel, name, it->first, context, kMaximumTimesOfNotify));
				success = true;
			}
		}
	}
	MCE_DEBUG("IMStormI::notify --> find_name:" << name << " = " << (pos != _poolN.end()) << " context.size = " << context.size() << " ObserverSetMap.size = " << _poolN.size() << " find index success = " << success);
	FunStatManager::instance().Stat("IMStormI::notify", ts.getTime(), (ts.getTime() > TIMEOUT));
}
void IMStormI::NotifyPhoneServerDisable(const Ice::Current&){
	xml_document doc;
	xml_node messageListNode = doc.append_child();
	messageListNode.set_name("msglist");
	xml_node messageNode = messageListNode.append_child(); 
	messageNode.set_name("message");
	messageNode.append_attribute("type").set_value("system");
	messageNode.append_attribute("action").set_value("stop");
	string topic = "message";
	Str2StrMap paras;
	paras.insert(make_pair<string, string>(topic, doc.xml()));
	notify(topic, paras, SYSTEMSERVERCOUNT, SYSTEMCHANNEL);
}
void IMStormI::NotifyPhoneServerEnalbe(const Ice::Current&){
	xml_document doc;
	xml_node messageListNode = doc.append_child();
	messageListNode.set_name("msglist");
	xml_node messageNode = messageListNode.append_child(); 
	messageNode.set_name("message");
	messageNode.append_attribute("type").set_value("system");
	messageNode.append_attribute("action").set_value("start");
	string topic = "message";
	Str2StrMap paras;
	paras.insert(make_pair<string, string>(topic, doc.xml()));
	notify(topic, paras, SYSTEMSERVERCOUNT, SYSTEMCHANNEL);
}

/*说明：向3G推送状态的改变
 *参数1：状态数据
 */
void IMStormI::notifyonlinestat(const OnlineStatNotify& stat, const Ice::Current& current){
	TimeStatN ts;
  string topic = "message";
  int servercount = 0;
  vector<ObserverPrx> queue;
	{
		IceUtil::Mutex::Lock lock(*this);
		ObserverWithIndexMap::iterator pos = _poolN.find(topic);
    if(pos == _poolN.end())
      return;
	  MCE_INFO("IMStormI::notifyonlinestat << stat.uid " << stat.uid << "--> active server count = " << pos->second.size());
    servercount = pos->second.size();
		if(servercount == 0){
			return;
		}
    for(ObserverMapN::iterator it = pos->second.begin(); it != pos->second.end(); ++it) 
      queue.push_back(it->first);
	}

  int i = stat.uid % servercount;
  ObserverPrx tmp_prx;
  try {
    tmp_prx = queue.at(i);
	  MCE_INFO("IMStormI::deliverstat --> To 3G Server index = " << queue.at(i));
  } catch(...) {
    MCE_WARN("IMStormI::deliver --> get tmp_prx from queue error");
    return;
  }
	TaskManager::instance().execute(new StatNotifyTask(i % kMaximumTimerTaskChannel, topic, tmp_prx, stat, kMaximumTimesOfNotify));

	FunStatManager::instance().Stat("IMStormI::delverStat", ts.getTime(), (ts.getTime() > TIMEOUT));
}
void IMStormI::notifystat(const string& name, const OnlineStatNotify& stat, int servercount, int channel, const Ice::Current& current) {
  //暂时废弃
  return;
/*
	TimeStatN ts;
	IceUtil::Mutex::Lock lock(*this);
	bool success = false;
	//MCE_INFO("IMStormI::UserOnlineStatChange--> _servercountset size:" << _servercountset.size());
	if(SYSTEMSERVERCOUNT != servercount && _servercountset.count(servercount) <= 0){
		MCE_WARN("IMStormI::UserOnlineStatChange--> _servercountset not find servercount:" << servercount << " return.");
		return;
	}
	ObserverWithIndexMap::iterator pos = _poolN.find(name);
	if (pos != _poolN.end()) {
		//MCE_INFO("IMStormI::notify --> success name = " << name << " context.size = " << context.size() << " ObserverSetMap.size = " << _pool.size());    ObserverMapN::iterator it = pos->second.begin();
		ObserverMapN::iterator it = pos->second.begin();
		for(size_t i = 0; it != pos->second.end(); ++it, ++i){
			// The Timer channels for NotifyTimerTask are the range from 0 to 9
			if(SYSTEMCHANNEL == channel || (it->second.servercount == servercount && (channel % it->second.servercount) == it->second.serverindex)){
				TaskManager::instance().execute(new StatNotifyTask(i % kMaximumTimerTaskChannel, name, it->first, stat, kMaximumTimesOfNotify));
				success = true;
			}
		}
	}
	MCE_DEBUG("IMStormI::notifystat --> find_name:" << name << " = " << (pos != _poolN.end()) << " GFeeds.size" << " ObserverSetMap.size = " << _poolN.size() <<
			" find index success = " << success);  FunStatManager::instance().Stat("IMStormI::notifyStat", ts.getTime(), (ts.getTime() > TIMEOUT));
*/
}

/*说明：由Message信息向ForPhoneMessage转换
 *参数1：message数据
 *参数2：ForPhoneMessage结构数据
 */
void IMStormI::MsgToForPhoneMessage(const MessageSeq& msgs, ForPhoneMessageSeq& for_phone_msgs) {
  for (MessageSeq::const_iterator i = msgs.begin(); i != msgs.end(); i++) {
    xml_document doc;
    ForPhoneMessage msg;
    if (doc.load((*i)->msg.c_str())) 
      if (((*i)->type == MESSAGE) || ((*i)->type == OFFLINE_MESSAGE)) {
        msg.msg = doc.child("message").child("body").first_child().value();
        if (msg.msg.empty()) 
          continue;
	      msg.from = (*i)->from->userId;
	      msg.to = (*i)->to->userId;
        try {
	        UserNameInfoPtr un = UserNameAdapter::instance().getUserName((*i)->from->userId);
		      if (un) {
            msg.fname = un->name();
		      }
	      } catch(Ice::Exception& e) {
		      MCE_WARN("IMStormManagerI::MsgToPhoneMessage --> call UserNameAdapter::getUserName error :" << e);
	      }
        msg.msgkey = (*i)->msgKey;

        for_phone_msgs.push_back(msg);  
	      MCE_INFO("IMStormI::MessageStatistics from" << msg.from << " to " << msg.to << " msgkey "<< msg.msgkey);
      }
  }
}

/*说明：向3G服务器推送聊天消息
 *参数1：聊天数据
 */
void IMStormI::deliver(const MessageSeq& msgs, const Ice::Current&){
	TimeStatN ts;
	string topic = "message";
  int servercount = 0;
  vector<ObserverPrx> queue;
	{
		IceUtil::Mutex::Lock lock(*this);
		ObserverWithIndexMap::iterator pos = _poolN.find(topic);
    if(pos == _poolN.end())
      return;
		MCE_INFO("IMStormI::deliverMessage --> active server count = " << pos->second.size());
    servercount = pos->second.size();
		if(servercount == 0){
			return;
		}
    for(ObserverMapN::iterator it = pos->second.begin(); it != pos->second.end(); ++it) 
      queue.push_back(it->first);
	}
	map<int, MessageSeq> splitmsgs;
	map<int, MessageSeq>::iterator spit;
	for(MessageSeq::const_iterator it = msgs.begin(); it != msgs.end(); ++it){
		int index = ((*it)->to->userId) % servercount;
		spit = splitmsgs.find(index);
		if(spit == splitmsgs.end()){
			splitmsgs.insert(make_pair<int, MessageSeq>(index, MessageSeq()));
		}
		splitmsgs[index].push_back((*it));
	}
	for(spit = splitmsgs.begin(); spit != splitmsgs.end(); ++spit){
		ForPhoneMessageSeq to_phone_msgs;
    MsgToForPhoneMessage(spit->second, to_phone_msgs);
    ObserverPrx tmp_prx;
    try {
      tmp_prx = queue.at(spit->first);
		  MCE_INFO("IMStormI::deliver --> To 3G Server index = " << queue.at(spit->first));
    } catch(...) {
		  MCE_WARN("IMStormI::deliver --> get tmp_prx from queue error");
      return;
    }
		TaskManager::instance().execute(new ForPhoneMessageTask(spit->first % kMaximumTimerTaskChannel, topic, tmp_prx, to_phone_msgs, kMaximumTimesOfNotify));
	}

	FunStatManager::instance().Stat("IMStormI::delverMessage", ts.getTime(), (ts.getTime() > TIMEOUT));
}
void IMStormI::notifyMessages(const string& name, const ForPhoneMessageSeq& context, int servercount, int channel, const Ice::Current& current) {
  //暂时废弃
  return;
/*
	TimeStatN ts;
	IceUtil::Mutex::Lock lock(*this);
	bool success = false;
	MCE_INFO("3GIMStormI::notifyMessage--> _servercountset size:" << _servercountset.size());
	if(SYSTEMSERVERCOUNT != servercount && _servercountset.count(servercount) <= 0){
		MCE_WARN("IMStormI::notifyMessage--> _servercountset not find servercount:" << servercount << " return.");
		return;
	}
	ObserverWithIndexMap::iterator pos = _poolN.find(name);
	if (pos != _poolN.end()) {
		MCE_INFO("IMStormI::notifyMessages --> success name = " << name << " context.size = " << context.size() << " ObserverSetMap.size = " << _pool.size());
		ObserverMapN::iterator it = pos->second.begin();
		for(size_t i = 0; it != pos->second.end(); ++it, ++i){
			// The Timer channels for NotifyTimerTask are the range from 0 to 9
			if(SYSTEMCHANNEL == channel || (it->second.servercount == servercount && (channel % it->second.servercount) == it->second.serverindex)){
				TaskManager::instance().execute(new ForPhoneMessageTask(i % kMaximumTimerTaskChannel, name, it->first, context, kMaximumTimesOfNotify));
				success = true;
			}
		}
	}
	MCE_DEBUG("IMStormI::notifyMessages --> find_name:" << name << " = " << (pos != _poolN.end()) << " context.size = " << context.size() << " ObserverSetMap.size = " << _poolN.size() << " find index success = " << success);
	FunStatManager::instance().Stat("IMStormI::notifyMessages", ts.getTime(), (ts.getTime() > TIMEOUT));
*/
}
//##############################################################

/*说明:向3G发送消息回执
 *参数1：发送目的用户
 *参数2：回执消息id
 */
void IMStormI::Notify3GMessageSended(int to, long msgKey, const Ice::Current& current) {
  Notify3GMsgSendedTask::instance().Push(to, msgKey);
}
/*说明:定时向3G发送消息回执
 */
void Notify3GMsgSendedTask::handle() {
  MsgSendedReceiptSeq tmp_queue;
  {
    IceUtil::Mutex::Lock lock(mutex_);
    tmp_queue.swap(receipt_queue_);
  }
  if(tmp_queue.empty())
    return;

  IMStormI::instance().deliverReceipt(tmp_queue);
}
/*说明:向3G发送消息回执
 *参数1：发送消息
 */
void IMStormI::deliverReceipt(const MsgSendedReceiptSeq& queue) {
  TimeStatN ts;
	string topic = "message";
  int servercount = 0;
  vector<ObserverPrx> obj_queue;
	{
		IceUtil::Mutex::Lock lock(*this);
		ObserverWithIndexMap::iterator pos = _poolN.find(topic);
    if(pos == _poolN.end())
      return;
		MCE_INFO("IMStormI::deliverReceipt --> active server count = " << pos->second.size() << " queue.size-->" << queue.size());
    servercount = pos->second.size();
		if(servercount == 0){
			return;
		}
    for(ObserverMapN::iterator it = pos->second.begin(); it != pos->second.end(); ++it) 
      obj_queue.push_back(it->first);
	}

  map<int, MsgSendedReceiptSeq> split_receipts;
	map<int, MsgSendedReceiptSeq>::iterator spit;
	for(MsgSendedReceiptSeq::const_iterator it = queue.begin(); it != queue.end(); ++it){
		int index = ((*it).to) % servercount;
		spit = split_receipts.find(index);
		if(spit == split_receipts.end())
			split_receipts.insert(make_pair<int, MsgSendedReceiptSeq>(index, MsgSendedReceiptSeq()));
		split_receipts[index].push_back((*it));
	}
	for(spit = split_receipts.begin(); spit != split_receipts.end(); ++spit){
		MsgSendedReceiptSeq to_phone_receipts;
    to_phone_receipts.insert(to_phone_receipts.end(), spit->second.begin(), spit->second.end());
    ObserverPrx tmp_prx;
    try {
      tmp_prx = obj_queue.at(spit->first);
		  MCE_INFO("IMStormI::deliverReceipt --> To 3G Server index = " << obj_queue.at(spit->first));
    } catch(...) {
		  MCE_WARN("IMStormI::deliver --> get tmp_prx from queue error");
      return;
    }

		TaskManager::instance().execute(new ReceiptNotifyTask(spit->first % kMaximumTimerTaskChannel, topic, tmp_prx, 
        to_phone_receipts, kMaximumTimesOfNotify));
	}

	FunStatManager::instance().Stat("IMStormI::deliverReceipt ", ts.getTime(), (ts.getTime() > TIMEOUT));
}
void IMStormI::notifyReceipt(const string& name, const MsgSendedReceiptSeq& context, int servercount, int channel) {
  //暂时废弃
  return;
/*
	TimeStatN ts;
	IceUtil::Mutex::Lock lock(*this);
	if(SYSTEMSERVERCOUNT != servercount && _servercountset.count(servercount) <= 0){
		MCE_WARN("Notify3GMsgSendedTask::notifyReceipt--> _servercountset not find servercount:" << servercount << " return.");
		return;
	}
	ObserverWithIndexMap::iterator pos = _poolN.find(name);
	if (pos != _poolN.end()) {
		MCE_INFO("Notify3GMessageSendede--> success name = " << name << " ObserverSetMap.size = " << _pool.size());
		ObserverMapN::iterator it = pos->second.begin();
		for(size_t i = 0; it != pos->second.end(); ++it, ++i){
			// The Timer channels for NotifyTimerTask are the range from 0 to 9
			if(SYSTEMCHANNEL == channel || (it->second.servercount == servercount && (channel % it->second.servercount) == it->second.serverindex)){
       	try{
				  TaskManager::instance().execute(new ReceiptNotifyTask(i % kMaximumTimerTaskChannel, name, it->first, 
            context, kMaximumTimesOfNotify));
	      }catch (Ice::Exception& e) {
	      	MCE_WARN("Notify3GMessageSended error" << e.what());
	      }
			}
		}
	}
	MCE_DEBUG("Notify3GMsgSendedTask::notifyReceipt--> find_name:" << name << " = " << (pos != _poolN.end()) << " context.size = " << context.size() << " ObserverSetMap.size = " << _poolN.size() << " receipt size = " << context.size());

	FunStatManager::instance().Stat("Notify3GMsgSendedTask::notifyReceipt", ts.getTime(), (ts.getTime() > TIMEOUT));
*/
}

/*说明：消息回执task
 */
void ReceiptNotifyTask::handle() {
	MCE_DEBUG("receipts.size" << _context.size());
	try{
		_observer->updateMessageSended(_context);
	}catch (Ice::Exception& e) {
		MCE_WARN("MsgSendedReceiptSeq--> " << e.what());
	}
}
/*说明：消息回执task异常处理
 *参数1：异常信息
 */
void ReceiptNotifyTask::exception(const string& message) {
	if (_retry-- > 0) {
		TaskManager::instance().execute(new ReceiptNotifyTask(level(), _name, _observer, _context, _retry));
	} else {
		IMStormI::instance().unsubscribe(_name, _observer);
		MCE_WARN("NotifyTimerTask::handle exception(removed) -> " << _name << " -> " << _observer << " -> " << message);
	}
}
//####################################################################################

/*说明：向3G服务器推送新鲜事
 *参数1：新鲜事数据
 */
void IMStormI::deliverfeeds(const GFeedTargetSeq& GFeeds, const Ice::Current&){
	TimeStatN ts;
	string topic = "message";
  int servercount = 0;
  vector<ObserverPrx> queue;
	{
		IceUtil::Mutex::Lock lock(*this);
		ObserverWithIndexMap::iterator pos = _poolN.find(topic);
    if(pos == _poolN.end())
      return;
		MCE_INFO("IMStormI::deliverfeed --> active server count = " << pos->second.size() << " GFeeds.size" << GFeeds.size());
    servercount = pos->second.size();
		if(servercount == 0){
			return;
		}
    for(ObserverMapN::iterator it = pos->second.begin(); it != pos->second.end(); ++it) 
      queue.push_back(it->first);
	}

	map<int, GFeedTargetSeq> splitmsgs;
	map<int, GFeedTargetSeq>::iterator spit;
	for(GFeedTargetSeq::const_iterator it = GFeeds.begin(); it != GFeeds.end(); ++it){
		int index = ((*it).actor) % servercount;
		spit = splitmsgs.find(index);
		if(spit == splitmsgs.end()){
			splitmsgs.insert(make_pair<int, GFeedTargetSeq>(index, GFeedTargetSeq()));
		}
		splitmsgs[index].push_back((*it));
	}
	for(spit = splitmsgs.begin(); spit != splitmsgs.end(); ++spit){
		string topic = "message";
		GFeedTargetSeq paras;
		paras.insert(paras.end(),spit->second.begin(),spit->second.end());
    ObserverPrx tmp_prx;
    try {
      tmp_prx = queue.at(spit->first);
		  MCE_INFO("IMStormI::deliverfeed --> To 3G Server index = " << queue.at(spit->first));
    } catch(...) {
		  MCE_WARN("IMStormI::deliver --> get tmp_prx from queue error");
      return;
    }

		TaskManager::instance().execute(new GNotifyTask(spit->first % kMaximumTimerTaskChannel, topic, tmp_prx, paras, kMaximumTimesOfNotify));
	}

	FunStatManager::instance().Stat("IMStormI::delverFeed", ts.getTime(), (ts.getTime() > TIMEOUT));
}
void IMStormI::notifyfeeds(const string& name, const GFeedTargetSeq& GFeeds, int servercount, int channel, const Ice::Current& current) {
  //暂时废弃
  return;
/*
	TimeStatN ts;
	IceUtil::Mutex::Lock lock(*this);
	bool success = false;
	//MCE_INFO("IMStormI::UserOnlineStatChange--> _servercountset size:" << _servercountset.size());
	if(SYSTEMSERVERCOUNT != servercount && _servercountset.count(servercount) <= 0){
		MCE_WARN("IMStormI::UserOnlineStatChange--> _servercountset not find servercount:" << servercount << " return.");
		return;
	}
	ObserverWithIndexMap::iterator pos = _poolN.find(name);
	if (pos != _poolN.end()) {
		//MCE_INFO("IMStormI::notify --> success name = " << name << " context.size = " << context.size() << " ObserverSetMap.size = " << _pool.size());
		ObserverMapN::iterator it = pos->second.begin();
		for(size_t i = 0; it != pos->second.end(); ++it, ++i){
			MCE_DEBUG("xxxx" << it->first);
			// The Timer channels for NotifyTimerTask are the range from 0 to 9
			if(SYSTEMCHANNEL == channel || (it->second.servercount == servercount && (channel % it->second.servercount) == it->second.serverindex)){
				TaskManager::instance().execute(new GNotifyTask(i % kMaximumTimerTaskChannel, name, it->first, GFeeds, kMaximumTimesOfNotify));
				success = true;
			}
		}
	}
	MCE_DEBUG("3GIMStormI::notifyfeeds --> find_name:" << name << " = " << (pos != _poolN.end()) << " GFeeds.size = " << GFeeds.size() << " ObserverSetMap.size = " << _poolN.size() << " find index success = " << success);
	FunStatManager::instance().Stat("IMStormI::notifyFeed", ts.getTime(), (ts.getTime() > TIMEOUT));
*/
}

/*说明：3G注册服务器
 *参数1: 注册类型名称
 *参数2：3G消息处理接口代理
 *参数3：3G服务器数量
 *参数4：当前注册服务器索引号
 */
void IMStormI::subscribe(const string& name, const ObserverPrx& observer, int phoneservercount, int phoneserverindex, const Ice::Current&) {
	TimeStatN ts;
	IceUtil::Mutex::Lock lock(*this);

	MCE_INFO("IMStormI::subscribe --> name=" << name << " observer=" << observer << " servercount=" << phoneservercount << " serverindex=" << phoneserverindex);

	ObserverPrx timeoutPrx = ObserverPrx::uncheckedCast(observer->ice_timeout(300)->ice_twoway());
	ObserverWithIndexMap::iterator pos = _poolN.find(name);
	if (pos != _poolN.end()) {
    if((size_t)phoneservercount != pos->second.size())
      MCE_WARN("IMStormI::subscribe --> different cout phoneservercount:" << phoneservercount 
        << " activeservercount" << pos->second.size());
      
	  if(phoneservercount != expect_server_count_[name]) {
      //3G服务器有变化
      MCE_WARN("IMStormI::subscribe --> 3g server count change :" << expect_server_count_[name] << " TO " << phoneservercount);
      expect_server_count_[name] = phoneservercount;
      pos->second.clear();
    }
    
		for(ObserverMapN::iterator it = pos->second.begin(); it != pos->second.end(); ) {
      // 以index作为服务器的 3G替换服务器
      if(it->first == timeoutPrx || it->second.serverindex == phoneserverindex)
			  pos->second.erase(it++);
      else
        ++it;
    }
		pos->second.insert(make_pair<ObserverPrx, PhoneServerInfo>(timeoutPrx, PhoneServerInfo(phoneservercount, phoneserverindex)));

    MCE_DEBUG("IMStormI::subscribe --> server count:" << expect_server_count_[name] << " : " << pos->second.size());
	} else {
		ObserverMapN mp;
		mp.insert(make_pair<ObserverPrx, PhoneServerInfo>(timeoutPrx, PhoneServerInfo(phoneservercount, phoneserverindex)));
		_poolN.insert(make_pair<string, ObserverMapN>(name, mp));
    expect_server_count_[name] = phoneservercount;
		// the Timer channel for PingTimerTask are the range from -1 to -11,
		// so, all PingTimerTasks are executed in different channels
		// which NotifyTimerTasks are using, thus, these two kinds of TimerTask
		// will not disturb each others
    // 新注册一种类型，就会启动一个定时器，定时检测proxy是否有效
		TaskManager::instance().scheduleRepeated(new PingTimerTask( kPeriodOfPingAllInMilliseconds, 0, name));
	}
	FunStatManager::instance().Stat("IMStormI::subscribe", ts.getTime(), (ts.getTime() > TIMEOUT));
}

/*说明：3G注销服务器
 *参数1: 注册类型名称
 *参数2：3G消息处理接口代理
 */
void IMStormI::unsubscribe(const string& name, const ObserverPrx& observer, const Ice::Current&) {
	TimeStatN ts;
	MCE_INFO("IMStormI::unsubscribe --> name = " << name << " observer = " << observer);
	IceUtil::Mutex::Lock lock(*this);
	ObserverWithIndexMap::iterator pos = _poolN.find(name);
	if (pos != _poolN.end()) {
		ObserverMapN::iterator it = pos->second.find(observer);
		if(it != pos->second.end()){
			pos->second.erase(it);
		}
	}
	FunStatManager::instance().Stat("IMStormI::unsubscribe", ts.getTime(), (ts.getTime() > TIMEOUT));
}

/*说明：取出name类型的所有proxy，如果name 为空取所有proxy
 *参数1: 注册类型名称
 */
ObserverPoolMap IMStormI::query(const string& name, const Ice::Current&) {
	ObserverPoolMap result;
	IceUtil::Mutex::Lock lock(*this);
	if (name.empty()) {
		for (ObserverWithIndexMap::iterator i = _poolN.begin(); i != _poolN.end(); ++i) {
			for (ObserverMapN::iterator j = i->second.begin(); j != i->second.end(); ++j) {
				result[i->first].push_back(j->first);
			}
		}
	} else {
		ObserverWithIndexMap::iterator pos = _poolN.find(name);
		if (pos != _poolN.end()) {
			for (ObserverMapN::iterator j = pos->second.begin(); j
					!= pos->second.end(); ++j) {
				result[name].push_back(j->first);
			}
		}
	}
	return result;
}

/*说明：推送聊天消息Task
 */
void ForPhoneMessageTask::handle() {
	try{
	_observer->updateMessages(_context);
	}catch (Ice::Exception& e) {
		MCE_WARN("3GNotifyTask->-> " << e.what());
    exception(e.what());
	}
}
/*说明：推送聊天Task异常处理
 *参数1：异常信息
 */
void ForPhoneMessageTask::exception(const string& message) {
	if (_retry-- > 0) {
		TaskManager::instance().execute(new ForPhoneMessageTask(level(), _name, _observer, _context, _retry));
	} else {
		IMStormI::instance().unsubscribe(_name, _observer);
		MCE_WARN("NotifyTimerTask::handle exception(removed) -> " << _name << " -> " << _observer << " -> " << message);
	}
}

/*说明：推送新鲜事Task
 */
void GNotifyTask::handle() {
	MCE_DEBUG("3Gcontext.size" << _context.size());
	try{
		_observer->updatefeeds(_context);
	}catch (Ice::Exception& e) {
		MCE_WARN("GNotifyTask->-> " << e.what());
	}
}
/*说明：推送新鲜事Task异常处理
 *参数1：异常信息
 */
void GNotifyTask::exception(const string& message) {
	if (_retry-- > 0) {
		TaskManager::instance().execute(new GNotifyTask(level(), _name, _observer, _context, _retry));
	} else {
		IMStormI::instance().unsubscribe(_name, _observer);
		MCE_WARN("NotifyTimerTask::handle exception(removed) -> " << _name << " -> " << _observer << " -> " << message);
	}
}

/*说明：推送状态Task
 */
void StatNotifyTask::handle() {
	MCE_DEBUG("3Gstat.uid" << _context.uid);
	try{
		_observer->updatestat(_context);
	}catch (Ice::Exception& e) {
		MCE_WARN("3Gstat->-> " << e.what());
	}

}
/*说明：推送状态Task异常处理
 *参数1：异常信息
 */
void StatNotifyTask::exception(const string& message) {
	if (_retry-- > 0) {
		TaskManager::instance().execute(new StatNotifyTask(level(), _name, _observer, _context, _retry));
	} else {
		IMStormI::instance().unsubscribe(_name, _observer);
		MCE_WARN("NotifyTimerTask::handle exception(removed) -> " << _name << " -> " << _observer << " -> " << message);
	}
}

void NotifyTask::handle() {
	MCE_DEBUG("3Gcontext.size" << _context.size());
	try{
	_observer->update(_context);
	}catch (Ice::Exception& e) {
		MCE_WARN("3GNotifyTask->-> " << e.what());
	}
}
void NotifyTask::exception(const string& message) {
	if (_retry-- > 0) {
		TaskManager::instance().execute(new NotifyTask(level(), _name, _observer, _context, _retry));
	} else {
		IMStormI::instance().unsubscribe(_name, _observer);
		MCE_WARN("NotifyTimerTask::handle exception(removed) -> " << _name << " -> " << _observer << " -> " << message);
	}
}

/*说明：定时检测注册类型为name的proxy
 */
void PingTimerTask::handle() {
	size_t success = 0, failure = 0;
	ObserverPoolMap pool = IMStormI::instance().query(_name);
	for (ObserverPoolMap::iterator it = pool.begin(); it != pool.end(); ++it) {
		for (size_t i = 0; i < it->second.size(); ++i) {
			try {
				it->second.at(i)->ice_ping();
				++success;
				//MCE_DEBUG("PingTimerTask::handle success -> " << _name << " -> " << it->second.at(i));
			} catch (Ice::Exception& e) {;
				IMStormI::instance().unsubscribe(_name, it->second.at(i));
				++failure;
				MCE_WARN("PingTimerTask::handle exception(removed) -> " << _name << " -> " << it->second.at(i) << " -> " << e.what())
			}
		}
	}
	MCE_INFO("PingTimerTask::handle -> summary " << _name << " success: " << success << ", failure: " << failure);
}

string IMStormI::MsgToString(const MessageSeq& msgs){
	TimeStatN ts;
	ostringstream os;
	os << "<msglist>";
	MessageSeq::const_iterator it = msgs.begin();
	
	string message_str = "";

	for(; it!=msgs.end(); ++it){
		xml_document doc;
		MessagePtr msg = (*it);
		if (doc.load(msg->msg.c_str())) {
			if ((msg->type == MESSAGE) || (msg->type == OFFLINE_MESSAGE)) {
				string m = doc.child("message").child("body").first_child().value();
				if (m.empty()) {
					continue;
				}
				xml_document doc1;
				xml_node messageNode = doc1.append_child();
				if(MESSAGE == msg->type){
					messageNode.set_name("message");
				}else{
					messageNode.set_name("offlinemessage");
				}
				messageNode.append_attribute("type").set_value("chat");
				if(MESSAGE == msg->type){
					messageNode.append_attribute("msgkey").set_value(boost::lexical_cast<string>(msg->msgKey).c_str());
				}
				//messageNode.append_attribute("id") = boost::lexical_cast<string>(it->first).c_str();
				messageNode.append_attribute("from").set_value(jidToString(msg->from).c_str());
				//messageNode.append_attribute("from").set_value((boost::lexical_cast<string>(msg->from->userId) + "@talk.renren.com").c_str());
				try {
					UserNameInfoPtr un = UserNameAdapter::instance().getUserName(msg->from->userId);
					if (un) {
						messageNode.append_attribute("fname").set_value(un->name().c_str());
					}
				} catch(Ice::Exception& e) {
					MCE_WARN("IMStormManagerI::MsgToString --> call UserNameAdapter::getUserName error :" << e);
				}
				messageNode.append_attribute("to").set_value(jidToString(msg->to).c_str());
				//messageNode.append_attribute("from").set_value((boost::lexical_cast<string>(msg->to->userId) + "@talk.renren.com").c_str());

				xml_node bodyNode = messageNode.append_child();
				bodyNode.set_name("body");
				bodyNode.append_child(node_pcdata).set_value(m.c_str());

				if(OFFLINE_MESSAGE == msg->type){
					xml_node delayNode = messageNode.append_child();
					delayNode.set_name("delay");
					string t = doc.child("message").child("delay").attribute("stamp").value().c_str();
					Date d(t, "%Y-%m-%dT%H:%M:%S+08:00Z");
					//MCE_INFO("IMStormManagerI::MsgToString--> delay string t = " << t);
					delayNode.append_attribute("stamp").set_value(boost::lexical_cast<string>(d.time()).c_str());

				}
				message_str = doc1.xml();
				FunStatManager::instance().Stat("IMStormI::MsgToString::MESSAGE", ts.getTime(), (ts.getTime() > 50.0));
			} else if (msg->type == PIMSTORM_MESSAGE){
				xml_node msgNode = doc.child("message");
				msgNode.append_attribute("type") = "storm_feed";
				message_str = doc.xml();
				MCE_DEBUG("IMStormManagerI::Message of PIMStorm: " << message_str);
				FunStatManager::instance().Stat("IMStormI::MsgToString::PIMSTORM_MESSAGE", ts.getTime(), (ts.getTime() > 50.0));
			}
//			 else if (msg->type == NOTIFY_MESSAGE) {
//				xml_node msgNode = doc.child("message");
//				msgNode.append_attribute("type") = "notify";
//				//msgNode.append_attribute("id") = boost::lexical_cast<string>(it->first).c_str();
//				message_str = doc.xml();
//			} else if (msg->type == FEED_STATUS) {
//				xml_node msgNode = doc.child("message");
//				msgNode.append_attribute("type") = "feed_status";
//				//msgNode.append_attribute("id") = boost::lexical_cast<string>(it->first).c_str();
//				message_str = doc.xml();
//			} else if(msg->type == PHONE_NOTIFY){
//				xml_node msgNode = doc.child("message");
//				msgNode.append_attribute("type") = "phone_notify";
//				//msgNode.append_attribute("id") = boost::lexical_cast<string>(it->first).c_str();
//				message_str = doc.xml();
//			}
		}
		else{
			MCE_WARN("load message error  msg:" << msg->msg);
		}
		os << message_str;
	}
	os << "</msglist>";
	MCE_DEBUG("3GIMStormManagerI::MsgToString-->" << os.str());
	FunStatManager::instance().Stat("IMStormI::MsgToString", ts.getTime(), (ts.getTime() > 50.0));
	return os.str();
}
void IMStormI::UserOnlineStatChange(int userid, int onlinestat, int onlinetype, const MyUtil::IntSeq& notifybuddys, const Ice::Current&){
  // 暂时废弃
  return;
/*
	MCE_DEBUG("3G已经调用UserOnlineStatChange");
	TimeStatN ts;
	set<int> servercountset;
	{
		IceUtil::Mutex::Lock lock(*this);
		//MCE_INFO("IMStormI::UserOnlineStatChange--> _servercountset size:" << _servercountset.size());
		if(_servercountset.empty()){
			return;
		}
		servercountset = _servercountset;
	}

	for(set<int>::iterator setit = servercountset.begin(); setit != servercountset.end(); ++setit){
		int servercount = *setit;

		map<int, IntSeq> splitbuddys;
		map<int, IntSeq>::iterator spit;

		for(IntSeq::const_iterator it = notifybuddys.begin(); it != notifybuddys.end(); ++it){
			int index = (*it) % servercount;
			spit = splitbuddys.find(index);
			if(spit == splitbuddys.end()){
				splitbuddys.insert(make_pair<int, IntSeq>(index, IntSeq()));
			}
			splitbuddys[index].push_back((*it));
		}

		MCE_INFO("IMStormI::UserOnlineStatChange --> userid:" << userid << " onlinestat:" << onlinestat << " onlinetype:" << onlinetype << " buddys.size:" << notifybuddys.size() << " splitbuddys.size:" << splitbuddys.size());
		for(spit = splitbuddys.begin(); spit != splitbuddys.end(); ++spit){
			xml_document doc;
			xml_node messageListNode = doc.append_child();
			messageListNode.set_name("msglist");
			for(IntSeq::const_iterator it = spit->second.begin(); it != spit->second.end(); ++it){
				xml_node messageNode = messageListNode.append_child(); 
				messageNode.set_name("message");
				messageNode.append_attribute("type").set_value("presence");
				messageNode.append_attribute("from").set_value(boost::lexical_cast<string>(userid).c_str());
				messageNode.append_attribute("to").set_value(boost::lexical_cast<string>((*it)).c_str());
				xml_node onlinestatNode = messageNode.append_child(); 
				onlinestatNode.set_name("onlinestat");
				onlinestatNode.append_child(node_pcdata).set_value(boost::lexical_cast<string>(onlinestat).c_str());
				xml_node onlinetypeNode = messageNode.append_child(); 
				onlinetypeNode.set_name("onlinetype");
				onlinetypeNode.append_child(node_pcdata).set_value(boost::lexical_cast<string>(onlinetype).c_str());
			}
			//MCE_INFO("IMStormI::UserOnlineStatChange --> msg=" << doc.xml());
			string topic = "message";
			Str2StrMap paras;
			paras.insert(make_pair<string, string>(topic, doc.xml()));
			notify(topic, paras, servercount, spit->first);
		}
	}
	FunStatManager::instance().Stat("IMStormI::UserOnlineStatChange", ts.getTime(), (ts.getTime() > TIMEOUT));
*/
}


