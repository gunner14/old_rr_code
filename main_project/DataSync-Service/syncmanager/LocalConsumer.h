#ifndef _LOCAL_CONSUMER_H__
#define _LOCAL_CONSUMER_H__

#include <ServiceI.h>
#include <Consumer.h>
#include "SyncWorker.h"

namespace com {
namespace renren {
namespace datasync {

class LocalConsumer : public Ice::Object, xce::messagepipe::Consumer {
    public:
        LocalConsumer(const std::string& zkAddress, const std::string cluster, 
                       const std::string& subject);

    public:

        virtual void onMessage(const std::vector<xce::messagepipe::MessageDataPtr>& receiveMessages);
};

typedef IceUtil::Handle<LocalConsumer> LocalConsumerPtr;

}
}
}

#endif
