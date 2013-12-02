/**
 * 用于创建一个静态的CacheProxy，不接收zk更新
 */

#ifndef STATIC_CACHE_PROXY_
#define STATIC_CACHE_PROXY_

#include "tripod2/cc/core/cache_proxy.h"

namespace xce {
namespace tripod {

class StaticCacheProxy : public CacheProxy {
 public:
  StaticCacheProxy(const std::string& ns, const std::string& biz,
      const std::list<std::string>& children_names, int conn_timeout=1000);
  virtual std::string keyType(const std::string& key);
  virtual int getExpireTime(const std::string& key);
};

}} // xce::tripod
#endif

