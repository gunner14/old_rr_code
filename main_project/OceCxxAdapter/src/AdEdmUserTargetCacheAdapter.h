#ifndef __AD_EDM_USER_TARGET_CACHE_ADAPTER_H__
#define __AD_EDM_USER_TARGET_CACHE_ADAPTER_H__

#include "AdapterI.h"
#include "AdEdmUserTargetCache.h"

namespace xce{
namespace ad{

using namespace MyUtil;
//class AdEdmUserTargetCacheAdapter : public AdapterI, public AdapterISingleton<MyUtil::AdChannel, AdEdmUserTargetCacheAdapter>{
class AdEdmUserTargetCacheAdapter : public MyUtil::ReplicatedClusterAdapterI<AdEdmUserTargetCachePrx>,
  public MyUtil::Singleton<AdEdmUserTargetCacheAdapter>{
public:
  //AdEdmUserTargetCacheAdapter() : server_index_(0) {
  //}
  AdEdmUserTargetCacheAdapter() : MyUtil::ReplicatedClusterAdapterI<AdEdmUserTargetCachePrx>("ControllerAdEdmUserTargetCache",120,300,new EdmChannel) ,server_index_(0) {
    srand(time(NULL));
  }
  
  AdgroupSeq GetAdSeqByUserID(const int userID);                  //ICE接口 获得Edm广告
  bool RmAdgroupUserID(const Ice::Long adgroupID, const int userID);    //ICE接口 删除某Edm广告的用户群中>指定用户
    
  //int getCluster(){
  //  return _cluster;
  //}
protected:
  virtual string name(){
    return "ADEDM";
  }
  virtual string endpoints(){
    return "@AdEdmUserTargetCache";
  }
  virtual size_t cluster(){
    return 1;
  }
  AdEdmUserTargetCachePrx getManagerOneway(int server_index);
  AdEdmUserTargetCachePrx getManager(int server_index);
private:
  int server_index_;
  vector<AdEdmUserTargetCachePrx> oneway_proxy_;
  vector<AdEdmUserTargetCachePrx> twoway_proxy_;
};

}
}
#endif
