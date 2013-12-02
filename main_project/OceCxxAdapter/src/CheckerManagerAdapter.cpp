#include "CheckerManagerAdapter.h"
using namespace xce::adapter::checkermanager;

KeyMsgPtr CheckerManagerAdapter::getKey(const string& beginKey, const int batch,
    const Ice::Context& ctx) {
  return getProxy(0)->getKey(beginKey, batch, ctx);
}

void CheckerManagerAdapter::restart(const string& businessId, const string& namespaceId,
    const string& zkAddress, const Ice::Context& ctx) {
  getProxyOneway(0)->restart(businessId, namespaceId, zkAddress, ctx);
}

void CheckerManagerAdapter::statis(const string& businessId, const StatisDataPtr& data,
    const Ice::Context& ctx) {
  getProxy(0)->statis(businessId, data, ctx);
}
