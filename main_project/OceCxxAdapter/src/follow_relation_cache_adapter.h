#ifndef _FOLLOW_RELATION_CACHE_ADAPTER_H__
#define _FOLLOW_RELATION_CACHE_ADAPTER_H__

#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "FollowRelation.h"

namespace xce {
namespace buddy {


class FollowRelationCacheAdapter : public MyUtil::ReplicatedClusterAdapterI<FollowRelationCachePrx>, 
                                 public MyUtil::Singleton<FollowRelationCacheAdapter> {
 public:
  FollowRelationCacheAdapter() : MyUtil::ReplicatedClusterAdapterI<FollowRelationCachePrx>("ControllerFollowRelationCache", 120, 1000) {

  }
  
  MyUtil::IntSeq getList(const MyUtil::GlobalIdPtr& gid, Ice::Int begin, Ice::Int limit);
  int getCommonCount(Ice::Int uid, const MyUtil::GlobalIdPtr& gid);
  int getCommonCountAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const long time);
  MyUtil::IntSeq getCommon(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, Ice::Int begin, Ice::Int limit);
  MyUtil::IntSeq getCommonAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const long time, Ice::Int begin, Ice::Int limit);
  int getCount(const MyUtil::GlobalIdPtr& gid);
  bool hasRelation(Ice::Int uid, const MyUtil::GlobalIdPtr& gid);
  void reload(const long globalId);

 private:
  bool checkParams(int begin, int limit){
    if ( begin < 0 || limit < 0) {
      return false;
    }
    return true;
  }

  bool checkParams(int uid, int begin, int limit){
    if (uid < 0) {
      return false;
    }

    if (!checkParams(begin, limit)){
      return false;
    }

    return true;
  }

};

}}

#endif

