#include "RemoteConsumer.h"
#include <Producer.h>

namespace com {
namespace renren {
namespace datasync {

RemoteConsumer::RemoteConsumer(const std::string& zkAddress, const std::string cluster, 
                             const std::string& subject, const std::string& localZkAddress, const std::string& localCluster) :
                             xce::messagepipe::Consumer(zkAddress, cluster, subject) {
    localZkAddress_ = localZkAddress;
    localCluster_ = localCluster;
}

void RemoteConsumer::onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {

    std::vector<xce::messagepipe::MessageDataPtr> failMessages;
    for(xce::messagepipe::MessageDataSeq::const_iterator it = receiveMessages.begin(); it != receiveMessages.end(); ++it) {
        std::string cluster = (*it)->getCluster();
        (*it)->setCluster(localCluster_);
        if(!xce::messagepipe::Producer::getInstance()->send(localZkAddress_, *it)) {
            (*it)->setCluster(cluster);
            failMessages.push_back(*it);
        }
    }
    
    if(!failMessages.empty()) {
        xce::messagepipe::Producer::getInstance()->sendBatch(zk_address_, failMessages);
    }
}

}
}
}
