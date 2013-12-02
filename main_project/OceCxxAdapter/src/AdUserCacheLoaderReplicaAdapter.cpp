#include "AdUserCacheLoaderReplicaAdapter.h"
namespace xce{
namespace ad{

AdUserCacheLoaderPrx AdUserCacheLoaderReplicaAdapter::getManager(int id) {
  return getProxy(id);
}

AdUserCacheLoaderPrx AdUserCacheLoaderReplicaAdapter::getManagerOneway(int id) {
  return getProxyOneway(id);
}
/*

void AdUserCacheLoaderReplicaAdapter::LoadByIdSeq(const MyUtil::IntSeq& ids){
  if(!ids.empty()){
    getManagerOneway(ids.at(0))->LoadByIdSeq(ids);
  }
}

void AdUserCacheLoaderReplicaAdapter::LoadById(int userid){
  getManagerOneway(userid)->LoadById(userid);
}

void AdUserCacheLoaderReplicaAdapter::ReloadUserInfoByIdSeq(const MyUtil::IntSeq& ids){
  if(!ids.empty()){
    getManagerOneway(ids.at(0))->ReloadUserInfoByIdSeq(ids);
  }
}

void AdUserCacheLoaderReplicaAdapter::ReloadUserInfoById(int userid){
  getManagerOneway(userid)->ReloadUserInfoById(userid);
}
*/


string AdUserCacheLoaderReplicaAdapter::GetById(int userid){
  return getManager(userid)->GetById(userid);
}


            
      //offline load interface
int AdUserCacheLoaderReplicaAdapter::LoadFromAduserinfoDBByIdSeq(MyUtil::IntSeq& userIds) {
  if (!userIds.empty()) {
    return getManager(userIds.at(0))->LoadFromAduserinfoDBByIdSeq(userIds);
  } else {
    return 0;
  }
}
  
int AdUserCacheLoaderReplicaAdapter::LoadFromAduserinfoDBById(int userId) {
  return getManager(userId)->LoadFromAduserinfoDBById(userId);
}

int AdUserCacheLoaderReplicaAdapter::LoadFromSourceDBByIdSeq(MyUtil::IntSeq& userIds) {
  if (!userIds.empty()) {
    return getManager(userIds.at(0))->LoadFromSourceDBByIdSeq(userIds);
  } else {
    return 0;
  }
}

int AdUserCacheLoaderReplicaAdapter::LoadFromSourceDBById(int userId) {
  return getManager(userId)->LoadFromSourceDBById(userId);
}
            
      //offline debug interface 
int AdUserCacheLoaderReplicaAdapter::Set(UserInfo& info,int& flag) {
  srand(time(NULL));
  return getManager(rand() % getCluster())->Set(info,flag);
}

int AdUserCacheLoaderReplicaAdapter::Reset(int userId) {
  return getManager(userId)->Reset(userId);
}

int AdUserCacheLoaderReplicaAdapter::GetDebuggerList(userseq& infoseq) {
  srand(time(NULL));
  return getManager(rand() % getCluster())->GetDebuggerList(infoseq);
}

int AdUserCacheLoaderReplicaAdapter::GetByIdOffline(int userId,int& flag,UserInfo& info) {
  return getManager(userId)->GetByIdOffline(userId,flag,info);
}
      //offline inference interface
int AdUserCacheLoaderReplicaAdapter::OfflineInference(int userId) {
  return getManager(userId)->OfflineInference(userId);
}

int AdUserCacheLoaderReplicaAdapter::SetOfflineGender(int userId,int gender) {
  return getManager(userId)->SetOfflineGender(userId,gender);
}


}
}
