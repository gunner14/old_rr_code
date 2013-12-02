#include "LocalProducerManagerI.h"
#include <util/cpp/InvokeMonitor.h>
#include <Monitor/client/service/ServiceMonitor.h>
#include <ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h>

#include <Producer.h>

void MyUtil::initialize() {
    activemq::library::ActiveMQCPP::initializeLibrary();
    MyUtil::ServiceI& service=MyUtil::ServiceI::instance(); 
    service.getAdapter()->add(&com::renren::datasync::LocalProducerManagerI::instance(), service.createIdentity(
			com::renren::datasync::LOCAL_PRODUCER_MANAGER, ""));

    int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("LocalProducerManagerXhy.Mod");
    int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("LocalProducerManagerXhy.Interval",5);
    xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerLocalProducerManagerXhy",&com::renren::datasync::LocalProducerManagerI::instance(),mod,interval);
    MyUtil::ServiceMonitorManager::instance().start();
    
    std::string zkAddress = service.getCommunicator()->getProperties()->getPropertyWithDefault("LocalProducerManagerXhy.ZkAddress", com::renren::datasync::DEFAULT_ZKADDRESS);
    std::string cluster = service.getCommunicator()->getProperties()->getPropertyWithDefault("LocalProducerManagerXhy.Cluster", com::renren::datasync::DEFAULT_CLUSTER);
    com::renren::datasync::LocalProducerManagerI::instance().init(zkAddress, cluster);
}


namespace com {
namespace renren {
namespace datasync {

void LocalProducerManagerI::init(const std::string& zkAddress, const std::string& cluster) {
    zkAddress_ = zkAddress;
    cluster_ = cluster;
}

bool LocalProducerManagerI::produce(const std::string& dbTable, const std::vector<unsigned char>& data, const Ice::Current& current) {
    std::ostringstream os;
    os <<" table " << dbTable << " size "<<data.size();
    MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, os.str(), MyUtil::InvokeClient::INVOKE_LEVEL_DEBUG);
    
    xce::messagepipe::MessageDataPtr msg(new xce::messagepipe::MessageData(cluster_, dbTable, data));

    bool flag = xce::messagepipe::Producer::getInstance()->send(zkAddress_, msg);

    os << " flag " << flag;

    MCE_DEBUG(os.str());

    return flag;
}

}
}
}
