#ifndef __AD_EDM_USER_TARGET_CACHE_ADAPTER_H__
#define __AD_EDM_USER_TARGET_CACHE_ADAPTER_H__

#include "AdapterI.h"
#include "AdUserFeatureCache.h"

namespace xce{
namespace ad{

using namespace MyUtil;

class AdUserFeatureCacheAdapter : public MyUtil::ReplicatedClusterAdapterI<AdUserFeatureCachePrx>,
  public MyUtil::Singleton<AdUserFeatureCacheAdapter>{
public:
  AdUserFeatureCacheAdapter() : MyUtil::ReplicatedClusterAdapterI<AdUserFeatureCachePrx>("ControllerAdUserFeatureCache",120,60,new EdmChannel) ,server_index_(0) {
    srand(time(NULL));
  }
  
  std::string GetFeature(Ice::Long userID);
  bool Rollback(const std::string& fileName);
  std::string GetNowFile();
    
protected:
  virtual string name(){
    return "ADEDM";
  }
  virtual string endpoints(){
    return "@AdUserFeatureCache";
  }
  virtual size_t cluster(){
    return 1;
  }
  AdUserFeatureCachePrx getManagerOneway(int server_index);
  AdUserFeatureCachePrx getManager(int server_index);
private:
  int server_index_;
  vector<AdUserFeatureCachePrx> oneway_proxy_;
  vector<AdUserFeatureCachePrx> twoway_proxy_;
};

}
}
#endif
