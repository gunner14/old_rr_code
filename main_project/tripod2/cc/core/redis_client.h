#ifndef REDIS_CLIENT_
#define REDIS_CLIENT_

#include "tripod2/cc/core/cache_client.h"
#include <hiredis.h>

namespace xce {
namespace tripod {

const static int kReplyMiss = 0;
const static int kReplySucess = 1;
const static int kReplyConnectionError = 2;

const static std::string kRedisStringType = "string";
const static std::string kRedisListType = "list";
const static std::string kRedisHashType = "hash";
const static std::string kRedisZsetType = "zset";
const static std::string kRedisNoneType = "none";

class RedisClient : public CacheClient {
 public:
  RedisClient(const std::string& address, int port, int timeout);
  virtual ~RedisClient();

  bool IsValid() {
    return valid_ == true;
  }
  void set_valid(bool v) {
    valid_ = v;
  }

  virtual bool Get(const std::string& key, std::string* value);
  virtual bool Set(const std::string& key, const std::string& value, int secs = 0);
  virtual std::map<std::string, bool> MultiGet(const StringListType& keys,StringMapType* values);
  virtual bool GetList(const std::string& key, StringListType* value, int begin = 0, int limit = 0);
  virtual bool SetList(const std::string& key, const StringListType& list_value, int secs = 0);
  virtual bool GetHash(const std::string& key, const StringListType& fields, StringMapType* hash_value);

  virtual std::string Inc(const std::string& key, const std::string& step);
  virtual int Ttl(const std::string& key);

  //add the only field function!!!
  virtual bool HMSet(const std::string& key, const StringMapType& hash_value);
  virtual bool HGetAll(const std::string& key, StringMapType* hash_value);  
  virtual bool HDel(const std::string& key, const StringListType& fields);
  virtual bool HIncrBy(const std::string& key, const std::string& field, const std::string& step);////////////////

  virtual bool SetHash(const std::string& key, const StringMapType& hash_value, int secs = 0);
  virtual int  Exists(const std::string& key);
  virtual bool Remove(const std::string& key);
  virtual int  Size(const std::string& key);
  virtual std::string Ping();

  virtual bool ZSet(const std::string& key, const StringMapType& set_with_scores, int secs = 0);
  virtual bool ZAdd(const std::string& key, const StringMapType& set_with_scores);
  virtual bool ZRemRangeByRank(const std::string& key, int begin, int end);
  virtual bool ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max);
  virtual bool ZRange(const std::string& key, int begin, int limit, StringListType* zset_value);
 private:
  static std::string GeneratorTempKey(const std::string& cache_key);
  bool Rename(const std::string& old_name, const std::string& new_name);

 public:
  bool Expire(const std::string& key, int secs);
  virtual std::string Type(const std::string& key);

 private:
  static const int kWriteBatchSize;
  static const int kReadBatchSize;
  std::string address_;
  int port_;
  int timeout_; //millisecond
  redisContext* conn_;
  bool valid_;
};

class RedisClientPool : public CacheClientPool {
 public:
  RedisClientPool(const std::string address, int port, int timeout, int core_size = 10, int max_size = 100);
  virtual ~RedisClientPool();
  CacheClient* Create();
};

class CacheClientPoolFactory {
 public:
  static CacheClientPoolPtr GetPool(const std::string& address, int port, int conn_time_out);
 private:
  static std::map<std::pair<std::string, int>, CacheClientPoolPtr> pool_map_;
  static boost::mutex mutex_;
};

}} // xce::tripod
#endif

