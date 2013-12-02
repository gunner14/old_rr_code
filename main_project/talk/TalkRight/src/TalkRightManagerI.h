#ifndef TALKRIGHTMANAGERI_H_
#define TALKRIGHTMANAGERI_H_

#include "TalkRight.h"
#include <Ice/Ice.h>
#include "ServiceI.h"
#include "LogWrapper.h"
#include "Singleton.h"
#include "BuddyCoreAdapter.h"
#include <set>
#include <IceUtil/RWRecMutex.h>

#include "TalkProxy.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>

using boost::multi_index_container;
using namespace boost::multi_index;

namespace com {
namespace xiaonei {
namespace talk {

const std::string WHITE_LIST_FILE = "/data/xce/XNTalk/etc/TalkRightWhiteList.conf";

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::service::buddy;
using namespace com::xiaonei::talk::common;

template<class T> 
class BlockSet : public Ice::Object {
	set<T> _set;
public:
	void add(T& blockId) {
		_set.insert(blockId);
	}
	;
	void remove(T& blockId){
		_set.erase(blockId);
	};
	bool has(T& blockId) {
		typename set<T>::iterator it;
		it = _set.find(blockId);
		return it != _set.end() ? true : false;
	}
	;
	vector<T> getSeq()
	{
		vector<T> seq;
		for(typename set<T>::iterator it = _set.begin(); it != _set.end(); ++it){
			seq.push_back(*it);
		}
		return seq;
	};
};
typedef IceUtil::Handle< BlockSet<int> > UserBlockSetPtr;
typedef IceUtil::Handle< BlockSet<string> > SysMsgBlockSetPtr;

class HiddenSet : public Ice::Object
{
	set<int> _set;
public:
	void add(int userId) {
		_set.insert(userId);
	};
	void remove(int userId) {
		_set.erase(userId);
	};
	bool has(int userId) {
		set<int>::iterator it = _set.find(userId);
		return it != _set.end() ? true:false;
	};
};

typedef IceUtil::Handle<HiddenSet> HiddenSetPtr;


class UserBlockSetFactoryI : public ServantFactory {
public:
	virtual Ice::ObjectPtr create(Ice::Int id);
	virtual ObjectResultPtr create(const MyUtil::IntSeq& ids);
};

class SysMsgBlockSetFactoryI : public ServantFactory {
public:
	virtual Ice::ObjectPtr create(Ice::Int id);
	virtual ObjectResultPtr create(const MyUtil::IntSeq& ids);
};

class UserStatusFactoryI : public ServantFactory{
public:
	virtual Ice::ObjectPtr create(Ice::Int id);
	virtual ObjectResultPtr create(const MyUtil::IntSeq& ids);	
};

class TimeStat{
	timeval _begin, _end;
	public:
	TimeStat(){
		reset();
	}
	void reset(){
		gettimeofday(&_begin, NULL);
	}
	float getTime(){
		gettimeofday(&_end, NULL);
		float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
			-_begin.tv_usec;
		timeuse/=1000;
		return timeuse;
	}
};

class RecentValidStranterCache {
  class t_idpair {
    string users_; // from 在前 to在后
    time_t stamp_;
  public:
    string users() const {return users_;}
    time_t stamp() const {return stamp_;}
    void set_stamp(time_t st) {stamp_ = st;}
  t_idpair(const string &users) : users_(users), stamp_(time(NULL)) {
      
    }
  };

  typedef boost::shared_ptr<t_idpair>  t_idpairPtr;
  struct tag_seq {};
  struct tag_users {};
  typedef multi_index_container<
    t_idpairPtr,
    indexed_by<
			sequenced< tag<tag_seq> >,
    	hashed_unique< tag<tag_users>,
    		BOOST_MULTI_INDEX_CONST_MEM_FUN(t_idpair, string, users)
    	>
    >
  > t_cache;
  typedef t_cache::index<tag_seq>::type ListIndex;
  typedef t_cache::index<tag_users>::type SetIndex;
  
  enum {
    DEFAULT_TIME_OUT = 1 * 60 * 60, // 单位 s 
    DEFAULT_CACHE_SIZE = 10000, // 缓存最大个数
  };

