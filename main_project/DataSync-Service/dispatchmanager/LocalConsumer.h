#ifndef _LOCAL_CONSUMER_H__
#define _LOCAL_CONSUMER_H__

#include <ServiceI.h>
#include <Consumer.h>
#include "DispatchWorker.h"

namespace com {
namespace renren {
namespace datasync {

class LocalConsumer : public Ice::Object, public xce::messagepipe::Consumer {
    public:
        LocalConsumer(const std::string& zkAddress, const std::string cluster, 
                       const std::string& subject);

    public:

        virtual void onMessage(const xce::messagepipe::MessageDataSeq& receiveMessages);

    private:

        SyncMessageISeq getSyncMessage(const xce::messagepipe::MessageDataSeq& datas);
        xce::messagepipe::MessageDataSeq getMessageData(const SyncMessageISeq& datas);
};

typedef IceUtil::Handle<LocalConsumer> LocalConsumerPtr;

}
}
}

#endif
