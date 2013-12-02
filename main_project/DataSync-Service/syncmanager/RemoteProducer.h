#ifndef _REMOTE_PRODUCER_H__ 
#define _REMOTE_PRODUCER_H__ 

#include <ServiceI.h>
#include <DataSync/share/SyncMessageI.hpp>
#include <Producer.h>

namespace com {
namespace renren {
namespace datasync {

const std::string LOCAL_ZKADDRESS = "10.22.206.33:2780,10.22.206.33:2781,10.22.206.33:2782";
const std::string REMOTE_CLUSTER = "bj_test2";

const int BATCH_SIZE = 100;

class RemoteProducer : public MyUtil::Singleton<RemoteProducer> {
    public:
        RemoteProducer() {
	    MyUtil::ServiceI& service = MyUtil::ServiceI::instance();
	    ZkAddress_ = service.getCommunicator()->getProperties()->getPropertyWithDefault("SyncManager.RemoteZkAddress",LOCAL_ZKADDRESS);
	    Cluster_ = service.getCommunicator()->getProperties()->getPropertyWithDefault("SyncManager.RemoteCluster",REMOTE_CLUSTER);
	}

        xce::messagepipe::MessageDataSeq send(const SyncMessageISeq& datas);
    private:
	std::string ZkAddress_;
	std::string Cluster_;
};

}
}
}

#endif
