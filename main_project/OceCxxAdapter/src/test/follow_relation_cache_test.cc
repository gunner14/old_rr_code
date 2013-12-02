#include "gtest/gtest.h"
#include <iostream>
#include "../follow_relation_cache_adapter.h"

using namespace xce::buddy;

int main(int argc, char *argv[]){
  int arg1,arg2,arg3,arg4,arg5;
  long arg6;
  int arg7;

  if (argc < 8) {
    printf("Usage: %s type, id, begin, limit, uid, time, getCount/getList/getCommon/getCommonCount(0 for getCount, 1 for getList, 2 for getCommon, 3 for getCommonCount,4 for hasRelation, 5 for getCommonAfterTime, 6 for getCommonCountAfterTime)\n",argv[0]);
    exit(1);
  }

  arg1 = 0;
  arg2 = 0;
  arg3 = 0;
  arg4 = 0;
  arg5 = 0;
  arg6 = 0L;
  arg7 = 0;

  if(argc >= 2){
    arg1 = atoi(argv[1]);
  }

  if(argc >= 3 ){
    arg2 = atoi(argv[2]);
  }

  if(argc >= 4 ){
    arg3 = atoi(argv[3]);
  }

  if(argc >= 5 ){
    arg4 = atoi(argv[4]);
  }

  if(argc >=6){
    arg5 = atoi(argv[5]);
  }

  if(argc >=7){
    arg6 = atol(argv[6]);
  }

  if(argc >=8){
    arg7 = atoi(argv[7]);
  }


  MyUtil::GlobalIdPtr gid = new MyUtil::GlobalId(arg1,arg2);

  int begin = arg3;
  int limit = arg4;
  int uid = arg5;
  long time = arg6;

  int opt = arg7;
  switch(opt){
    case 0:
      try{
        int count = FollowRelationCacheAdapter::instance().getCount(gid);
        printf("%d\n", count);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 1:
      try{
        MyUtil::IntSeq idList;
        idList = FollowRelationCacheAdapter::instance().getList(gid, begin, limit);
        for(MyUtil::IntSeq::iterator it = idList.begin(); it != idList.end(); ++it){
          printf("%d\n", *it);
        }
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 2:
      try{
        MyUtil::IntSeq idList;
        idList = FollowRelationCacheAdapter::instance().getCommon(uid, gid, begin, limit);
        for(MyUtil::IntSeq::iterator it = idList.begin(); it != idList.end(); ++it){
          printf("%d\n", *it);
        }
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 3:
      try{
        int count = FollowRelationCacheAdapter::instance().getCommonCount(uid, gid);
        printf("%d\n", count);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 4:
      try{
        bool result = FollowRelationCacheAdapter::instance().hasRelation(uid, gid);
        printf("%d\n", result);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 5:
      try{
        MyUtil::IntSeq idList;
        idList = FollowRelationCacheAdapter::instance().getCommonAfterTime(uid, gid, time, begin, limit);
        for(MyUtil::IntSeq::iterator it = idList.begin(); it != idList.end(); ++it){
          printf("%d\n", *it);
        }
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 6:
      try{
        int count = FollowRelationCacheAdapter::instance().getCommonCountAfterTime(uid, gid, time);
        printf("%d\n", count);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    default:
      break;
  }


  

//  try {
//    MyUtil::IntSeq idList;
//
//    MyUtil::GlobalIdPtr gid = new MyUtil::GlobalId(arg1,arg2);
//
//    int begin = arg3;
//    int limit = arg4;
//    int uid = arg5;
//
//    printf("rpc calling...getCount()\n");
//    int count = FollowRelationCacheAdapter::instance().getCount(gid);
//    printf("rpc call success. count = %d\n", count);
//
//    printf("rpc calling...getList()\n");
//    idList = FollowRelationCacheAdapter::instance().getList(gid, begin, limit);
//    printf("rpc call result:\n");
//    for(MyUtil::IntSeq::iterator it = idList.begin(); it != idList.end(); ++it){
//      printf("--%d\n", *it);
//    }
//
//    printf("rpc calling...getCommon()\n");
//    idList = FollowRelationCacheAdapter::instance().getCommon(uid,gid, begin, limit);
//    printf("rpc call result:\n");
//    for(MyUtil::IntSeq::iterator it = idList.begin(); it != idList.end(); ++it){
//      printf("--%d\n", *it);
//    }
//
//    printf("rpc calling...getCommonCount()\n");
//    count = FollowRelationCacheAdapter::instance().getCommonCount(uid, gid);
//    printf("rpc call success. common count = %d\n", count);
//
//    printf("rpc calling...hasRelation()\n");
//    bool hasRelation = FollowRelationCacheAdapter::instance().hasRelation(uid, gid);
//    printf("rpc call success. hasRelation = %d\n", hasRelation);
//
//  } catch (std::exception& e) {
//    std::cerr << e.what() << std::endl;
//  }

  return 0;
}




