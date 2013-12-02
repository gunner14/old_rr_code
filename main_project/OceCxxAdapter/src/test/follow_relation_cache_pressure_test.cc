#include "gtest/gtest.h"
#include <iostream>
#include "../follow_relation_cache_adapter.h"
#include <TaskManager.h>
#include "UtilCxx/src/GlobalId.h"

#define TASK_LEVEL_CALL     1

#define MIN_TRHEAD          10
#define MAX_THREAD          500

using namespace xce::buddy;
using namespace MyUtil;

class CallTask: virtual public MyUtil::Task {
public:
    CallTask(int threadId) :
        threadId_(threadId) {
    }

    virtual void handle() {
        try {
            
            //for(int i = 0; i < 1000; ++i){

            while(1){

              int category = rand()%10;
              int id = (int)(rand()*1.0/RAND_MAX*10000);
              int uid = (int)(rand()*1.0/RAND_MAX*10000);

              MyUtil::GlobalIdPtr gid = new MyUtil::GlobalId(category, id);
              long globalId = GlobalIdTransfer::get(gid);
              MyUtil::IntSeq result;

              std::cout << "CallTask " << threadId_ << ": calling getCount, globalId = " << globalId
                      << std::endl;
              int count = FollowRelationCacheAdapter::instance().getCount(gid);
              std::cout << "CallTask " << threadId_ << ": rpc call success. count = " << count
                      << std::endl;
              

              std::cout << "CallTask " << threadId_ << ": calling getCommon, globalId = " << globalId
                      << "userId = " << uid << std::endl;
              result = FollowRelationCacheAdapter::instance().getCommon(uid,
                      gid, 0, -1);
              std::cout << "CallTask " << threadId_ << ": rpc call success. Common List size: "
                      << result.size() << std::endl;


              std::cout << "CallTask " << threadId_ << " calling getList, globalId = " << globalId
                      << std::endl;
              result = FollowRelationCacheAdapter::instance().getList(gid, 0, -1);
              std::cout << "CallTask " << threadId_ << " rpc call success. List size: "
                      << result.size() << std::endl;


              std::cout << "CallTask " << threadId_ << ": calling getCommonCount, globalId = " << globalId
                      << "userId = " << uid << std::endl;
              count = FollowRelationCacheAdapter::instance().getCommonCount(uid, gid);
              std::cout << "CallTask " << threadId_ << ": rpc call success. common count = " << count
                      << std::endl;


              std::cout << "CallTask " << threadId_ << ": calling hasRelation, globalId = " << globalId
                      << "userId = " << uid << std::endl;
              bool hasRelation = FollowRelationCacheAdapter::instance().hasRelation(uid, gid);
              std::cout << "CallTask " << threadId_ << ": rpc call success. hasRelation = " << hasRelation
                      << std::endl;

              //usleep(1000);

            }

           //             for(MyUtil::IntSeq::iterator it = result.begin(); it != result.end(); ++it){
           //                 int id = *it;
           //                 std::cout<<it
           //             }
            //}


            //            std::cout << "CallTask: calling getList, globalId = " << globalId
            //                    << std::endl;
            //            MyUtil::IntSeq result;
            //            result = FollowRelationCacheAdapter::instance().getList(mGid, 0, -1);
            //            std::cout << "CallTask: rpc call success. List size: "
            //                    << result.size() << std::endl;
            //            for(MyUtil::IntSeq::iterator it = result.begin(); it != result.end(); ++it){
            //                int id = *it;
            //                std::cout<<it
            //            }

//            std::cout << "CallTask: calling getCommon, globalId = " << globalId
//                    << std::endl;
//            MyUtil::IntSeq result;
//            result = FollowRelationCacheAdapter::instance().getCommon(mUid,
//                    mGid, 0, -1);
//            std::cout << "CallTask: rpc call success. Common List size: "
//                    << result.size() << std::endl;

        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
private:
    int threadId_;
};

int main(int argc, char *argv[]) {
    TaskManager::instance().config(TASK_LEVEL_CALL, ThreadPoolConfig(
            MIN_TRHEAD, MAX_THREAD));

    //    MyUtil::GlobalIdPtr gid = new MyUtil::GlobalId(1, 108);
    //    long globalId = GlobalIdTransfer::get(gid);
    //    std::cout << "Main: globalId = " << globalId << std::endl;


    int type;
    int id;
    int uid;

    for(int i = 0; i < 10; ++i){
      TaskManager::instance().execute(new CallTask(i));
    }
//    for (type = 0; type < 9; ++type) {
//        for (id = 0; id < 1000; ++id) {
//            //for (uid = 0; uid < 1000; ++uid) {
//            //usleep(1000);
//                MyUtil::GlobalIdPtr gid = new MyUtil::GlobalId(type, id);
//                TaskManager::instance().execute(new CallTask(uid, gid));
//                //usleep(100);
//            //}
//        }
//    }

    while (1)
        ;

    return 0;
}

