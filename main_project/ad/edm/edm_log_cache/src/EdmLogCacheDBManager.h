#ifndef __EDM_LOG_CACHE_DB_MANAGER_H__
#define __EDM_LOG_CACHE_DB_MANAGER_H__

#include <Ice/Ice.h>
#include <list>
#include <map>
#include <string>
#include "TaskManager.h"
#include "Singleton.h"
#include "EdmLogCache.h"
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <boost/shared_ptr.hpp>
#include <IceUtil/Monitor.h>
#include <IceUtil/Thread.h>
#include <QueryRunner.h>
#include <DbDescriptor.h>
#include <connection.h>
#include <query.h>

namespace xce {
namespace ad {

using namespace boost;
using namespace std;

struct DBEntry {   //DB cache中每条log的格式
public:
  DBEntry(const string& key, const string& value, const time_t& time) : key_(key), value_(value), expire_time_(time) {}  

  string GetValue() { return value_; }
  void SetValue(const string& value) { value_ = value; }

  string GetKey() { return key_; }
  void SetKey(const string& key) { key_ = key; }
 
  time_t GetTime() { return expire_time_; }
  void SetTime(const time_t& time) { expire_time_ = time; }

private:
  string key_;
  string value_;
  time_t expire_time_;
};

typedef shared_ptr<DBEntry> DBEntryPtr;

class DBEntryQueue : public IceUtil::Monitor<IceUtil::Mutex> {
public:
  void Put(DBEntryPtr entry);
  std::vector<DBEntryPtr > Get();
private:
  list<DBEntryPtr > entry_queue_;
};

class EdmLogCacheDBManager : public ::MyUtil::Singleton<EdmLogCacheDBManager> {
public:
  EdmLogCacheDBManager() : db_inserter_(entry_queue_, db_instance_) {}

  void Init();

  void SetDB(const string& db_instance ) { db_instance_ = db_instance; } ;

  void Insert(DBEntryPtr entry);
  std::string GetFromDB(const string& key);
  void EvictFromDB();

  class EdmLogDBInserter : public IceUtil::Thread {
  public:
    EdmLogDBInserter(DBEntryQueue& queue, std::string& db_instance) : queue_(queue), db_(db_instance){}
    virtual void run();   //线程体
  protected:
    com::xiaonei::xce::Statement GetInsertSql(const std::vector<DBEntryPtr >& entry_vec);
  private:
    void InsertIntoDB(com::xiaonei::xce::Statement& sql);  //替代原InsertIntoDB接口，供Add接口调用
    DBEntryQueue& queue_;
    std::string& db_;
  };//end of class EdmLogDBInserter 

private:
  DBEntryQueue entry_queue_;
  std::string db_instance_;
  EdmLogDBInserter db_inserter_;
};//end of class EdmLogCacheDBManager

}//end of namespace ad 
}//end of namespace xce
#endif
