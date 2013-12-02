#ifndef MULTIBIZ_TRIPOD_CLIENT_H_
#define MULTIBIZ_TRIPOD_CLIENT_H_

#include <string>


#include "tripod2/cc/core/tripod_client.h"
#include "tripod2/cc/core/multibiz_cache_proxy.h"

namespace xce {
namespace tripod {

class MultiBizTripodClient : public TripodClient {
 public:
  explicit MultiBizTripodClient(const std::string& ns, int read_time_out =1000, int write_time_out = 1000) :TripodClient(ns,"",read_time_out,write_time_out) {
    multibiz_cache_proxy_ = boost::shared_ptr<MultiBizCacheProxy> (new MultiBizCacheProxy(ns,read_time_out)) ;
    write_time_out_= write_time_out;
  }

  virtual ~MultiBizTripodClient() {}

  std::map<std::string, bool> MultiBizGet(const std::string& key,const StringListType& keys,
      StringMapType* values, int secs = 0);
  std::map<std::string, bool> MultiBizGet(const std::string& key,const StringListType& keys,
      StringMapType* values, bool use_master, int secs = 0);

 private:
  boost::shared_ptr<CacheManagerProxy> LocateCacheManagerProxy(const std::string& biz);

  boost::shared_ptr<MultiBizCacheProxy> multibiz_cache_proxy_;
  typedef std::map<std::string, boost::shared_ptr<CacheManagerProxy> > CacheManagerProxyMapType;
  CacheManagerProxyMapType cache_manager_proxy_map_; 
  boost::shared_mutex cache_manager_proxy_mutex_;
  
  int write_time_out_;
};

}} // xce::tripod
#endif


