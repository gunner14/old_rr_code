#ifndef _AD_MC_PROXY_ADAPTER_H
#define _AD_MC_PROXY_ADAPTER_H_

#include <boost/lexical_cast.hpp>

#include "AdapterI.h"
#include "AdMemcProxy.h"
#include "Singleton.h"


namespace xce {
namespace ad {

using namespace MyUtil;

class AdMemcProxyAdapter : public AdapterI,
                   public AdapterISingleton<MyUtil::TriggerChannel, AdMemcProxyAdapter>
{
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

public:
  AdMemcProxyAdapter() {
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
    _managers.resize(cluster() > 0 ? cluster() : 1);
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

private:
  AdMemcProxyPrx getManagerOneway(int id){
    return locate<AdMemcProxyPrx>(_managersOneway, "M", id, ONE_WAY);
  }

  AdMemcProxyPrx getManager(int id){
    return locate<AdMemcProxyPrx>(_managers, "M", id, TWO_WAY);
  }

  vector<AdMemcProxyPrx> _managersOneway;
  vector<AdMemcProxyPrx> _managers;
};

}
}

#endif // _AD_MC_PROXY_ADAPTER_H_
