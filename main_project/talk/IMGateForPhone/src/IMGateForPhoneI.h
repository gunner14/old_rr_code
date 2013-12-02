#ifndef _IMGATEFORPHONE_
#define _IMGATEFORPHONE_

#include "Singleton.h"
#include "ServiceI.h"
#include "IMGateForPhone.h"
#include "XmppTools.h"


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>

using namespace MyUtil;
//using namespace com::xiaonei::xce;
using namespace std;
using boost::multi_index_container;
using namespace boost::multi_index;

namespace com{
namespace xiaonei{
namespace talk{
namespace gate{

//----------------------------------------------------------------------
class OnlineCheckSeq {

  class t_user {
    int uid_;
    time_t stamp_;
  public:
  t_user(int uid) : uid_(uid), stamp_(time(NULL)) {
    }

    void set_stamp(const time_t t) {
      stamp_ = t;
    }
    int uid() const { return uid_; }
    time_t stamp() const { return stamp_; }

  };

  typedef boost::shared_ptr<t_user>  t_userPtr;


  struct tag_seq {};
  struct tag_users {};
  typedef multi_index_container<
    t_userPtr,
    indexed_by<
			sequenced< tag<tag_seq> >,
    	hashed_unique< tag<tag_users>,
    		BOOST_MULTI_INDEX_CONST_MEM_FUN(t_user, int, uid)
    	>
    >
  > t_cache;
  typedef t_cache::index<tag_seq>::type ListIndex;
  typedef t_cache::index<tag_users>::type SetIndex;


  IceUtil::Mutex mutex_;
  t_cache cache_;
  int timeout_;
  enum {
    DEFAULT_ONLINE_TIME_OUT = 60 * 60 * 24 * 1, //一天
  };

 public:
 OnlineCheckSeq() : timeout_(DEFAULT_ONLINE_TIME_OUT) {
  }
  int timeout() { return timeout_; }
  void set_timeout(int tm) { timeout_ = tm; }
  bool online(int uid);
  size_t offline(int uid);
  // 返回当前的size
  size_t get_timeout(IntSeq &seq);

};


class PhoneOnlineCheckThread : virtual public IceUtil::Thread,
virtual public Singleton<PhoneOnlineCheckThread> {
  enum {
    ONLINE_USER_POOL_SIZE = 123, // 不要取个整的数，不然分不均
    DEFAULT_SLEEP_TIME = 60, // 检测间隔
  };
  OnlineCheckSeq online_pool_[ONLINE_USER_POOL_SIZE];
 public:
  PhoneOnlineCheckThread()
    {
      start(65535).detach();
    }
  virtual void run();
  bool online(int uid) {
    return online_pool_[uid % ONLINE_USER_POOL_SIZE].online(uid);
  }

  void offline(int uid) {
    online_pool_[uid % ONLINE_USER_POOL_SIZE].offline(uid);
  }

  void set_timeout(int tm) {
    for (int i = 0; i < ONLINE_USER_POOL_SIZE; ++i) {
      MCE_WARN("PhoneOnlineCheckThread::set_timeout-->before i " << i << " timeout " << online_pool_[i].timeout());
      online_pool_[i].set_timeout(tm);
      MCE_WARN("PhoneOnlineCheckThread::set_timeout-->after i " << i << " timeout " << online_pool_[i].timeout());
    }

  }

};

class PhoneRPCCountThread : virtual public IceUtil::Thread,
virtual public Singleton<PhoneRPCCountThread> {
    volatile int count_off_;
    int thresh_count_off_;

    volatile int count_on_;
    int thresh_count_on_;

    enum {
      DEFAULT_THREAH_COUNT_OFF = 20,
      DEFAULT_THREAH_COUNT_ON = 30,
    };
  public:
  PhoneRPCCountThread() :
    count_off_(0), thresh_count_off_(DEFAULT_THREAH_COUNT_OFF),
      count_on_(0), thresh_count_on_(DEFAULT_THREAH_COUNT_ON)
      {
      start(65535).detach();
    }
    virtual void run() {
      MCE_INFO("PhoneRPCCountThread::run --> start");
      for (;;) {
        if (count_on_ > thresh_count_on_ || count_off_ > thresh_count_off_) {
          MCE_INFO("PhoneRPCCountThread::run --> clear count_on " << count_on_ 
                   << " count_off " << count_off_);
        }
        count_on_ = 0;
        count_off_ = 0;
        usleep(100000);
      }
    }
    int thresh_count_on() const { return thresh_count_on_; }
    int thresh_count_off() const { return thresh_count_off_; }
    void set_thresh_count_on(int limit) { thresh_count_on_ = limit; }
    void set_thresh_count_off(int limit) { thresh_count_off_ = limit; }
    //const volatile int &count_on() const { return count_on_; }
    void upcount_on() { ++count_on_; }

    //const volatile int &count_off() const { return count_off_; }
    void upcount_off() { ++count_off_; }

    bool can_online() { return count_on_ > thresh_count_on_ ? false : true; }
    bool can_offline() { return count_off_ > thresh_count_off_ ? false : true; }

};

	
class IMGateForPhoneManagerI : public IMGateForPhoneManager, virtual  public Singleton<IMGateForPhoneManagerI>{
public:
	void SendMessage(int phoneuserid, int touserid, const string& message, const Ice::Current& = Ice::Current());
  void SendMessageWithMsgkey(int phoneuserid, int touserid, long msgkey, const string& message, const Ice::Current& = Ice::Current());
	void PhoneUserOnline(int phoneuserid, const Ice::Current& = Ice::Current());
	void PhoneTimerOnline(const IntSeq &seq, const Ice::Current& = Ice::Current());
	void HeartBeat(const IntSeq &seq, const Ice::Current& = Ice::Current());
	void PhoneUserOffline(int phoneuserid, const Ice::Current& = Ice::Current());
	void NotifyMessageSended(int userid, Ice::Long msgKey, const Ice::Current& = Ice::Current());
	int getOfflineMessageCount(int userid, const Ice::Current& = Ice::Current());

  void CallOnline(int phoneuserid);
  void CallOffline(int phoneuserid);
	void ServerDisable(const Ice::Current& = Ice::Current());
	void ServerEnable(const Ice::Current& = Ice::Current());

  MyUtil::IntSeq getCallLimit(const Ice::Current& = Ice::Current()) {
    IntSeq rv;
    rv.push_back(PhoneRPCCountThread::instance().thresh_count_on());
    rv.push_back(PhoneRPCCountThread::instance().thresh_count_off());
    return rv;
  }

  void setOnlineBuffTimeout(int tm, const Ice::Current& = Ice::Current()) {
    PhoneOnlineCheckThread::instance().set_timeout(tm);
  }
  
  void setCallLimit(int online, int offline, const Ice::Current& = Ice::Current()) {

    PhoneRPCCountThread::instance().set_thresh_count_on(online);
    PhoneRPCCountThread::instance().set_thresh_count_off(offline);
    MCE_WARN("IMGateForPhoneManagerI::setCallLimit on " << PhoneRPCCountThread::instance().thresh_count_on()
             << " off " << PhoneRPCCountThread::instance().thresh_count_off());

  }

	
private:
	bool _serverenable;
	IceUtil::RWRecMutex _rwmutex;

};


};
};
};
};

#endif 
