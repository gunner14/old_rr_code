#ifndef _FOLLOW_RELATION_ICE_
#define _FOLLOW_RELATION_ICE_ 
#include <Util.ice>

module xce {
  module buddy {

    interface FollowRelationCache{
      MyUtil::IntSeq getList(MyUtil::GlobalId gid, int begin, int limit);
      MyUtil::IntSeq getCommon(int uid, MyUtil::GlobalId gid, int begin, int limit);
      int getCommonCount(int uid, MyUtil::GlobalId gid);
      int getCount(MyUtil::GlobalId gid);
      bool hasRelation(int uid, MyUtil::GlobalId gid);
      void reload(long gid);
    };

    interface FollowRelationWriter {
      void add(MyUtil::GlobalId gid, int uid);
      void remove(MyUtil::GlobalId gid, int uid);
    };

  };
};

#endif
