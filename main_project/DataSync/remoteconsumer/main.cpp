#include <activemq/library/ActiveMQCPP.h>

#include "RemoteConsumer.h"
#include <DataSync/share/ConfigHolder.hpp>
#include <ServiceI.h>

const std::string LOCAL_ZK_ADDRESS = "UserZooKeeper1.n.xiaonei.com:2181,UserZooKeeper2.n.xiaonei.com:2181,UserZooKeeper3.n.xiaonei.com:2181,UserZooKeeper4.n.xiaonei.com:2181,UserZooKeeper5.n.xiaonei.com:2181";
const std::string REMOTE_ZK_ADDRESS = "UserZooKeeper1.d.xiaonei.com:2181,UserZooKeeper2.d.xiaonei.com:2181,UserZooKeeper3.d.xiaonei.com:2181,UserZooKeeper4.d.xiaonei.com:2181,UserZooKeeper5.d.xiaonei.com:2181";
const std::string REMOTE_CLUSTER = "bj_test2";
const std::string LOCAL_CLUSTER = "sh_test";

int main() {
    MCE_DEFAULT_INIT("WARN");
    activemq::library::ActiveMQCPP::initializeLibrary();
    
    com::xiaonei::xce::ConnectionPoolManager::instance().initialize();

    std::map<std::string, com::renren::datasync::RemoteConsumerPtr> consumers;

    com::renren::datasync::ConfigHolder configHolder;
    configHolder.renew();
    configHolder.start().detach();

    while(true) {
        std::vector<std::string> allSubjects = configHolder.getAllSubject();
        for(std::vector<std::string>::const_iterator it = allSubjects.begin(); it != allSubjects.end(); ++it) {
            std::map<std::string, com::renren::datasync::RemoteConsumerPtr>::const_iterator findIt = consumers.find(*it);
            if(findIt == consumers.end()) {
                com::renren::datasync::RemoteConsumerPtr remoteConsumer = new com::renren::datasync::RemoteConsumer(
                                                                              REMOTE_ZK_ADDRESS, REMOTE_CLUSTER, *it, LOCAL_ZK_ADDRESS, LOCAL_CLUSTER);
                consumers.insert(make_pair(*it, remoteConsumer));
            }
        }
        sleep(10);
    }
    return 1;
}
