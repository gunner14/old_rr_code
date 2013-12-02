#ifndef CACHE_CLIENT_
#define CACHE_CLIENT_

#include <string>

#include "boost/thread/mutex.hpp"
#include "boost/shared_ptr.hpp"
#include "tripod2/cc/core/common.h"

namespace xce {
namespace tripod {

class CacheClientPool;
typedef boost::shared_ptr<CacheClientPool> CacheClientPoolPtr;

class CacheClient {
 public:
  virtual bool Get(const std::string& key, std::string* value) = 0;
  // argc secs is the expire time from now
  // if secs == 0, no expire!
  virtual bool Set(const std::string& key, const std::string& value, int secs = 0) = 0;
  //成功返回结果字符串，失败返回空字符串
  virtual std::string Inc(const std::string& key, const std::string& step);
  virtual std::map<std::string, bool> MultiGet(const StringListType& keys,StringMapType* values);
  //change by jipeng.lin
  virtual bool GetList(const std::string& key, StringListType* value, int begin = 0, int limit = 0);
  virtual bool SetList(const std::string& key, const StringListType& list_value, int secs = 0);

  virtual bool GetHash(const std::string& key, const StringListType& fields, StringMapType* hash_value);
  virtual bool HMSet(const std::string& key, const StringMapType& hash_value);
  virtual bool HGetAll(const std::string& key, StringMapType* hash_value);
  virtual bool HDel(const std::string& key, const StringListType& fields);
  virtual bool HIncrBy(const std::string& key, const std::string& field, const std::string& step);
  virtual bool SetHash(const std::string& key, const StringMapType& hash_value, int secs = 0);
  virtual int  Exists(const std::string& key);
  virtual bool Remove(const std::string& key);
  virtual int  Size(const std::string& key);
  virtual bool IsValid();
  virtual std::string Type(const std::string& key);
  virtual int Ttl(const std::string& key);
  // for monitor
  virtual std::string Ping();
  virtual ~CacheClient(){}

  //add Zset operations
  virtual bool ZAdd(const std::string& key, const StringMapType& set_with_scores);
  virtual bool ZSet(const std::string& key, const StringMapType& set_with_scores, int secs = 0);
  virtual bool ZRemRangeByRank(const std::string& key, int begin, int end);
  virtual bool ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max);
  virtual bool ZRange(const std::string& key, int begin, int limit, StringListType* zset_value);

};

class CacheClientPool {
 public:
  CacheClientPool(const std::string address, int port, int timeout, int core_size = 10, int max_size = 100);
  virtual ~CacheClientPool();
  virtual CacheClient* Create() = 0;
  void Destroy(CacheClient*);
  CacheClient* BorrowItem();
  void ReturnItem(CacheClient*);
  std::string Dump();

  std::string address() const {
    return address_;
  }
  int port() const {
    return port_;
  }
  int timeout() const {
    return timeout_;
  }
  int id() const {
    return id_;
  }
 private:
  static int count_;
  int id_;
  std::string address_;
  int port_;
  int timeout_; // millisecond

  size_t core_size_;
  size_t max_size_;
  
  boost::mutex un_used_mutex_;
  std::deque<CacheClient*> un_used_;
  size_t used_;
};

}} // xce::tripod
#endif

