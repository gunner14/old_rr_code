#include "tripod2/cc/client/tripod_cache_client_factory.h"

namespace xce {
namespace tripod {
namespace client {

TripodCacheClientPtr TripodCacheClientFactory::GetClient(const Entry& entry) {
  MAP_TYPE::iterator iter;
  if ( ( iter = map_.find(entry) ) == map_.end() ) {
    boost::unique_lock<boost::mutex> lock(mutex_);
    if ( ( iter = map_.find(entry) ) == map_.end() ) {
      map_.insert(make_pair(entry,
          TripodCacheClientPtr(new TripodCacheClient(entry.GetNamespace(),entry.GetBiz()))));
    }
  }
  return map_[entry];
}


}}}



