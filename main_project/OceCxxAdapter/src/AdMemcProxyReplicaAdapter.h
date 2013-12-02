#ifndef _AD_MC_PROXY_REPLICA_ADAPTER_H_
#define _AD_MC_PROXY_REPLICA_ADAPTER_H_

#include <boost/lexical_cast.hpp>

#include "AdapterI.h"
#include "AdMemcProxy.h"
#include "Singleton.h"


namespace xce {
namespace ad {

using namespace MyUtil;

class AdMemcProxyReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<AdMemcProxyPrx>, public Singleton<AdMemcProxyReplicaAdapter>
{
public:
  AdMemcProxyReplicaAdapter(): MyUtil::ReplicatedClusterAdapterI<AdMemcProxyPrx>("ControllerAdMemcProxy", 120, 300, new AdChannel()) {
  }

  string GetEndpoint(int index) {
    return getManager(index)->GetEndpoint();
  }
  
  ProxyInfoPtr GetProxyInfo(int index){
    return getManager(index)->GetProxyInfo();
  }

  size_t GetClusterSize() {
    return cluster();
  }

protected:
	virtual string name() {
		return "AdMemcProxy";
	}
	
	virtual string endpoints() {
		return "@AdMemcProxy";
	}
	
	virtual size_t cluster() {
		return 1;
	}

private:
  vector<AdMemcProxyPrx> _managersOneway;
  vector<AdMemcProxyPrx> _managers;

  AdMemcProxyPrx getManagerOneway(int id){
    return getProxyOneway(id);
  }

  AdMemcProxyPrx getManager(int id){
    return getProxy(id);
  }
};

}
}

#endif // _AD_MC_PROXY_REPLICA_ADAPTER_H_
