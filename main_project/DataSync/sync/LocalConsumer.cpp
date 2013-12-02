#include "LocalConsumer.h"
#include <Producer.h>

namespace com {
namespace renren {
namespace datasync {

LocalConsumer::LocalConsumer(const std::string& zkAddress, const std::string cluster, 
                             const std::string& subject) :
                             xce::messagepipe::Consumer(zkAddress, cluster, subject) {
}

void LocalConsumer::onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages) {
    std::vector<xce::messagepipe::MessageDataPtr> failMessages = SyncWorker::instance().sync(receiveMessages);
    if(!failMessages.empty()) {
        xce::messagepipe::Producer::getInstance()->sendBatch(zk_address_, failMessages);
    }
}

}
}
}
