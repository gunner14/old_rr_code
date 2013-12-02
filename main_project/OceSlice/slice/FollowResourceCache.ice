#ifndef _FOLLOW_RESOURCE_CACHE_ICE_
#define _FOLLOW_RESOURCE_CACHE_ICE_ 
#include <Util.ice>

module xce {
  module buddy {
    interface FollowResourceCache{
      int getCount(int userId, int type);
      MyUtil::LongSeq getList(int userId, int type, int begin, int limit);
      MyUtil::BoolSeq hasRelationBatch(int userId, int type, MyUtil::IntSeq idSeq);
      void reload(int userId, int type);
    };
  };
};

#endif
