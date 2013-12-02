#ifndef _TRIPOD_REDIS_CLIENT_POOL_H__
#define _TRIPOD_REDIS_CLIENT_POOL_H__

#include <deque>
#include <map>
#include <boost/thread/mutex.hpp>

#include <Common.h>
#include <hiredis.h>
#include <TripodData.h>

namespace com {
namespace renren {
namespace tripod {

const static int kReplyMiss = 0;
const static int kReplySucess = 1;
const static int kReplyConnectionError = 2;

class RedisClient {
 public:
  RedisClient(const std::string& address, int port, int timeout = 1000);
  virtual ~RedisClient();

  bool IsValid() {
    return valid_ == true;
  }
  void set_valid(bool v) {
    valid_ = v;
  }

  bool GetList(const std::string& cache_key, StrList& value, int begin = 0, int limit = 0);
  bool SetList(const std::string& cache_key, const StrList& list);
  int GetListSize(const std::string& cache_key);
  bool Remove(const std::string& cache_key);
  bool GetHash(const std::string& cache_key, const StrList& fields, HashPair& value);
  bool SetHash(const std::string& cache_key, const HashPair& hash_pair);
  int Exists(const std::string& cache_key);

 private:
  static std::string GeneratorTempKey(const std::string cache_key);
  bool Rename(const std::string& old_name, const std::string& new_name);

 private:
  static const int kWriteBatchSize;
  static const int kReadBatchSize;
  std::string address_;
  int port_;
  int timeout_; //millisecond
  redisContext* conn_;
  bool valid_;
};

class RedisClientPool {
 public:
  RedisClientPool(const std::string& address, int port, size_t core_size = 10, size_t max_size = 100);
  virtual ~RedisClientPool();
 
  RedisClient* BorrowItem();
  void ReturnItem(RedisClient*);
  RedisClient* Create();
  void Destroy(RedisClient*);
  std::string Dump();
 private:
  std::string address_;
  int port_;
 
  size_t core_size_;
  size_t max_size_;
 
  boost::mutex un_used_mutex_;
  std::deque<RedisClient*> un_used_;
  size_t used_;
};

}}} // end com::renren::tripod
#endif

