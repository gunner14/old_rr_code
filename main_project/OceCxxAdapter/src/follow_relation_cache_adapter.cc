#include "follow_relation_cache_adapter.h"
#include "UtilCxx/src/GlobalId.h"

namespace xce {
namespace buddy {

MyUtil::IntSeq FollowRelationCacheAdapter::getList(const MyUtil::GlobalIdPtr& gid, Ice::Int begin, Ice::Int limit){
  MyUtil::IntSeq seq;

  if(!checkParams(begin, limit)){
    return seq;
  }
  
  try{
    seq = getProxy(gid->id)->getList(gid, begin, limit);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
  return seq;
}

MyUtil::IntSeq FollowRelationCacheAdapter::getCommon(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, Ice::Int begin, Ice::Int limit){
  MyUtil::IntSeq seq;

  if(!checkParams(uid, begin, limit)){
    return seq;
  }

  try{
    seq = getProxy(gid->id)->getCommon(uid, gid,begin, limit);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
  return seq;
}

MyUtil::IntSeq FollowRelationCacheAdapter::getCommonAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const long time, Ice::Int begin, Ice::Int limit){
  MyUtil::IntSeq seq;

  if(!checkParams(uid, begin, limit)){
    return seq;
  }

  try{
    seq = getProxy(gid->id)->getCommonAfterTime(uid, gid, time, begin, limit);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
  return seq;
}

int FollowRelationCacheAdapter::getCommonCount(Ice::Int uid, const MyUtil::GlobalIdPtr& gid){
  int count = 0;

  if(uid < 0 ){
    return 0;
  }

  try{
    count = getProxy(gid->id)->getCommonCount(uid,gid);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
  return count;
}

int FollowRelationCacheAdapter::getCommonCountAfterTime(Ice::Int uid, const MyUtil::GlobalIdPtr& gid, const long time){
  int count = 0;

  if(uid < 0 ){
    return 0;
  }

  try{
    count = getProxy(gid->id)->getCommonCountAfterTime(uid, gid, time);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
  return count;
}

bool FollowRelationCacheAdapter::hasRelation(Ice::Int uid, const MyUtil::GlobalIdPtr& gid){
  bool hasRelation = false;

  if(uid < 0 ){
    return false;
  }

  try{
    hasRelation = getProxy(gid->id)->hasRelation(uid,gid);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
  return hasRelation;
}

int FollowRelationCacheAdapter::getCount(const MyUtil::GlobalIdPtr& gid){
  int count = 0;
  try{
    count = getProxy(gid->id)->getCount(gid);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
  return count;
}

void FollowRelationCacheAdapter::reload(const long globalId){
  try{
    getProxy(globalId)->reload(globalId);
  } catch(Ice::TimeoutException& e){
    std::cout << "TimeoutException!" << std::endl;
  } catch(std::exception& e){
    std::cout << "Exception:" << e.what() << std::endl;
  }
}
}} // xce::test


