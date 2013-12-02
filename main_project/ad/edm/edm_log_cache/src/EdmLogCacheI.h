#ifndef __EDM_LOG_CACHEI_H__
#define __EDM_LOG_CACHEI_H__

#include <Ice/Ice.h>
#include <IceUtil/Mutex.h>
#include <list>
#include <map>
#include <string>
#include "TaskManager.h"
#include "Singleton.h"
#include "EdmLogCache.h"
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <boost/shared_ptr.hpp>
#include "Notify/util/fcgi_request.h"
namespace xce {
namespace ad {
using namespace boost;
using namespace std;
using namespace xce::notify;

typedef shared_ptr<map<string, string> > StrMapPtr;

/*
 * Entry中存储1天的log数据
 * @expire_time_ 是当天的日期，以当日凌晨3点
 * @StrMapPtr 以map结构存储log数据，key值为userid_adgroupid，value值为log数据 
 */
struct Entry {
public:
  Entry() : log_ptr_(new map<string, string>()){}

  time_t GetTime() { return expire_time_; }
  void SetTime(const time_t& time) { expire_time_ = time; } 

  string GetLog(const string& key) {
    map<string, string>::iterator s_iter = log_ptr_->find(key);
    if (log_ptr_->end() == s_iter){
      return "";
    }
    return (*s_iter).second;
  }

  void InsertLog(const string& key, const string& log) {
    MCE_DEBUG("Entry::InsertLog --> after size: " << log_ptr_->size());
    log_ptr_->insert(make_pair<string, string>(key, log));
    MCE_DEBUG("Entry::InsertLog --> before size: " << log_ptr_->size());
  }
  
  std::string FindLog(const string& key) {
    map<string, string>::iterator s_iter = log_ptr_->find(key);
    if (log_ptr_->end() == s_iter) {
      return "";
    } else {
      return (*log_ptr_)[key];
    }
  }

private:
  time_t expire_time_;
  StrMapPtr log_ptr_;
};


class EdmLogCacheI : public EdmLogCache, public MyUtil::Singleton<EdmLogCacheI> {
public:
  EdmLogCacheI();
  virtual void Add(const string& key, const string& value, const Ice::Current& = Ice::Current());
  virtual void Expose(const string& key, Ice::Int index, const Ice::Current& = Ice::Current());
  virtual void Click(const string& key, const Ice::Current& = Ice::Current());

  bool IsEvict();
  void Evict();
  void EvictFromDB();

  void UpdateLastClearTime();
  
protected:
  void AddInCache(const string& key, const string& value, const time_t& time);
  string Get(const string& key);

  time_t GetExpireTime(time_t expire_time, const time_t days);

private:
  IceUtil::RWRecMutex log_mutex_;
  list<Entry> log_cache_;

  time_t last_clear_time_;
};

class EvictTask : virtual public MyUtil::Task {
public:
  EvictTask() {}
  virtual void handle();
};

class Process {
  public:
    static int ProcessLogStr(string& source, string& edm_log, string& left_click_log, string& left_cpc_charge);
    static void ProcessLeftClickLog(string& log);
    static void ProcessLeftChargeLog(string& log);
};
}//end of namespace ad 
}//end of namespace xce

#endif
