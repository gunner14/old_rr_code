#ifndef _REMOTE_PRODUCER_H__ 
#define _REMOTE_PRODUCER_H__ 

#include <ServiceI.h>
#include <DataSync/share/SyncMessageI.hpp>
#include <Producer.h>

namespace com {
namespace renren {
namespace datasync {

const std::string LOCAL_ZKADDRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";
const std::string REMOTE_CLUSTER = "bj_test2";

const int BATCH_SIZE = 100;

class RemoteProducer : public MyUtil::Singleton<RemoteProducer> {
    public:

        xce::messagepipe::MessageDataSeq send(const SyncMessageISeq& datas);

};

}
}
}

#endif
