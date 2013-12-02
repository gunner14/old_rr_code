#ifndef _TRIPOD_REDIS_CACHE_CLIENT_H__
#define _TRIPOD_REDIS_CACHE_CLIENT_H__

#include <Common.h>
#include <HashHolder.hpp>
#include <ConfigClientFactory.h>
#include <RedisCacheConfigClient.h>
#include <RedisClientPool.h>

namespace com {
namespace renren {
namespace tripod {

class RedisCacheClient {
 public:
  StrList GetList(const std::string& key, int begin, int limit, const std::string& namespace_id, const std::string& business_id);
  
  bool SetList(const std::string& key, const StrList& value,
           const std::string& namespace_id, const std::string& business_id);

  bool Remove(const std::string& key, const std::string& namespace_id, const std::string& business_id);

  bool GetHash(const std::string& key, const StrList& fields, HashPair& hp,
                   const std::string& namespace_id, const std::string& business_id);
  
  bool SetHash(const std::string& key, const HashPair& value,
               const std::string& namespace_id, const std::string& business_id);

  // if key exists, return kReplySucess
  // if not exists, return kReplyMiss
  // if connection become invalid, return kReplyConnectionError
  int Exists(const std::string& key, const std::string& namespace_id, const std::string& business_id);

 protected:
  RedisCacheClient(const std::string& zk_address);
 
  std::string GetBigKey(const std::string& key, const std::string& business_id) {
    return business_id + "_" + key;
  }
  std::string GetKey(const std::string& big_key, const std::string& business_id) {
    return big_key.substr(business_id.length() + 1, business_id.length());
  }
 
  RedisCacheConfigClientPtr GetConfig();
 private:
  friend class ClientFactory; 

 private:
  RedisCacheConfigClientPtr config_;
  boost::shared_mutex config_mutex_;
  std::string zk_address_;
};

}}} // end com::renren::tripod

#endif

