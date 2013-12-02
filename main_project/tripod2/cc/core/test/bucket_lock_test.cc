#include "gtest/gtest.h"

#include <string>
#include <sstream>

#include "boost/shared_ptr.hpp"

#include "base/logging.h"
#include "base/ptime.h"
#include "base/port.h"
#include "base/threadpool.h"

#include "tripod2/cc/core/bucket_lock.h"

namespace test {

using namespace xce::tripod;

class NoLockMessage : public xce::Message {
 public:
  explicit NoLockMessage(int id) : id_(id) {}
  virtual ~NoLockMessage() {}
  void Run() {
    LOG(INFO) << "id:" << id_ << "\tstart"; 
    Sleep(1000);
    LOG(INFO) << "id:" << id_ << "\tend"; 
  }
 private:
  int id_;
};

class LockMessage : public xce::Message {
 public:
  LockMessage(int id, const std::string& msg, BucketMutexPtr bucket_mutex) : id_(id), message_(msg), bucket_mutex_(bucket_mutex) {}
  virtual ~LockMessage() {}
  void Run() {
    if (bucket_mutex_) {
      BucketLock lock(message_,bucket_mutex_);
      LOG(INFO) << "id:" << id_ << "\tstart";
      Sleep(1000);
      LOG(INFO) << "id:" << id_ << "\tend";
    }
  } 
 private:
  int id_;
  std::string message_;
  BucketMutexPtr bucket_mutex_;
};

TEST(BucketMutexTest, NoLock) {
  xce::ThreadPool thread_pool(8, 20);
  for (int i = 0; i < 10; ++i) {
    thread_pool.Post(new NoLockMessage(i));
  }
  Sleep(1000);
  thread_pool.Stop();
  thread_pool.Join();
}

TEST(BucketMutexTest, Lock) {
  xce::ThreadPool thread_pool(8, 20);
  std::string message = "fffff";
  BucketMutexPtr mutex;
  mutex.reset(new BucketMutex());
  for (int i = 0; i < 10; ++i) {
    thread_pool.Post(new LockMessage(i, message, mutex));
  }
  Sleep(1000);
  thread_pool.Stop();
  thread_pool.Join();
}

void ThreadFunction(int i, const std::string& message, BucketMutexPtr mutex) {
  if (mutex) {
    BucketLock lock(message,mutex);
    LOG(INFO) << "[" << i << ", " << message << "]\tstart";
    Sleep(1000);
    LOG(INFO) << "[" << i << ", " << message << "]\tend";
  }
}

TEST(BucketMutexTest, Lock2) {
  boost::thread_group group;
  std::string message = "abc";
  BucketMutexPtr mutex;
  mutex.reset(new BucketMutex());
  for (int i = 0; i < 10; ++i) {
    group.create_thread(boost::bind(ThreadFunction, i, message, mutex));
  } 
  group.join_all();
}

} // test


