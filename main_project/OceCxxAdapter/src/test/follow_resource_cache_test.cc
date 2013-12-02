#include <iostream>
#include "../follow_resource_cache_adapter.h"

using xce::buddy::FollowResourceCacheAdapter;

int main(int argc, char *argv[]){

  if (argc < 3) {
    printf("Usage: %s userId, type, [begin], [limit], [getCount/getList/reload/hasRelationBatch](0 for getCount, 1 for getList, 2 for reload, 3 for hasRelationBatch) resourceIds...\n",argv[0]);
    exit(1);
  }

  int userId = 0;
  int type = 0;
  int begin = 0;
  int limit = 1;
  int opt = 0;

  if(argc >= 2){
    userId = atoi(argv[1]);
  }

  if(argc >= 3 ){
    type = atoi(argv[2]);
  }

  if(argc >= 4 ){
    begin = atoi(argv[3]);
  }

  if(argc >= 5 ){
    limit = atoi(argv[4]);
  }

  if(argc >= 6 ){
    opt = atoi(argv[5]);
  }

  MyUtil::IntSeq idSeq;

  switch(opt){
    case 0:
      try{
        int count = FollowResourceCacheAdapter::instance().getCount(userId, type);
        printf("%d\n", count);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 1:
      try{
        MyUtil::LongSeq idList;
        idList = FollowResourceCacheAdapter::instance().getList(userId, type, begin, limit);
        for(MyUtil::LongSeq::iterator it = idList.begin(); it != idList.end(); ++it){
          printf("%d\n", *it);
        }
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 2:
      try{
        FollowResourceCacheAdapter::instance().reload(userId, type);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 3:
      for(int i = 6; i < argc ; ++i ){
        idSeq.push_back(atoi(argv[i]));
      }

      if(idSeq.size() > 0){
        MyUtil::BoolSeq resultSeq;
        try{
          resultSeq = FollowResourceCacheAdapter::instance().hasRelationBatch(userId, type, idSeq);
        } catch (std::exception& e) {
          std::cerr << e.what() << std::endl;
        }

        for(MyUtil::BoolSeq::iterator it = resultSeq.begin(); it != resultSeq.end(); ++it){
          std::cout << *it << std::endl;
          //printf("%d\n", *it);
        }
      }
      break;
    default:
      break;
  }

//  try {
//    MyUtil::LongSeq idList;
//    printf("rpc calling...getCount()\n");
//    int count = FollowResourceCacheAdapter::instance().getCount(userId, type);
//    printf("rpc call success. count = %d\n", count);
//
//    printf("rpc calling...getList()\n");
//    idList = FollowResourceCacheAdapter::instance().getList(userId, type, begin, limit);
//    printf("rpc call result:\n");
//    for(MyUtil::LongSeq::iterator it = idList.begin(); it != idList.end(); ++it){
//      printf("--%d\n", *it);
//    }
//
//    printf("rpc calling...reload()\n");
//    FollowResourceCacheAdapter::instance().reload(userId, type);
//    printf("rpc call success.");
//  } catch (std::exception& e) {
//    std::cerr << e.what() << std::endl;
//  }

  return 0;
}




