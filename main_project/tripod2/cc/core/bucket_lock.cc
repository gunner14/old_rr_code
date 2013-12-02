#include "tripod2/cc/core/bucket_lock.h"

#include "boost/foreach.hpp"

namespace xce {
namespace tripod {

BucketMutex::BucketMutex(uint32 size) : bucket_size_(size) {
  for (uint32 i = 0; i < size; ++i) {
    buckets_.push_back(new Bucket());
  }
}

BucketMutex::~BucketMutex() {
  BOOST_FOREACH(Bucket* bucket, buckets_) {
    if (bucket) {
      delete bucket;
    }
  }
  buckets_.clear();
}

BucketMutex::Bucket* BucketMutex::LocateBucket(const std::string& text) {
  return buckets_[Hash(text) % bucket_size_];
}

void BucketMutex::Lock(const std::string& key) {
  Bucket* bucket = LocateBucket(key);
  if (!bucket) {
    return;
  }
  while (true) {
    boost::unique_lock<boost::mutex> lock(bucket->mutex_);
    FlagMapType::const_iterator iter = bucket->flag_map_.find(key);
    if (iter != bucket->flag_map_.end()) {
      if (iter->second == 1) {
        bucket->bucket_cond_.wait(lock);
        continue;
      } else {
        bucket->flag_map_[key] = 1;
        break;
      }
    } else {
      bucket->flag_map_[key] = 1;
      break;
    }
  }
}

void BucketMutex::Unlock(const std::string& key) {
  Bucket* bucket = LocateBucket(key);
  if (!bucket) {
    return;
  }
  boost::unique_lock<boost::mutex> lock(bucket->mutex_);
  FlagMapType::iterator iter = bucket->flag_map_.find(key);
  if (iter != bucket->flag_map_.end()) {
    bucket->flag_map_.erase(iter);
  }
  bucket->bucket_cond_.notify_all();
}


}} // xce::tripod



