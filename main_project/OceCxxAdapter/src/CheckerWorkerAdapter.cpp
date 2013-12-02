#include "CheckerWorkerAdapter.h"
using namespace xce::adapter::checkerworker;

void CheckerWorkerAdapter::start(const string& businessId, const string& namespaceId,
    const string& zkAddress, const Ice::Context& ctx) {
  vector<CheckerWorkerPrx> proxies = getAllProxySeq(0);
  MCE_DEBUG("CheckerWorkerAdapter::start proxies size=" << proxies.size());
  for(vector<CheckerWorkerPrx>::const_iterator it = proxies.begin(); it != proxies.end(); ++it) {
    try {
      (*it)->start(businessId, namespaceId, zkAddress, ctx);
    } catch(Ice::Exception& e) {
      MCE_WARN("CheckerWorkerAdapter::start Exception " << e.what());
    }
  }
}

void CheckerWorkerAdapter::stop(const string& businessId, const Ice::Context& ctx) {
  vector<CheckerWorkerPrx> proxies = getAllProxySeq(0);
  for(vector<CheckerWorkerPrx>::const_iterator it = proxies.begin(); it != proxies.end(); ++it) {
    try {
      (*it)->stop(businessId, ctx);
    } catch(Ice::Exception& e) {
      MCE_WARN("CheckerWorkerAdapter::stop Exception " << e.what());
    }
  }
}

//void CheckerWorkerAdapter::statis(const string& businessId, const StatisDataPtr& data, const Ice::Context& ctx) {
//  getProxy(0)->statis(businessId, data, ctx)
//}
