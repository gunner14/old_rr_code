#ifndef CACHE_PROXY_
#define CACHE_PROXY_

#include <map>

#include "base/consistenthash.h"

#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

#include "tripod2/cc/core/cache_client.h"
#include "tripod2/cc/core/redis_client.h"
#include "tripod2/cc/core/zk_client.h"


namespace xce {
namespace tripod {

class CacheProxy : public ZNodeListener, public CacheClient {
 public:
  CacheProxy(const std::string& ns, const std::string& biz, int conn_time_out);
  virtual ~CacheProxy();

  virtual bool Update(const std::list<std::string>& children_names);

  virtual bool Get(const std::string& key, std::string* value);
  virtual bool Set(const std::string& key, const std::string& value, int secs = 0);

  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values);

  virtual std::string Inc(const std::string& key, const std::string& step);

  virtual bool GetList(const std::string& key, StringListType* value, int begin = 0, int limit = 0);
  virtual bool SetList(const std::string& key, const StringListType& list_value, int secs = 0);

  virtual bool GetHash(const std::string& key, const StringListType& fields, StringMapType* hash_value);
  virtual bool SetHash(const std::string& key, const StringMapType& hash_value, int secs = 0);
  virtual bool HMSet(const std::string& key, const StringMapType& hash_value);  
  virtual bool HGetAll(const std::string& key, StringMapType* hash_value);
  virtual bool HDel(const std::string& key, const StringListType& fields);
  virtual bool HIncrBy(const std::string& key, const std::string& field, const std::string& step);

  //attion : this three operations is for the set-get mode, haven`t use producer need to add other operators for the total pm-cm-cc mode!! 
  virtual bool ZAdd(const std::string& key, const StringMapType& set_with_scores);
  virtual bool ZSet(const std::string& key, const StringMapType& set_with_scores, int secs = 0);
  virtual bool ZRemRangeByRank(const std::string& key, int begin = 0, int end = 0);
  virtual bool ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max);
  virtual bool ZRange(const std::string& key, StringListType* zset_value, int begin = 0, int limit = 0); 
   
  virtual int  Exists(const std::string& key);
  virtual bool Remove(const std::string& key);
  virtual int  Size(const std::string& key);

  bool IsValid() {
    return valid_ == true;
  }
  std::string namespace_id() const {
    return namespace_id_;
  }
  std::string biz_id() const {
    return biz_id_;
  }

  static std::string GenerateCacheKey(const std::string& prefix, const std::string& key) {
    const static std::string separator = ":";
    std::ostringstream otem;
    otem << prefix << separator << key;
    return otem.str();
  }
 protected:
  CacheClientPoolPtr LocateCacheClientPool(const std::string&);
 private:
  void set_valid(bool v) {
    valid_ = v;
  }
 private:
  std::string namespace_id_;
  std::string biz_id_;
  int conn_time_out_;

  bool valid_;

  ContinuumPtr continuum_;
  boost::shared_mutex continuum_mutex_;
  typedef std::map<std::string, CacheClientPoolPtr> CacheClientPoolMapType; // key:nickname,  value: pool
  CacheClientPoolMapType pool_map_;
};

}} // xce::tripod
#endif

