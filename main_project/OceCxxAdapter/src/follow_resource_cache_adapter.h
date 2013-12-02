#ifndef _FOLLOW_RESOURCE_CACHE_ADAPTER_H__
#define _FOLLOW_RESOURCE_CACHE_ADAPTER_H__

#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "FollowResourceCache.h"

namespace xce {
namespace buddy {

class FollowResourceCacheAdapter : public MyUtil::ReplicatedClusterAdapterI<FollowResourceCachePrx>, 
                                 public MyUtil::Singleton<FollowResourceCacheAdapter> {
 public:
  FollowResourceCacheAdapter() : MyUtil::ReplicatedClusterAdapterI<FollowResourceCachePrx>("ControllerFollowResourceCache", 120, 1000) {} 

  MyUtil::LongSeq getList(const int userId, const int type, int begin, int limit);
  MyUtil::BoolSeq hasRelationBatch(const int userId, const int type, const MyUtil::IntSeq& idSeq);
  int getCount(const int userId, const int type);
  void reload(const int userId, const int type);
};

}}

#endif

