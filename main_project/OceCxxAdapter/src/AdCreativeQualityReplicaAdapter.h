#ifndef AD_CREATIVE_QUALITY_REPLICA_ADAPTER_H_
#define AD_CREATIVE_QUALITY_REPLICA_ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "AdCreativeQuality.h"

namespace xce {
namespace ad {

using namespace MyUtil;

class AdCreativeQualityReplicaAdapter: public MyUtil::ReplicatedClusterAdapterI<AdCreativeQualityManagerPrx>,
    public MyUtil::Singleton<AdCreativeQualityReplicaAdapter> {
public:
	AdCreativeQualityReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<AdCreativeQualityManagerPrx> ("ControllerAdCreativeQuality",120,300,new EdmChannel){
		srand(time(NULL));
	}

  AdQualityMap GetDemoPool(int index);
  FreshAdQualityMap GetFreshAdMap(int index);

private:
  AdCreativeQualityManagerPrx getManager(int id);
  AdCreativeQualityManagerPrx getManagerOneway(int id);

  virtual string name() {
    return "M";
  }
  virtual string endpoints() {
    return "@AdCreativeQuality";
  }
};

}
}


#endif
