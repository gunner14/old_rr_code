#ifndef __XCE_ADAPTER_CHECKERWORKERADAPTER_H__
#define __XCE_ADAPTER_CHECKERWORKERADAPTER_H__

#include <TripodChecker.h>
#include <Channel.h>
#include <Singleton.h>
#include <AdapterI.h>

namespace xce {
namespace adapter {
namespace checkerworker {

  using com::renren::tripod::CheckerWorkerPrx;
  using com::renren::tripod::KeySeq;
  using com::renren::tripod::KeyMsgPtr;

  class CheckerWorkerAdapter : virtual public MyUtil::ReplicatedClusterAdapterI<CheckerWorkerPrx>,
                                virtual public MyUtil::Singleton<CheckerWorkerAdapter> {
  public:
    CheckerWorkerAdapter() :
        MyUtil::ReplicatedClusterAdapterI<CheckerWorkerPrx>("ControllerCheckerWorker", 120, 300) {
    }

    void start(const string& businessId, const string& namespaceId, const string& zkAddress,
        const Ice::Context& ctx = Ice::Context());
    void stop(const string& businessId, const Ice::Context& ctx = Ice::Context());
    //void statis(const string& businessId, const StatisDataPtr& data, const Ice::Context& ctx = Ice::Context());

  };

}}} //namespace xce::adapter::checermanager

#endif

