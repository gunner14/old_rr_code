#include "tripod2/cc/core/static_cache_proxy.h"

#include "LogWrapper.h"

namespace xce {
namespace tripod {

StaticCacheProxy::StaticCacheProxy(const std::string& ns, const std::string& biz,
    const std::list<std::string>& children_names, int conn_timeout) : CacheProxy(ns, biz,conn_timeout) {
  MCE_INFO("StaticCacheProxy::StaticCacheProxy() ns: " << ns << " biz: " << biz
      << " child_size: " << children_names.size());
  if (IsValid()) {
    ZkClientPtr zk_client = ZkClient::GetInstance(kDefaultZkAddress);
    if (zk_client) {
      zk_client->RemoveZNodeListener(this);
    }
  }
  Update(children_names);
}

std::string StaticCacheProxy::keyType(const std::string& key) {
  std::string cache_key = GenerateCacheKey(biz_id(), key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_WARN("StaticCacheProxy::keyType() ns: " << namespace_id() << "\tbiz:" << biz_id() << "\tcacheKey" << cache_key << "\tlocateCacheClientPool failed");
    return ""; 
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client) {
    MCE_WARN("StaticCacheProxy::keyType() ns: " << namespace_id() << "\tbiz:" << biz_id() << "\tcacheKey" << cache_key << "\tBorrowItem failed");
    return "";
  }
  if (!cache_client->IsValid() ) {
    pool->ReturnItem(cache_client);
    MCE_WARN("StaticCacheProxy::keyType() ns: " << namespace_id() << "\tbiz:" << biz_id() << "\tcacheKey" << cache_key << "\treturn not valid cache_client");
    return "";
  }
  std::string result = cache_client->Type(cache_key);
  pool->ReturnItem(cache_client);
  return result;
}

int StaticCacheProxy::getExpireTime(const std::string& key) {
  std::string cache_key = GenerateCacheKey(biz_id(), key);
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_WARN("StaticCacheProxy::getExpireTime() ns: " << namespace_id() << "\tbiz:" << biz_id() << "\tcacheKey" 
        << cache_key << "\tlocateCacheClientPool failed");
    return -1; 
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client) {
    MCE_WARN("StaticCacheProxy::getExpireTime() ns: " << namespace_id() << "\tbiz:" << biz_id() << "\tcacheKey" 
        << cache_key << "\tBorrowItem failed");
    return -1;
  }
  if (!cache_client->IsValid() ) {
    pool->ReturnItem(cache_client);
    MCE_WARN("StaticCacheProxy::getExpireTime() ns: " << namespace_id() << "\tbiz:" << biz_id() << "\tcacheKey" 
        << cache_key << "\treturn not valid cache_client");
    return -1;
  }
  int result = cache_client->Ttl(cache_key);
  pool->ReturnItem(cache_client);
  return result; 
}


}} // xce::tripod
