
#include "Producer.h"

namespace xce{
namespace messagepipe{

bool Producer::send(const std::string& zk_address, const MessageDataPtr& producerData) {
	if(producerData->getCluster().empty() || producerData->getSubject().empty() || producerData->getData().empty()){
		return false;
	}
	NodeSetPtr nodes = ClusterManager::getInstance()->getConnection(zk_address,producerData->getCluster());
	if (nodes != NULL) {
		return nodes->send(producerData);
	} else {
		return false;
	}
}

void Producer::sendBatch(const std::string& zk_address, const std::vector<MessageDataPtr>& producerDatas) {
	for (std::vector<MessageDataPtr>::const_iterator it=producerDatas.begin();it!=producerDatas.end();++it) {
		send(zk_address, *it);
	}
}

}
}
