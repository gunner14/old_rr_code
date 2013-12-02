#ifndef AD_EXPERIMENT_ADAPTER_H_
#define AD_EXPERIMENT_ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "AdExperiment.h"

namespace xce {
namespace ad {

using namespace MyUtil;

class AdExperimentAdapter: public MyUtil::AdapterI,
    public MyUtil::AdapterISingleton<MyUtil::AdChannel, AdExperimentAdapter> {

public:
  AdExperimentAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }
  
  string GetModelMd5sumById(int sindex, int id){
    return getManager(getsindex(sindex))->GetModelMd5sumById(id);
  }
 
  AdRefreshCtrRatioMap GetAdRefreshCtrRatioPoolById(int sindex, int id){
    return getManager(getsindex(sindex))->GetAdRefreshCtrRatioPoolById(id);
  }
 
  CtrOnlineModelMap GetCtrOnlineModelPoolById(int sindex, int id){
    return getManager(getsindex(sindex))->GetCtrOnlineModelPoolById(id);
  }

  AdClassListMap GetAdClassListPool(int sindex){
    return getManager(getsindex(sindex))->GetAdClassListPool();
  }
  
  AdExtraDataMap GetAdExtraDataPool(int sindex){
    return getManager(getsindex(sindex))->GetAdExtraDataPool();
  }

  AdCtrFeatureList GetAdCtrFeaturePool(int sindex){
    return getManager(getsindex(sindex))->GetAdCtrFeaturePool();
  }
  
  AdCtrRealTimeMap GetAdCtrRealTimePool(int sindex){
    return getManager(getsindex(sindex))->GetAdCtrRealTimePool();
  }
  
  AdMemberIndustryMap GetAdMemberIndustryPool(int sindex){
    return getManager(getsindex(sindex))->GetAdMemberIndustryPool();
  }

private:
  AdExperimentManagerPrx getManager(int id){
      return locate<AdExperimentManagerPrx> (_managers, "M", id, TWO_WAY);
  }
  AdExperimentManagerPrx getManagerOneway(int id){
      return locate<AdExperimentManagerPrx> (_managersOneway, "M", id, ONE_WAY);
  }
  
  int getsindex(int sindex);
  virtual string name() {
    return "M";
  }
  virtual string endpoints() {
    return "@AdExperimentLoaderCache";
  }
  virtual size_t cluster() {
    return 3; 
  }

  vector<AdExperimentManagerPrx> _managersOneway;
  vector<AdExperimentManagerPrx> _managers;
};

}
}


#endif // AD_EXPERIMENT_ADAPTER_H_
