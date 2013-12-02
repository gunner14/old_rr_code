#include <activemq/library/ActiveMQCPP.h>

#include "LocalConsumer.h"
#include <ServiceI.h>

const std::string LOCAL_ZK_ADDRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";
const std::string LOCAL_CLUSTER = "sh_test";

int main() {
    MCE_DEFAULT_INIT("WARN");
    activemq::library::ActiveMQCPP::initializeLibrary();
   
    com::xiaonei::xce::ConnectionPoolManager::instance().initialize();

    std::map<std::string, com::renren::datasync::LocalConsumerPtr> consumers;

    com::renren::datasync::ConfigHolder configHolder;
    configHolder.renew();
    configHolder.start().detach();

    while(true) {
        std::vector<std::string> allSubjects = configHolder.getAllSubject();
        for(std::vector<std::string>::const_iterator it = allSubjects.begin(); it != allSubjects.end(); ++it) {
            std::map<std::string, com::renren::datasync::LocalConsumerPtr>::const_iterator findIt = consumers.find(*it);
            if(findIt == consumers.end()) {
                com::renren::datasync::LocalConsumerPtr localConsumer = new com::renren::datasync::LocalConsumer(
                                                                              LOCAL_ZK_ADDRESS, LOCAL_CLUSTER, *it);
                consumers.insert(make_pair(*it, localConsumer));
            }
        }
        sleep(10);
    }
    return 1;
}
