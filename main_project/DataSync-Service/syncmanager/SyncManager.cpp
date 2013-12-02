#include "SyncManager.h"
#include <activemq/library/ActiveMQCPP.h>
#include <util/cpp/InvokeMonitor.h>
#include <Monitor/client/service/ServiceMonitor.h>
#include <DataSync/share/ConfigHolder.hpp>


void MyUtil::initialize() {
        using namespace com::renren::datasync;
	MyUtil::ServiceI& service=MyUtil::ServiceI::instance(); 
	activemq::library::ActiveMQCPP::initializeLibrary();
	MyUtil::ServiceMonitorManager::instance().start();
	std::string zkAddress = service.getCommunicator()->getProperties()->getPropertyWithDefault("SyncManager.LocalZkAddress",LOCAL_ZK_ADDRESS);
        std::string cluster = service.getCommunicator()->getProperties()->getPropertyWithDefault("SyncManager.LocalCluster", LOCAL_CLUSTER);

	SyncManagerInitPtr initptr = new SyncManagerInit(zkAddress, cluster);
	initptr->start().detach();
}

namespace com {
    namespace renren {
	namespace datasync {
	    SyncManagerInit::SyncManagerInit(std::string localZkAddress, std::string localCluster) {
		zkAddress_ = localZkAddress;
		cluster_ = localCluster;
	    }

	    void SyncManagerInit::run() {
		std::map<std::string, com::renren::datasync::LocalConsumerPtr> consumers;
		com::renren::datasync::ConfigHolder configHolder;
		configHolder.renew();
		configHolder.start().detach();

		while(true) {
		std::vector<std::string> allSubjects = configHolder.getAllSubject();
		for(std::vector<std::string>::const_iterator it = allSubjects.begin(); it != allSubjects.end(); ++it) {
		    std::map<std::string, com::renren::datasync::LocalConsumerPtr>::const_iterator findIt = consumers.find(*it);
		    if(findIt == consumers.end()) {
			com::renren::datasync::LocalConsumerPtr localConsumer = new com::renren::datasync::LocalConsumer(zkAddress_, cluster_, *it);
			consumers.insert(make_pair(*it, localConsumer));
		    }
		}
		}
	    }
	}
    }

}
