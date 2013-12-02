#ifndef _MC_PROXY_PROXY_ADAPTER_H_
#define _MC_PROXY_PROXY_ADAPTER_H_

#include <boost/lexical_cast.hpp>

#include "AdapterI.h"
#include "FeedMemcachedProxy.h"
#include "Singleton.h"


namespace xce {
namespace feed {

using namespace MyUtil;

class FeedMemcachedProxyAdapter : public AdapterI,
                   public AdapterISingleton<MyUtil::XceFeedMcProxyChannel, FeedMemcachedProxyAdapter>
{
protected:
  virtual string name() {
    return "FeedMemcachedProxy";
  }
  virtual string endpoints() {
    return "@FeedMemcachedProxy";
  }
  virtual size_t cluster() {
    return 50;
  }

public:
  FeedMemcachedProxyAdapter() {
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
  FeedMemcachedProxyPrx getManagerOneway(int id){
    return locate<FeedMemcachedProxyPrx>(_managersOneway, "M", id, ONE_WAY);
  }

  FeedMemcachedProxyPrx getManager(int id){
    return locate<FeedMemcachedProxyPrx>(_managers, "M", id, TWO_WAY);
  }

  vector<FeedMemcachedProxyPrx> _managersOneway;
  vector<FeedMemcachedProxyPrx> _managers;
};

}
}

#endif // _MC_PROXY_PROXY_ADAPTER_H_
