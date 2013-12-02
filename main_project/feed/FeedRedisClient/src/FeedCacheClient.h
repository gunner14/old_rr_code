#ifndef __XCE_FEED_CACHE_CLIENT_H__
#define __XCE_FEED_CACHE_CLIENT_H__

#include <iostream>
#include "Singleton.h"
#include "CacheLocationConfig.h"
#include "feed/FeedRedisClient/src/ClientPool.h"
#include "feed/FeedRedisClient/src/FeedRedisClient.h"


namespace xce {
namespace feed {


class FeedCacheClient {
public:
  ~FeedCacheClient() {
  }

  FeedCacheClient() {
  }

  RedisClientPtr GetRedisClient(const std::string key) {
    Continuum::CachePoint cp = CacheLocationConfig::instance().GetCachePoint(key.c_str(), key.size());

#ifdef STDOUT
    std::cout << "hash:" << cp.hash_point_ << " desc:" << cp.desc_ << " ip:" << cp.ip_addr_ << " port:" << cp.port_ << " cap:" << cp.capacity_ << std::endl; 
#else
    MCE_INFO("hash:" << cp.hash_point_ << " desc:" << cp.desc_ << " ip:" << cp.ip_addr_ << " port:" << cp.port_ << " cap:" << cp.capacity_); 
#endif

    xce::feed::RedisClientPtr client = xce::feed::RedisClientPoolFactory::instance().GetClient(cp.ip_addr_, cp.port_);

    if(!client) {
      MCE_WARN("FeedCacheClient::GetRedisClient --> client is NULL! key:" << key << " len:" << key.size() << " desc:" << cp.desc_ << " ip:" << cp.ip_addr_ << " port:" << cp.port_);
      return client;
    }

    if(!(client->IsValid())) {
      MCE_WARN("FeedCacheClient::GetRedisClient --> client is not valid! source:" << key << " len:" << key.size() << " desc:" << cp.desc_ << " ip:" << cp.ip_addr_ << " port:" << cp.port_);
      client.reset();
      return client;
    }

    return client;

  }

  bool Push2List(const std::string key, RedisBinArgv& cmd_argv) {

    int cn = 0;
    while (cn < kRetry) {

      RedisClientPtr client = GetRedisClient(key);
      if (!client) {
        cn++;
        continue;
      }

      if (!(client->Push2List(cmd_argv))) {
        MCE_WARN("FeedCacheClient::Push2List --> return false. key:" << key);
        cn++;
        client.reset();
        continue;
      }


      xce::feed::RedisClientPoolFactory::instance().ReleaseClient(client);
      return true;
    }

    return false;

  }

  template <class T>
  bool GetList(const std::string& key, std::vector<T>& ret_vec, int begin = 0, int limit = 1) {


    int cn = 0;
    while (cn < kRetry) {

      RedisClientPtr client = GetRedisClient(key);
      if (!client) {
        cn++;
        continue;
      }
      
      if (!(client->GetList<T>(key, ret_vec, begin, limit))) {
        MCE_WARN("FeedCacheClient::GetList --> GetList return false. key:" << key << " begin:" << begin << " limit:" << limit);
        cn++;
        client.reset();
        continue;
      }


      xce::feed::RedisClientPoolFactory::instance().ReleaseClient(client);
      return true;
    }

    return false;

  }

  bool Del(const std::string& key) {

    int cn = 0;
    while (cn < kRetry) {

      RedisClientPtr client = GetRedisClient(key);
      if (!client) {
        cn++;
        continue;
      }

      if (!(client->Del(key))) {
        MCE_WARN("FeedCacheClient::Del --> return false. key:" << key);
        cn++;
        client.reset();
        continue;
      }


      xce::feed::RedisClientPoolFactory::instance().ReleaseClient(client);
      return true;
    }

    return false;
  }


 // static const size_t LIST_ITEM_SIZE;
 // static const size_t KEY_SIZE;
  static const int kRetry = 2;
  static const int kGetClientRetry = 5;
 // static const size_t LIST_LIMIT;


};

}}
#endif 
