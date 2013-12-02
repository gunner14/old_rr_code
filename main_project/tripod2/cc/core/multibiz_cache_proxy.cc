#include "tripod2/cc/core/multibiz_cache_proxy.h"

#include "boost/foreach.hpp"
#include "LogWrapper.h"

namespace xce {
namespace tripod {

std::map<std::string, bool> MultiBizCacheProxy::MultiBizGet(const std::string& key,
    const StringListType& bizs, StringMapType* values) {
  std::map<std::string, bool> result;
  std::list<std::string> cache_keys;
  std::map<std::string,std::string> cache_map;
  BOOST_FOREACH(const std::string& biz, bizs) { 
    std::string cache_key = GenerateCacheKey(biz, key);
    cache_keys.push_back(cache_key);
    cache_map.insert(std::make_pair(biz,cache_key));
  }
  CacheClientPoolPtr pool = LocateCacheClientPool(key);
  if (!pool) {
    MCE_ERROR("CacheProxy::MultiBizGet() call LocateCacheClientPool return NULL key" << key);
    return result;
  }
  CacheClient* cache_client = pool->BorrowItem();
  if (!cache_client || !cache_client->IsValid()) {
    return result;
  }
  std::map<std::string, bool> cache_result;
  std::map<std::string, std::string> cache_values;
  cache_result = cache_client->MultiGet(cache_keys,&cache_values);
  for(std::map<std::string,std::string>::iterator iter= cache_map.begin(); iter != cache_map.end(); ++iter) { 
    if(cache_result.find(iter->second) != cache_result.end()) {
      result.insert(std::make_pair(iter->first,cache_result[iter->second]));
      values->insert(std::make_pair(iter->first,cache_values[iter->second]));
    }
  }
  pool->ReturnItem(cache_client);
  return result;
}


}} // xce::tripod

