#include "AdEdmUserTargetCacheAdapter.h"

using namespace MyUtil;
using namespace xce::ad;
using namespace boost;

AdEdmUserTargetCachePrx AdEdmUserTargetCacheAdapter::getManagerOneway(int server_index){
  return getProxyOneway(server_index);
  //return locate<AdEdmUserTargetCacheManagerPrx> (oneway_proxy_, "M", server_index, ONE_WAY);
}

AdEdmUserTargetCachePrx AdEdmUserTargetCacheAdapter::getManager(int server_index){
  return getProxy(server_index);
  //return locate<AdEdmUserTargetCacheManagerPrx> (twoway_proxy_, "M", server_index, TWO_WAY);
}

AdgroupSeq AdEdmUserTargetCacheAdapter::GetAdSeqByUserID(const int userID) {
  int index = userID;
  if (userID< 10) {
    index = rand() % 100000;
  }
  index = index % getCluster();
  return getManager(index)->GetAdSeqByUserID(userID);
}

bool AdEdmUserTargetCacheAdapter::RmAdgroupUserID(const Ice::Long adgroupID, const int userID) {
  int i;
  int flag = 0;
  bool result = false;
  for(i = 0; i<getCluster(); i++) {
    if (getManager(i)->RmAdgroupUserID(adgroupID, userID) == false) {
      flag = 1;
      MCE_WARN("AdEdmUserTargetCacheAdapter::RmEdm rm failed <adgroupID:"<<adgroupID<<", userID:"<<userID<<">");
    } else {
      MCE_INFO("AdEdmUserTargetCacheAdapter::RmEdm rm sucess <adgroupID:"<<adgroupID<<", userID:"<<userID<<">");
    }
  }
  if (flag == 0) {
    result = true; 
  }
  return result;
}

