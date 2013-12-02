#include <RedisCacheClient.h>
#include <boost/foreach.hpp>
#include <Common.h>

namespace com {
namespace renren {
namespace tripod {

RedisCacheClient::RedisCacheClient(const std::string& zk_address) : zk_address_(zk_address) {
  TRIPOD_INFO("RedisCacheClient::RedisCacheClient zk_address " + zk_address_);
}

RedisCacheConfigClientPtr RedisCacheClient::GetConfig() {
  if(config_ == NULL) {
    boost::upgrade_lock<boost::shared_mutex> lock(config_mutex_);
    boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock1(lock);
    config_ = ConfigClientFactory::getInstance()->GetRedisCacheConfigClient(zk_address_);
  }
  return config_;
}


StrList RedisCacheClient::GetList(const std::string& key,  
                              int begin, 
                              int limit,
                              const std::string& namespace_id, 
                              const std::string& business_id) {
  StrList result;
  RedisCacheConfigClientPtr config = GetConfig();
  if (!config) {
    MCE_DEBUG("RedisCacheClient::get config is NULL, namespace_id " + namespace_id + " business_id " + business_id);
    return result;
  }
  RedisClientPoolPtr cache_pool = config->GetPrx(key, namespace_id);
  MCE_DEBUG("cache_pool->Dump():" << cache_pool->Dump());
  std::string cache_key = GetBigKey(key, business_id);
  RedisClient* redis_client = cache_pool->BorrowItem();
  if (!redis_client || !redis_client->IsValid()) {
    MCE_WARN("RedisCacheClient::GetList() not enough connection to redis server, business_id:" << business_id 
              << " RedisClientPoolPtr cache_pool->Dump():" << cache_pool->Dump());
    return result;
  }
  redis_client->GetList(cache_key, result, begin, limit);
  cache_pool->ReturnItem(redis_client);
  return result;
}


bool RedisCacheClient::SetList(const std::string& key, const StrList& value,
                           const std::string& namespace_id, const std::string& business_id) {
  RedisCacheConfigClientPtr config = GetConfig();
  if (!config) {
    MCE_DEBUG("RedisCacheClient::get config is NULL, namespace_id " + namespace_id + " business_id " + business_id);
    return false;
  }
  RedisClientPoolPtr cache_pool = config->GetPrx(key, namespace_id);
  MCE_DEBUG("cache_pool->Dump():" << cache_pool->Dump());
  std::string cache_key = GetBigKey(key, business_id);
  RedisClient* redis_client = cache_pool->BorrowItem();
  if (!redis_client || !redis_client->IsValid()) {
    MCE_WARN("RedisCacheClient::SetList() not enough connection to redis server, business_id:" << business_id);
    return false;
  }
  bool flag = redis_client->SetList(cache_key, value);
  cache_pool->ReturnItem(redis_client);
  return flag;
  
}

bool RedisCacheClient::Remove(const std::string& key, const std::string& namespace_id, const std::string& business_id) {
  RedisCacheConfigClientPtr config = GetConfig();
  if (!config) {
    MCE_DEBUG("RedisCacheClient::get config is NULL, namespace_id " + namespace_id + " business_id " + business_id);
    return false;
  }
  RedisClientPoolPtr cache_pool = config->GetPrx(key, namespace_id);
  MCE_DEBUG("cache_pool->Dump():" << cache_pool->Dump());
  std::string cache_key = GetBigKey(key, business_id);
  RedisClient* redis_client = cache_pool->BorrowItem();
  if (!redis_client || !redis_client->IsValid()) {
    MCE_WARN("RedisCacheClient::Remove() not enough connection to redis server, business_id:" << business_id);
    return false;
  }
  bool flag = redis_client->Remove(cache_key);
  cache_pool->ReturnItem(redis_client);
  return flag;
}

bool RedisCacheClient::GetHash(const std::string& key, const StrList& fields, HashPair& hp,
                                   const std::string& namespace_id, const std::string& business_id) {
  RedisCacheConfigClientPtr config = GetConfig();
  if (!config) {
    MCE_DEBUG("RedisCacheClient::get config is NULL, namespace_id " + namespace_id + " business_id " + business_id);
    return false;
  }
  RedisClientPoolPtr cache_pool = config->GetPrx(key, namespace_id);
  MCE_DEBUG("cache_pool->Dump():" << cache_pool->Dump());
  std::string cache_key = GetBigKey(key, business_id);
  RedisClient* redis_client = cache_pool->BorrowItem();
  if (!redis_client || !redis_client->IsValid()) {
    MCE_WARN("RedisCacheClient::GetHash() not enough connection to redis server, business_id:" << business_id);
    MCE_WARN("RedisCacheClient::GetHash() not enough connection to redis server, business_id:" << business_id);
    return false;
  }
  bool flag = redis_client->GetHash(cache_key, fields, hp);
  cache_pool->ReturnItem(redis_client);
  return flag;
}

bool RedisCacheClient::SetHash(const std::string& key, const HashPair& value,
                               const std::string& namespace_id, const std::string& business_id) {
  RedisCacheConfigClientPtr config = GetConfig();
  if (!config) {
    MCE_DEBUG("RedisCacheClient::get config is NULL, namespace_id " + namespace_id + " business_id " + business_id);
    return false;
  }
  RedisClientPoolPtr cache_pool = config->GetPrx(key, namespace_id);
  MCE_DEBUG("cache_pool->Dump():" << cache_pool->Dump());
  std::string cache_key = GetBigKey(key, business_id);
  RedisClient* redis_client = cache_pool->BorrowItem();
  if (!redis_client || !redis_client->IsValid()) {
    MCE_WARN("RedisCacheClient::SetHash() not enough connection to redis server, business_id:" << business_id 
              << " RedisClientPoolPtr cache_pool->Dump():" << cache_pool->Dump());
    return false;
  }
  bool flag = redis_client->SetHash(cache_key, value);
  cache_pool->ReturnItem(redis_client);
  return flag;
}

int RedisCacheClient::Exists(const std::string& key, const std::string& namespace_id, const std::string& business_id) {
  RedisCacheConfigClientPtr config = GetConfig();
  if (!config) {
    MCE_DEBUG("RedisCacheClient::get config is NULL, namespace_id " + namespace_id + " business_id " + business_id);
    return false;
  }
  RedisClientPoolPtr cache_pool = config->GetPrx(key, namespace_id);
  MCE_DEBUG("cache_pool->Dump():" << cache_pool->Dump());
  std::string cache_key = GetBigKey(key, business_id);
  RedisClient* redis_client = cache_pool->BorrowItem();
  if (!redis_client || !redis_client->IsValid()) {
    MCE_WARN("RedisCacheClient::Exists() not enough connection to redis server, business_id:" << business_id);
    return false;
  }
  int result = redis_client->Exists(cache_key);
  cache_pool->ReturnItem(redis_client);
  return result;
}

}}} // end com::renren::tripod

