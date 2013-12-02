#ifndef __XCE_ADAPTER_CHECKERMANAGERADAPTER_H__
#define __XCE_ADAPTER_CHECKERMANAGERADAPTER_H__

#include <TripodChecker.h>
#include <Channel.h>
#include <Singleton.h>
#include <AdapterI.h>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace checkermanager {

  using com::renren::tripod::CheckerManagerPrx;
  using com::renren::tripod::KeySeq;
  using com::renren::tripod::KeyMsgPtr;
  using com::renren::tripod::StatisDataPtr;

  class CheckerManagerAdapter : virtual public MyUtil::ReplicatedClusterAdapterI<CheckerManagerPrx>,
                                virtual public MyUtil::Singleton<CheckerManagerAdapter> {
  public:
    CheckerManagerAdapter() :
        MyUtil::ReplicatedClusterAdapterI<CheckerManagerPrx>("ControllerCheckerManager", 120, 300) {
    }

    KeyMsgPtr getKey(const string& beginKey, const int batch,
        const Ice::Context& ctx = Ice::Context());
    void restart(const string& businessId, const string& namespaceId, const string& zkAddress,
        const Ice::Context& ctx = Ice::Context());
    void statis(const string& businessId, const StatisDataPtr& data,
        const Ice::Context& ctx = Ice::Context());

  };

}}} //namespace xce::adapter::checermanager

#endif