  IceUtil::Mutex mutex_;
  t_cache cache_;
  int cache_size_;
  int timeout_; // 过期时间值


 public:
 RecentValidStranterCache() : cache_size_(DEFAULT_CACHE_SIZE), timeout_(DEFAULT_TIME_OUT) {}
  void update(int from, int to); // 更新cache
  bool lookup(int from, int to); // 查询cache，如果命中则返回真，且把命中的变为最热
  static int get_small_id(int from, int to) {
    if (from > to) {
      return to;
    } else {
      return from;
    }
  }
  static void get_key(int from, int to, string &key) {
    char buff[100] = {0};
    int id0 = from;
    int id1 = to;
    if (from > to) {
      id0 = to;
      id1 = from;
    }
    
    sprintf(buff, "%d:%d", id0, id1);
    key = buff;
    //MCE_INFO("RecentValidStranterCache::get_key--> " << key);
  }

  int cache_size() { return cache_size_; }
  void set_cache_size(int cache_size) { cache_size_ = cache_size; }
  int time_out() { return timeout_; }
  void set_time_out(int time_out) { timeout_ = time_out; }
  
};

class RightManagerI : public virtual RightManager,
 public	MyUtil::Singleton<RightManagerI> {

  
	HiddenSetPtr _hiddenUsers;
  enum {
    RECENT_CACHE_POOL_SIZE = 231,
  };
  RecentValidStranterCache _verifycache[RECENT_CACHE_POOL_SIZE];
public:
	RightManagerI(){
		_hiddenUsers = new HiddenSet();
	}
	//virtual bool verify(Ice::Int from, Ice::Int to , Ice::Int type, const Ice::Current& = Ice::Current());
	virtual MyUtil::IntSeq batchVerify(const VerifySeq& seq, const Ice::Current& = Ice::Current());
  virtual int MsgVerify(const VerifyMsgPtr &vmsg, const Ice::Current& = Ice::Current());
	virtual bool isHiddenUser(Ice::Int userId,const Ice::Current& = Ice::Current());
	virtual void addHiddenUser(Ice::Int userId,const Ice::Current& = Ice::Current());
	virtual void removeHiddenUser(Ice::Int userId,const Ice::Current& = Ice::Current());
	virtual void reloadBlockUser(Ice::Int userId,const Ice::Current& = Ice::Current());
	
	virtual void modifySystemMessageBlock(Ice::Int userId, const MyUtil::StrSeq& typeSeq,const Ice::Current& = Ice::Current());
	//virtual void removeSystemMessageBlock(Ice::Int userId, const string& type,const Ice::Current& = Ice::Current());
	virtual MyUtil::StrSeq getSystemMessageBlock(Ice::Int userId,const Ice::Current& = Ice::Current());
	virtual UserStatusPtr getUserStatus(Ice::Int userId,const Ice::Current& = Ice::Current());
	virtual void reloadUserStatus(Ice::Int userId, const Ice::Current& = Ice::Current());
  virtual IntSeq getRecentBufferSet(const Ice::Current& = Ice::Current());
  virtual void setRecentBufferSet(int cachesize, int timeout, const Ice::Current& = Ice::Current());

  virtual void ReloadWhiteList(const Ice::Current& = Ice::Current());

	virtual void kickDesk(Ice::Int userId, const Ice::Current& = Ice::Current());

 private:
  bool BuddyRelationVerify(BuddyDesc &bd, int from, int to);
  bool UserBlockVerify(int from, int to);
  bool SpamFilterVerify(int from, const string &msg);
  bool SpamActivityVerify(int from, int to, const string &msg);
  // 白名单功能 by jian.wang3 2012-09-14
  bool InWhiteList(int uid);

  ProxyManagerPrx getProxyServerOneway(const JidPtr& ptr);
  set<int> white_list_;
  IceUtil::RWRecMutex white_list_mutex_;

	IceUtil::Mutex _mutex_prx;
	map<string, ProxyManagerPrx> _managerOneway;
  
};



};
};
}
;

#endif /*TALKRIGHTMANAGERI_H_*/
