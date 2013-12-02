#ifndef TRIPOD_CACHE_CLIENT_FACTORY_H_
#define TRIPOD_CACHE_CLIENT_FACTORY_H_

#include <string>
#include <map>
#include "tripod2/cc/core/singleton.h"
#include "tripod2/cc/client/tripod_cache_client.h"
#include "tripod2/cc/client/entry_type.h"

namespace xce {
namespace tripod {
namespace client {

class TripodCacheClientFactory : public xce::tripod::Singleton<TripodCacheClientFactory> {
 public:
  virtual ~TripodCacheClientFactory() {}
  TripodCacheClientPtr GetClient(const Entry& entry);
 private:
  friend class xce::tripod::Singleton<TripodCacheClientFactory>;
  typedef std::map<Entry,TripodCacheClientPtr,EntryLessTo> MAP_TYPE; 
  MAP_TYPE map_; 
  boost::mutex mutex_;
};

}}} // xce::tripod
#endif



