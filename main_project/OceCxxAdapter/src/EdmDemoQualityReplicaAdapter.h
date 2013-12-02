#ifndef AD_DEMOQUALITY_ADAPTER_H_
#define AD_DEMOQUALITY_ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "EdmDemoQuality.h"

namespace xce {
namespace ad {

using namespace MyUtil;

class EdmDemoQualityReplicaAdapter: public MyUtil::ReplicatedClusterAdapterI<EdmDemoQualityManagerPrx>,
		public MyUtil::Singleton<EdmDemoQualityReplicaAdapter>{
    //public MyUtil::AdapterISingleton<MyUtil::EdmChannel, EdmDemoQualityAdapter> {
public:
  /*EdmDemoQualityAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }
	*/
	EdmDemoQualityReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<EdmDemoQualityManagerPrx> ("ControllerEdmDemoLoaderCache",120,300,new EdmChannel) {
	    srand(time(NULL));
	  }

  EdmQualityMap GetEdmPool(int sindex){
    return getManager(getsindex(sindex))->GetEdmPool();
  }
  EdmQualityMap GetAdEdmPool(int sindex){
    return getManager(getsindex(sindex))->GetAdEdmPool();
  }
private:
  EdmDemoQualityManagerPrx getManager(int id){
		//return locate<EdmDemoQualityManagerPrx> (_managers, "M", id, TWO_WAY);
		return getProxy(id);	
  }
  EdmDemoQualityManagerPrx getManagerOneway(int id){
      //return locate<EdmDemoQualityManagerPrx> (_managersOneway, "M", id, ONE_WAY);
		return getProxyOneway(id);
  }

  
	int getsindex(int sindex);
  virtual string name() {
    return "M";
  }
  virtual string endpoints() {
    return "@EdmDemoLoaderCache";
  }

  vector<EdmDemoQualityManagerPrx> _managersOneway;
  vector<EdmDemoQualityManagerPrx> _managers;
};

}
}


#endif // AD_DEMOQUALITY_ADAPTER_H_
