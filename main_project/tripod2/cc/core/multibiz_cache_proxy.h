#ifndef MULTIBIZ_CACHE_PROXY_
#define MULTIBIZ_CACHE_PROXY_

#include <string>
#include <map>
#include "boost/shared_ptr.hpp"
#include "tripod2/cc/core/cache_proxy.h"


namespace xce {
namespace tripod {

class MultiBizCacheProxy : public CacheProxy {
 public:
  explicit MultiBizCacheProxy(const std::string& ns, int conn_time_out): CacheProxy(ns,"",conn_time_out) { } 
  virtual ~MultiBizCacheProxy() {}

  std::map<std::string, bool> MultiBizGet(const std::string& key,
      const StringListType& bizs, StringMapType* values);
};

}} // xce::tripod
#endif

