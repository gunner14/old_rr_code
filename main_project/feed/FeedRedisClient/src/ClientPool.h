#ifndef __XCE_FEED_REDIS_CLIENT_POOL_H__
#define __XCE_FEED_REDIS_CLIENT_POOL_H__

#include <string>
#include <list>
#include <map>
#include <deque>
#include "boost/thread/mutex.hpp"
#include "boost/shared_ptr.hpp"
#include "LogWrapper.h"
#include "Singleton.h"
#include "RWLock.h"
#include "FeedRedisClient.h"


namespace  xce {
namespace feed {

class RedisClientPool;
typedef boost::shared_ptr<RedisClient> RedisClientPtr;
typedef boost::shared_ptr<RedisClientPool> RedisClientPoolPtr;

class RedisClientPool {
  public:
    RedisClientPool(const std::string& address, int port, long timeout = 1000,  size_t max_size = 10);
    ~RedisClientPool();

    RedisClientPtr GetClient();
    void ReleaseClient(RedisClientPtr);
    std::string Dump();

    std::string address() const {
      return address_;
    }
    int port() const {
      return port_;
    }
    long timeout() const {
      return timeout_;
    }
  //  int id() const {
  //    return id_;
  //  }
    size_t GetFreeClientsSize() const {
      return free_clients_.size();
    }
    size_t used() const {
      return used_;
    }

  private:
    RedisClientPtr Create();
//    void DestroyClient(RedisClientPtr);


  private:
    //static int count_;
    //int id_;
    std::string address_;
    int port_;
    long timeout_; // millisecond

    //size_t core_size_;
    size_t max_size_;

    boost::mutex free_clients_mutex_;
    std::deque<RedisClientPtr> free_clients_;
    size_t used_;
};

class RedisClientPoolFactory : public MyUtil::Singleton < RedisClientPoolFactory > {
	public:
    RedisClientPoolFactory() {
      pthread_rwlockattr_t lockattr;

      if (int ret = pthread_rwlockattr_init(&lockattr)) {
        MCE_WARN("RedisClientPoolFactory::RedisClientPoolFactory() --> pthread_rwlockattr_init error:" << ret);
        exit(-2);
      }

      if (int ret = pthread_rwlockattr_setkind_np(&lockattr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP)) {
        MCE_WARN("RedisClientPoolFactory::RedisClientPoolFactory() --> pthread_rwlockattr_setkind_np error:" << ret);
        exit(-2);
      }


      if (int ret = pthread_rwlock_init(&rwlock_,&lockattr)) {
        MCE_WARN("RedisClientPoolFactory::RedisClientPoolFactory() --> pthread_rwlock_init error:" << ret);
      }

      pthread_rwlockattr_destroy(&lockattr);

    }

		RedisClientPoolPtr GetPool(const std::string& address, int port);
    RedisClientPtr GetClient(const std::string& address, int port);
		void ReleaseClient(RedisClientPtr client);
		void Dump();
	private:
		std::map<std::pair<std::string, int>, RedisClientPoolPtr> pool_map_;
		//boost::mutex mutex_;
    mutable pthread_rwlock_t rwlock_;

};

}
}

#endif
