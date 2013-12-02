#include "tripod2/cc/core/multibiz_tripod_client.h"
#include "boost/foreach.hpp"
#include "LogWrapper.h"

namespace xce {
namespace tripod {

boost::shared_ptr<CacheManagerProxy> MultiBizTripodClient::LocateCacheManagerProxy(
    const std::string& biz) {
  boost::unique_lock<boost::shared_mutex> wlock(cache_manager_proxy_mutex_);
  CacheManagerProxyMapType::const_iterator iter = cache_manager_proxy_map_.find(biz);
  if (iter == cache_manager_proxy_map_.end()) {
    //create
    boost::shared_ptr<CacheManagerProxy> proxy(new CacheManagerProxy(namespace_id(), biz, write_time_out_));
    cache_manager_proxy_map_.insert(std::make_pair(biz, proxy));
    return proxy;
  }
  return iter->second;
}

std::map<std::string, bool> MultiBizTripodClient::MultiBizGet(const std::string& key,
    const StringListType& bizs, StringMapType* values, bool use_master, int secs) {
  std::map<std::string, bool> result;
  if (!multibiz_cache_proxy_) {
    MCE_ERROR("TripodClient::MultiBizGet() cache_proxy is NULL ");
    return result;
  }
  result = multibiz_cache_proxy_->MultiBizGet(key,bizs,values);
  BOOST_FOREACH(const std::string& biz, bizs) {
    std::map<std::string, bool>::iterator iter = result.find(biz);
    if(iter == result.end() || !iter->second) {
      std::string value;
      boost::shared_ptr<CacheManagerProxy> cache_manager_proxy = LocateCacheManagerProxy(biz);
      result[biz] = cache_manager_proxy->Get(key, &value, secs, use_master);
      (*values)[biz] = value;
    }
  }
  return result;
}

std::map<std::string, bool> MultiBizTripodClient::MultiBizGet(const std::string& key,
    const StringListType& bizs, StringMapType* values, int secs) {
  return MultiBizGet(key, bizs, values, false, secs);
}

}} // xce::tripod


