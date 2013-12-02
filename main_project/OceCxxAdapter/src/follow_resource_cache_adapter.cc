#include "follow_resource_cache_adapter.h"

namespace xce {
namespace buddy {

MyUtil::LongSeq FollowResourceCacheAdapter::getList(const int userId, const int type, const int begin, const int limit){
  //对proxy做缓存能否提高性能？
  MyUtil::LongSeq seq;
  try{
    seq = getProxy(userId)->getList(userId, type, begin, limit);
  } catch(std::exception& e){
    std::cout << e.what() << std::endl;
  }
  return seq;
}

int FollowResourceCacheAdapter::getCount(const int userId, const int type){
  int count = 0;
  try{
    count = getProxy(userId)->getCount(userId, type);
  } catch(std::exception& e){
    std::cout << e.what() << std::endl;
  }
  return count;
}

void FollowResourceCacheAdapter::reload(const int userId, const int type){
  try{
    getProxy(userId)->reload(userId, type);
  } catch(std::exception& e){
    std::cout << e.what() << std::endl;
  }
}

MyUtil::BoolSeq FollowResourceCacheAdapter::hasRelationBatch(const int userId, const int type, const MyUtil::IntSeq& idSeq){
  MyUtil::BoolSeq seq;
  try{
    seq = getProxy(userId)->hasRelationBatch(userId,type,idSeq);
  } catch(std::exception& e){
    std::cout << e.what() << std::endl;
  }
  return seq;
} 

}} // xce::test
