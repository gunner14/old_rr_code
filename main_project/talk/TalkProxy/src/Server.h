#ifndef __SERVER_H__
#define __SERVER_H__

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Thread.h>
#include <Ice/Ice.h>
#include "Session.h"
#include "Singleton.h"
#include "TalkCommon.h"
#include "ServiceI.h"
#include "Refactor.h"
#include "Connection.h"
#include "TalkProxy.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>


void changeDomain(string& data, const string& src, const string& dst);

namespace com {
namespace xiaonei {
namespace talk {

using boost::multi_index_container;
using namespace boost::multi_index;

using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace std;



struct less_jid : public binary_function<JidPtr, JidPtr, bool> {
	bool operator()(const JidPtr& x, const JidPtr& y) {
		if (x->userId < y->userId) {
			return true;
		} else if (x->userId == y->userId) {
			return x->index < y->index;
		} else {
			return false;
		}
	}
};
typedef map<JidPtr, SessionPtr, less_jid> SessionMap;
typedef set<SessionPtr, less_jid> SessionSet;
//----------------------------------------------------------------------
class t_session_size : public IceUtil::Shared {
  SessionPtr session_;
  time_t stamp_;
};

class SessionHeartBeat {
  // 增加心跳逻辑-----------------
  class t_session : public IceUtil::Shared {
    SessionPtr session_;
    time_t stamp_;
  public:
  t_session(const SessionPtr &s, const time_t t) : session_(s), stamp_(t) {

    }
    void change_stamp(const time_t t) {
      stamp_ = t;
    }
    SessionPtr session() const {
      return session_;
    }
    time_t stamp() const {
      return stamp_;
    }
    Ice::Long cid() {
      return (0 == session_ || 0 == session_->jid()) ? 0 : session_->jid()->index;
    }
  };

  typedef IceUtil::Handle<t_session> t_sessionPtr;

  struct list_tag {};
  struct sid_tag {};
  struct seq_tag {};
  typedef multi_index_container<
    t_sessionPtr,
    indexed_by<
	    sequenced< tag<list_tag> >,
	    hashed_unique< tag<sid_tag>, BOOST_MULTI_INDEX_MEM_FUN(t_session, Ice::Long, cid) >
    >
    > t_session_collects;
  typedef t_session_collects::index<list_tag>::type ListIndex;
  typedef t_session_collects::index<sid_tag>::type SidIndex;


	IceUtil::Mutex mutex_;
  t_session_collects session_collects;




 public:
 SessionHeartBeat() {

  }
  size_t count();
  void insert(const SessionPtr &s);
  void del(const SessionPtr &s);
  bool get_old_sessions(vector<SessionPtr> &sessions, const int one_heart_inv, const int max_num);
};

class SessionHeartBeatThread : virtual public IceUtil::Thread {
 public:
  virtual void run();
};

typedef IceUtil::Handle<SessionHeartBeatThread> SessionHeartBeatThreadPtr;

class SessionHeartBeatPool : 
  public Singleton<SessionHeartBeatPool> {
  enum {
    POOL_SIZE = 500,
  };

  // 一系列的默认值
  enum {
    ONE_HEART_BEAT_INV = 100, // 心跳时间间隔
    GET_MORE_INTV_FLOOR = ONE_HEART_BEAT_INV/2,

    MAX_INTERV = 10 * 1000000, // 10s
    MIN_INTERV = 100 * 1000, // 100ms

    MAX_HEARTBEAT_USERS = 1000,
    // 一次检测最多取多少用户，默认最大取够10次rpc online调用的用户数目
    CEIL_ONG_GETUSERS = MAX_HEARTBEAT_USERS * 5,



    // 常量
    ONE_CHG_HEART_INTV = MIN_INTERV, // 一次动态微调心跳检测等待间隔时间
    ZOOM_INTV_THRESH_SCOPE = 5, // 调大心跳检测间隔值时候使用
    ZOOM_INTV_THRESH_TIME = 4, // 调小心跳检测间隔值时候使用

    INTV_CALLONLINE = 100 * 1000, // 100ms 每次调用funstate等待时间间隔
  };


  SessionHeartBeat pool_[POOL_SIZE];

	IceUtil::Mutex mutex_thread_run_;
  SessionHeartBeatThreadPtr thread_;
  bool flag_run_;
  int interv_; //一次循环usleep的时间

  // 可被外部接口改变的变量
  bool run_thread_;
  // config
  int one_beat_intv_;
  int max_intv_;
  int min_intv_;

  int max_heart_users_;

  int ceil_one_get_;
 
  int get_more_floor_intv_;


 private:
  void reset_runflag();
 public:
 SessionHeartBeatPool() 
    : flag_run_(false), 
    interv_((MAX_INTERV+MIN_INTERV)/2),

    run_thread_(true),
    one_beat_intv_(ONE_HEART_BEAT_INV),
    max_intv_(MAX_INTERV),
    min_intv_(MIN_INTERV),
    max_heart_users_(MAX_HEARTBEAT_USERS),
    ceil_one_get_(CEIL_ONG_GETUSERS),
    get_more_floor_intv_(GET_MORE_INTV_FLOOR)
      {
        start_thread();
      }

  int one_beat_intv() const { return one_beat_intv_; }
  int max_intv() const { return max_intv_; }
  size_t pool_count();
  void insert(const SessionPtr &s);
  void del(const SessionPtr &s);
  int get_old_sessions(vector<SessionPtr> &sessions,  const int scan_begin_idx);
  
  void run();
  void do_heart(const vector<SessionPtr> &sessions, int cn);
  void start_thread();
  void stop_thread();

  HeartBeatConfPtr get_thread_conf();
  void set_heartbeat_conf(const com::xiaonei::talk::HeartBeatConfPtr &conf);
};

//----------------------------------------------------------------------
class Server : public Singleton<Server> {
public:
	Server() :
		_joinCount(0), _onlineUserCount(0) {
	}
	;
	void start(const Ice::PropertiesPtr& props);
	void stop();

	//	void apply(const SessionPtr& session);
	//	void join(const SessionPtr& session);
	//	void leave(const SessionPtr& session);
	//	void leave(const JidPtr& jid);

	bool deliver(const JidPtr& jid, const string& message);
	bool verify(const JidPtr& jid, const string& ticket);
	string GetTicket(const JidPtr& jid);

	void join() {
		IceUtil::Mutex::Lock lock(_mutex);
		++_joinCount;
	}
	void leave() {
		IceUtil::Mutex::Lock lock(_mutex);
		--_joinCount;
	}
	int getJoinCount(){
		return _joinCount;
	}
	void bind() {
		IceUtil::Mutex::Lock lock(_mutex);
		++_onlineUserCount;
	}
	void unbind() {
		IceUtil::Mutex::Lock lock(_mutex);
		--_onlineUserCount;
	}
	int getOnlineUserCount(){
		return _onlineUserCount;
	}
	int size(bool apply = false);
	bool indexExist(Ice::Long index);
	//void sendLog(const JidPtr& from, const JidPtr& to, const string& msg);
	bool isRenren(){
		return _renren;
	}
	void kick(const JidPtr& jid);


private:

	RefactorPtr _refactor;
	IceUtil::Mutex _mutex;
	//	set<SessionPtr> _applies;
	//	SessionMap _sessions; // userid-resourceid -> sessionPtr;
	//	SessionSet _sessions;
	int _joinCount, _onlineUserCount;
	bool _renren;

};


typedef IceUtil::Handle<Server> ServerPtr;

}
;
}
;
}
;
#endif

