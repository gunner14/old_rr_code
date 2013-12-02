#ifndef BUCKET_LOCK_H_
#define BUCKET_LOCK_H_

#include <string>
#include <map>
#include <vector>
#include "boost/thread.hpp"
#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "base/hash.h"

namespace xce {
namespace tripod {

//
// 桶锁
// 解决： 对一个key,多个线程顺序访问，key可能由外部输入的参数
//
class BucketMutex {
 public:
  explicit BucketMutex(uint32 size = 100);
  virtual ~BucketMutex();
  void Lock(const std::string& key);
  void Unlock(const std::string& key);
 private:
  typedef std::map<std::string, int> FlagMapType;
  struct Bucket {
    FlagMapType flag_map_;
    boost::condition_variable bucket_cond_;
    boost::mutex mutex_;
  };
  
  static uint32 Hash(const std::string& text) {
    return ::xce::MurmurHash2(text.c_str(), text.size(), 0);
  } 

  Bucket* LocateBucket(const std::string& text);
 private:
  uint32 bucket_size_;
  std::vector<Bucket*> buckets_;
};

typedef boost::shared_ptr<BucketMutex> BucketMutexPtr;

//为了异常安全性和使用方便，析构函数释放锁
class BucketLock : private boost::noncopyable {
 public:
  BucketLock(const std::string& key, BucketMutexPtr mutex) : key_(key),mutex_(mutex) {
    mutex_->Lock(key_);
  }
  ~BucketLock() {
    mutex_->Unlock(key_);
  }
 private:
  const std::string& key_;
  boost::shared_ptr<BucketMutex> mutex_;
};

}} // xce::tripod
#endif


