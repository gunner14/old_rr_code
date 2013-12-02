#ifndef __IM_STORMI_H__
#define __IM_STORMI_H__

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "IMStorm.h"
#include "ServiceI.h"
#include <ext/hash_map>
#include "util/cpp/String.h"

namespace xce {
namespace talk{

using namespace std;
using namespace __gnu_cxx;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;

const static string IM_STORM = "M";
const static int kPeriodOfPingAllInMilliseconds = 20*60*1000;
const static int kPeriodOfPingOneInMilliseconds = 30*1000;
const static int kMaximumTimesOfPing = 2;
const static int kPeriodOfNotifyInMilliseconds = 30*1000;
const static int kMaximumTimesOfNotify = 3; //重试3次
const static int kMaximumTimerTaskChannel = 10;

const static int SYSTEMCHANNEL = -100;
const static int SYSTEMSERVERCOUNT = -100;

const static float TIMEOUT = 200.0;


struct ObserverLessTo: public binary_function<ObserverPrx, ObserverPrx, bool> {
	bool operator()(const ObserverPrx& __x, const ObserverPrx& __y) const {
		return __x->ice_getIdentity() < __y->ice_getIdentity();
	}
};

struct PhoneServerInfo{
public:
	PhoneServerInfo(){
		servercount = serverindex = 0;
	};
	PhoneServerInfo(int sc, int si){
		servercount = sc;
		serverindex = si;
	}
	int servercount;
	int serverindex;	
};

class IMStormI: public IMStorm, public IceUtil::Mutex, public Singleton<IMStormI> {
public:
	virtual void UserOnlineStatChange(int userid, int onlinestat, int onlinetype, const MyUtil::IntSeq& notifybuddys, const Ice::Current& = Ice::Current());
	virtual void NotifyPhoneServerDisable(const Ice::Current& = Ice::Current());
	virtual void NotifyPhoneServerEnalbe(const Ice::Current& = Ice::Current());
	virtual void express(const JidPtr& jid, const string& msg, const Ice::Current& = Ice::Current()){}
	virtual void deliver(const MessageSeq& msgs, const Ice::Current& = Ice::Current());
	virtual void deliverfeeds(const GFeedTargetSeq& GFeeds, const Ice::Current& = Ice::Current());
	virtual void notifyonlinestat(const OnlineStatNotify& stat, const Ice::Current& = Ice::Current());
  virtual void Notify3GMessageSended(int to, long msgKey, const Ice::Current& = Ice::Current());
	virtual void notify(const string& name, const MyUtil::Str2StrMap& context, int servercount, int channel, const Ice::Current& = Ice::Current());
	virtual void notifyfeeds(const string& name, const GFeedTargetSeq& GFeeds, int servercount, int channel, const Ice::Current& = Ice::Current());
	virtual void notifystat(const string& name, const OnlineStatNotify& stat, int servercount, int channel, const Ice::Current& = Ice::Current());
	virtual void notifyMessages(const string& name, const ForPhoneMessageSeq& context, int servercount, int channel, const Ice::Current& = Ice::Current());
	void deliverReceipt(const MsgSendedReceiptSeq& queue);
  void notifyReceipt(const string& name, const MsgSendedReceiptSeq& context, int servercount, int channel);
	virtual void subscribe(const string& name, const ObserverPrx& observer, int phoneservercount, int phoneserverindex, const Ice::Current& = Ice::Current());
	virtual void unsubscribe(const string& name, const ObserverPrx& observer, const Ice::Current& = Ice::Current());
	virtual ObserverPoolMap query(const string& name, const Ice::Current& = Ice::Current());

