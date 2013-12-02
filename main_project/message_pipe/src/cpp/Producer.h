#ifndef _MP_PRODUCER_H__
#define _MP_PRODUCER_H__


#include "Common.h"
#include "Singleton.h"
#include "MessageData.h"
#include "ClusterManager.h"

namespace xce{
namespace messagepipe{

class Producer : public Singleton<Producer>{

public:
	bool send(const std::string& zk_address, const MessageDataPtr& producerData);

	void sendBatch(const std::string& zk_address, const std::vector<MessageDataPtr>& producerDatas); 


};

}
}
#endif
