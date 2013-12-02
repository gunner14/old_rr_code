#ifndef __XOA_SERVICE_ADAPTER_I_H__
#define __XOA_SERVICE_ADAPTER_I_H__

#include "IceUtil/Thread.h"
#include <IceUtil/UUID.h>
#include "IceLogger.h"
#include "xoa2/client/xoa_registry_client.h"

namespace MyUtil {

template<class PrxType>
class XoaServiceAdapterI {
 private:
  Ice::CommunicatorPtr communicator_;
  std::string service_id_;
  xoa2::XoaRegistryClient registry_client_;
  std::vector<vector<PrxType> > proxy_cache_;

 private:
	XoaServiceAdapterI(const char * service_id) : service_id_(service_id), 
      registry_client_("10.2.78.62:2181/xce") {
    Ice::StringSeq args;
    Ice::InitializationData initData;
    communicator_ = Ice::initialize(args, initData);
	}
  
  static XoaServiceAdapterI<PrxType> * instance_;
  static IceUtil::Mutex mutex_;

  std::vector<vector<PrxType> > LoadProxies() {
    std::vector<vector<PrxType> > res;
    vector<string> children;
    registry_client_.GetChildren("/", &children);
    
    vector<PrxType> proxies;
    for(size_t i = 0; i < children.size(); ++i) {
      cout << children[i] << endl;
      string sprx("M:");
      sprx += children[i];
      PrxType twoway_prx = PrxType::checkedCast(
          communicator_->stringToProxy(sprx)->ice_twoway()->ice_timeout(1000)->ice_secure(false));
      proxies.push_back(twoway_prx);
    }

    res.push_back(proxies);

    return res;
  }
 public:
  static XoaServiceAdapterI<PrxType> * instance(const char * service_id) {
    if (instance_ == NULL) {
      XoaServiceAdapterI<PrxType> * adapter = new XoaServiceAdapterI<PrxType>(service_id);
      adapter->LoadProxies();
      {
        IceUtil::Mutex::Lock lock(mutex_);
        if (instance_ == NULL) {
          instance_ = adapter;
        }
      }
    }
    return instance_;
  }

	virtual ~XoaServiceAdapterI() {
    if (communicator_) {
      communicator_->destroy();
    }
	}

	PrxType GetProxy(long mod) {
    static bool loading = false;
    if (proxy_cache_.empty()) {
      if (loading) { // 防止对配置中心的大量并发查询
        return NULL;
      } else {
        loading = true;
        std::vector<vector<PrxType> > proxy_cache = LoadProxies();
        if (proxy_cache_.empty()) {
          if (proxy_cache.empty()) {
            return NULL;
          } else {
            proxy_cache_.swap(proxy_cache);
          }
        }
        loading = false;
      }
    }
    
    vector<PrxType> & proxies = proxy_cache_[mod % proxies.size()];
    if (proxies.empty())
      return NULL;
    return proxies[0];
	}
};

template<class PrxType>
XoaServiceAdapterI<PrxType> * XoaServiceAdapterI<PrxType>::instance_ = NULL;

template<class PrxType>
IceUtil::Mutex XoaServiceAdapterI<PrxType>::mutex_;

}

#endif // __XOA_SERVICE_ADAPTER_I_H__
