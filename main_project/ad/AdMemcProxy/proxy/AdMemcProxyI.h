#ifndef _XCE_AD_MEMC_PROXY_
#define _XCE_AD_MEMC_PROXY_

//#include "memc_command.h"

#include "AdMemcProxy.h"
#include "Singleton.h"
#include "TaskManager.h"

namespace xce {
namespace ad {

using namespace MyUtil;

/*class GetMissedTask : public Task {
public:
  GetMissedTask(const MyUtil::StrSeq & keys, MemcCommandPtr cmd);
  virtual void handle();
private:
  MyUtil::StrSeq keys_;
  MemcCommandPtr memc_cmd_;
};*/

class AdMemcProxyI : public AdMemcProxy, public Singleton<AdMemcProxyI> {
public:
  virtual bool SetUpstreamList(const ::std::string & cfg, const Ice::Current& = Ice::Current());

  // 取得McProxy 的 memcache 监听端口
  virtual ::std::string GetEndpoint(const Ice::Current& = Ice::Current());
  virtual ProxyInfoPtr GetProxyInfo(const Ice::Current& = Ice::Current());

  void SetEndpoint(const ::std::string & endpoint) {
    endpoint_ = endpoint;
  }
private:
  std::string endpoint_;
  std::string ip_;
};

}
}
#endif 

