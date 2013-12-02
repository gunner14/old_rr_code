/*************************************************
 *
 *  @Copyright    Renren inc.
 *  @Brief        AdUseCacheLoader模块的查询队列接口层实现
 *  @Filename     AdUserList.cpp
 *  @Author       chaojie.liu
 *  @Email        chaojie.liu@renren-inc.com
 *  @Date         2012-09-08
 *  @History
 *
 *************************************************/

#include "AdUserList.h"
#include "AdUserDataWrapper.h"
#include "AdUserCacheDBAccess.h"
#define MAX_CACHE 100000
using namespace xce::ad;
//向查询队列放入一个uid
void AdUserList::Put(int &uid) {
//  MCE_DEBUG("AdUserList::Put ---> uid = " << uid);
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  if(q_.size() >= MAX_CACHE){
//    MCE_DEBUG("List is full");
    return;
  }

  std::list<int>::iterator it = find(q_.begin(), q_.end(), uid);
  if (it != q_.end()){
    return;
  }

  q_.push_back(uid);
  if (q_.size() == 1){ 
    notify();
  }    
}
//从查询队列获取一个uid
int AdUserList::Get() {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  while (q_.size() == 0){
    wait();
  }
  int uid = q_.front();
  q_.pop_front();
  MCE_INFO("AdUserList::Get ---> uid = "<< uid);
  return uid;
}
//线程启动
int AdUserUpdate::Init() {

  start().detach();

  return 0;
}

void AdUserUpdate::run() {
  MCE_INFO("AdUserUpdate::run()");

  while(true) {
    //从队列取出一个uid
    int uid = user_list_.Get();
    vector<int> uids;
    AdUserBaseInfoMap res;
    uids.push_back(uid);
    vector<int> miss_ids;
    UserProfileMap upfMap; 
    AdUserCacheDBAccess db_obj;
    UserProfile upf;
    //从ad_user_info中读取用户定向信息
    if (db_obj.GetFromAduserinfoDB(uids, miss_ids, upfMap)) {
      MCE_WARN("AdUserUpdate::run --> GetFromAduserinfoDB error!");
    } 

    //二级缓存查到了该用户信息,此时其id肯定为userId
    if (miss_ids.empty()) {
      upf = upfMap[uid];

      //更新一级缓存memcache
      if (db_obj.UpdateMemcache(upf)) {
        MCE_WARN("AdUserUpdate::run ----> UpdateMemcache error!" );
      }   
    } else {
      //从源DB获取对应的用户基础信息
      if (db_obj.GetBaseInfoFromSourceDB(uids, res)) {
        MCE_WARN("AdUserUpdate::run --> GetBaseInfoFromSourceDB error!");
      }
      for (AdUserBaseInfoMap::iterator it = res.begin(); it != res.end(); it++) {
        AdUserProfileInference infer_obj;
        //根据用户基础信息生成定向信息
        upf = infer_obj.Inference(it->second);
        //更新二级缓存ad_user_info
        if (db_obj.UpdateAduserinfoDB(upf)) {
          MCE_WARN("AdUserUpdate::run --> UpdateAduserinfoDB error!");
        }
        //更新一级缓存memcache
        if (db_obj.UpdateMemcache(upf)) {
          MCE_WARN("AdUserUpdate::run --> UpdateMemcache error!");
        }
      }
//      MCE_DEBUG("AdUserUpdate::run ---> uid = "<< uid<<" , update finish");
    }
  }
}
//提供给数据服务层的接口
void AdUserUpdate::PutId(int uid) {
//  MCE_DEBUG("AdUserUpdate::PutId ---> uid = " << uid);
  user_list_.Put(uid);
}