	string MsgToString(const MessageSeq& msgs);
  void MsgToForPhoneMessage(const MessageSeq& msgs, ForPhoneMessageSeq& for_phone_msgs);

private:
	typedef set<ObserverPrx, ObserverLessTo> ObserverSet;
	typedef map<ObserverPrx, PhoneServerInfo, ObserverLessTo> ObserverMapN;
	typedef hash_map<string, ObserverSet> ObserverSetMap;
	typedef hash_map<string, ObserverMapN> ObserverWithIndexMap;
	ObserverSetMap _pool;
	ObserverWithIndexMap _poolN;
  // 3G提供的服务器数量
	hash_map<string, int> expect_server_count_;
};



class NotifyTask: public MyUtil::Task {
public:
	NotifyTask(int channel, const string& name,
			const ObserverPrx& observer, const Ice::Context& context,
			int retry) :
		MyUtil::Task(channel), _name(name), _observer(observer),
				_context(context), _retry(retry) {
		;
	}
	virtual void handle();
	virtual void exception(const string& message);
private:
	string _name;
	ObserverPrx _observer;
	Ice::Context _context;
	int _retry;
};

class ForPhoneMessageTask: public MyUtil::Task {
	public:
		ForPhoneMessageTask(int channel, const string& name,
				const ObserverPrx& observer, const ForPhoneMessageSeq& PMsgs,
				int retry) :
			MyUtil::Task(channel), _name(name), _observer(observer),
			_context(PMsgs), _retry(retry) {
				;
			}
		virtual void handle();
		virtual void exception(const string& message);
	private:
		string _name;
		ObserverPrx _observer;
		ForPhoneMessageSeq _context;
		int _retry;
};

class GNotifyTask: public MyUtil::Task {
	public:
		GNotifyTask(int channel, const string& name,
				const ObserverPrx& observer, const GFeedTargetSeq& GFeeds,
				int retry) :
			MyUtil::Task(channel), _name(name), _observer(observer),
			_context(GFeeds), _retry(retry) {
				;
			}
		virtual void handle();
		virtual void exception(const string& message);
	private:
		string _name;
		ObserverPrx _observer;
		GFeedTargetSeq _context;
		int _retry;
};

class StatNotifyTask: public MyUtil::Task {
	public:
		StatNotifyTask(int channel, const string& name,
				const ObserverPrx& observer, const OnlineStatNotify& stat,
				int retry) :
			MyUtil::Task(channel), _name(name), _observer(observer),
			_context(stat), _retry(retry) {
				;
			}
		virtual void handle();
		virtual void exception(const string& message);
	private:
		string _name;
		ObserverPrx _observer;
		OnlineStatNotify _context;
		int _retry;
};

class ReceiptNotifyTask: public MyUtil::Task {
	public:
		ReceiptNotifyTask(int channel, const string& name,
				const ObserverPrx& observer, const MsgSendedReceiptSeq& context,
				int retry) :
			MyUtil::Task(channel), _name(name), _observer(observer),
			_context(context), _retry(retry) {
				;
			}
		virtual void handle();
		virtual void exception(const string& message);
	private:
		string _name;
		ObserverPrx _observer;
		MsgSendedReceiptSeq _context;
		int _retry;
};


class PingTimerTask: public MyUtil::Timer {
public:
	PingTimerTask(int delay, int channel, const string& name
			) :
		Timer(delay, channel), _name(name) {
		;
	}
	virtual void handle();
private:
	string _name;
};

// add by jian.wang3 向3G发送消息回执Task(全推)
class Notify3GMsgSendedTask : public MyUtil::Timer, 
  virtual public Singleton<Notify3GMsgSendedTask> {
public:
  Notify3GMsgSendedTask() : Timer(1000) {}
  void Push(int to, long msgKey) {
    MsgSendedReceipt tmp_receipt;
    tmp_receipt.to = to;
    tmp_receipt.msgKey = msgKey;
    
    IceUtil::Mutex::Lock lock(mutex_);
    receipt_queue_.push_back(tmp_receipt);
  }
  virtual void handle();
private:
  MsgSendedReceiptSeq receipt_queue_;
  IceUtil::Mutex mutex_;
};

}
;

}
;

#endif
