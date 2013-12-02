#ifndef _REMOTE_CONSUMER_H__
#define _REMOTE_CONSUMER_H__

#include <ServiceI.h>
#include <Consumer.h>

namespace com {
namespace renren {
namespace datasync {

class RemoteConsumer : public Ice::Object, xce::messagepipe::Consumer {
    public:
        
        RemoteConsumer(const std::string& remoteZkAddress, const std::string cluster, 
                       const std::string& subject, const std::string& localZkAddress, const std::string& localCluster);

    public:

        virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages);

    private:
        std::string localZkAddress_;
        std::string localCluster_;
};

typedef IceUtil::Handle<RemoteConsumer> RemoteConsumerPtr;

}
}
}

#endif
