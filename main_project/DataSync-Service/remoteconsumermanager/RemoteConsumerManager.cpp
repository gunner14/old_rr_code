#include <activemq/library/ActiveMQCPP.h>
#include "RemoteConsumer.h"
#include "DataSync/share/ConfigHolder.hpp"
#include "IceExt/src/ServiceI.h"
#include "UtilCxx/src/LogWrapper.h"
using std::vector;
using std::map;
using std::string;
using com::renren::datasync::RemoteConsumer;
using com::renren::datasync::RemoteConsumerPtr;
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
          getPropertyWithDefault("RemoteConsumer.LocalZkAddress", DEFAULT_LOCAL_ZK_ADDRESS);
        localCluster_ = service.getCommunicator()->getProperties()->
          getPropertyWithDefault("RemoteConsumer.LocalCluster", DEFAULT_LOCAL_CLUSTER);
        remoteZkAddress_ = service.getCommunicator()->getProperties()->
          getPropertyWithDefault("RemoteConsumer.RemoteZkAddress", DEFAULT_REMOTE_ZK_ADDRESS);
        remoteCluster_ = service.getCommunicator()->getProperties()->
          getPropertyWithDefault("RemoteConsumer.RemoteCluster", DEFAULT_REMOTE_CLUSTER);
        MCE_INFO("RemoteConsumer.LocalZkAddress: " << localZkAddress_);
        MCE_INFO("RemoteConsumer.LocalCluster: " << localCluster_);
        MCE_INFO("RemoteConsumer.RemoteZkAddress: " << remoteZkAddress_);
        MCE_INFO("RemoteConsumer.RemoteCluster: " << remoteCluster_);

      }
      void run() {

        configHolder_.renew();
        configHolder_.start().detach();

        while (true) {
          vector<string> allSubjects = configHolder_.getAllSubject();
          for(vector<string>::const_iterator it = allSubjects.begin();
              it != allSubjects.end(); ++it) {
            map<string, RemoteConsumerPtr>::const_iterator findIt = consumers_.find(*it);
            if(findIt == consumers_.end()) {
              RemoteConsumerPtr remoteConsumer =  new RemoteConsumer(
                  remoteZkAddress_, remoteCluster_, *it, localZkAddress_, localCluster_);
              consumers_.insert(make_pair(*it, remoteConsumer));
            }
          }
          sleep(10);
        }

      }

    private:
      ConfigHolder configHolder_;
      map<string, RemoteConsumerPtr> consumers_;
      string localZkAddress_;
      string localCluster_;
      string remoteZkAddress_;
      string remoteCluster_;
      static const string DEFAULT_LOCAL_ZK_ADDRESS;
      static const string DEFAULT_REMOTE_ZK_ADDRESS;
      static const string DEFAULT_REMOTE_CLUSTER;
      static const string DEFAULT_LOCAL_CLUSTER;
  };
  const string Initializer::DEFAULT_LOCAL_ZK_ADDRESS
    = "10.22.206.33:2780,10.22.206.33:2781,10.22.206.33:2782";
  const string Initializer::DEFAULT_REMOTE_ZK_ADDRESS
    = "10.22.206.33:2780,10.22.206.33:2781,10.22.206.33:2782";
  const string Initializer::DEFAULT_REMOTE_CLUSTER = "bj_test2";
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
