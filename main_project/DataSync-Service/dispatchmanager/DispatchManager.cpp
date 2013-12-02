#include <activemq/library/ActiveMQCPP.h>
#include "LocalConsumer.h"
#include "IceExt/src/ServiceI.h"
#include "UtilCxx/src/LogWrapper.h"
using std::vector; using std::string;
using std::map;
using com::renren::datasync::LocalConsumer;
using com::renren::datasync::LocalConsumerPtr;
using com::renren::datasync::ConfigHolder;

namespace com {
namespace renren {
namespace datasync {
  class Initializer : public IceUtil::Thread {
    public:
      Initializer() {
        activemq::library::ActiveMQCPP::initializeLibrary();
        MyUtil::ServiceI  &service = MyUtil::ServiceI::instance();

        localZkAddress_ = service.getCommunicator()->getProperties()->
          getPropertyWithDefault("DispatchManager.ZkAddress", DEFAULT_LOCAL_ZK_ADDRESS);
        localCluster_ = service.getCommunicator()->getProperties()->
          getPropertyWithDefault("DispatchManager.Cluster", DEFAULT_LOCAL_CLUSTER);
        MCE_INFO("DispatchManager.ZkAddress: " << localZkAddress_);
        MCE_INFO("DispatchManager.Cluster: " << localCluster_);
      }
      void run() {
        configHolder_.renew();
        configHolder_.start().detach();

        while (true) {
          vector<string> allSubjects = configHolder_.getAllSubject();
          for(vector<string>::const_iterator it = allSubjects.begin();
              it != allSubjects.end(); ++it) {
            map<string, LocalConsumerPtr>::const_iterator findIt = consumers_.find(*it);
            if(findIt == consumers_.end()) {
              LocalConsumerPtr localConsumer = new LocalConsumer(
                  localZkAddress_, localCluster_, *it);
              consumers_.insert(make_pair(*it, localConsumer));
            }
          }
          sleep(10);
        }

      }

    private:
      ConfigHolder configHolder_;
      map<string, LocalConsumerPtr> consumers_;
      string localZkAddress_;
      string localCluster_;
      static const string DEFAULT_LOCAL_ZK_ADDRESS;
      static const string DEFAULT_LOCAL_CLUSTER;
  };
  const string Initializer::DEFAULT_LOCAL_ZK_ADDRESS
    = "10.22.206.33:2780,10.22.206.33:2781,10.22.206.33:2782";
  const string Initializer::DEFAULT_LOCAL_CLUSTER = "sh_test";

  typedef IceUtil::Handle<Initializer> InitializerPtr;
} /* namespace datasync */
} /* namespace renren */
} /* namespace com */
using com::renren::datasync::Initializer;
using com::renren::datasync::InitializerPtr;

void MyUtil::initialize() {
  InitializerPtr initializer = new Initializer;
  initializer->start().detach();
}
