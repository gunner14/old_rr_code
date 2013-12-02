#ifndef __LOCALPRODUCERMANAGER_I_H__
#define __LOCALPRODUCERMANAGER_I_H__

#include <DataSync.h>
#include "ServiceI.h"

namespace com {
namespace renren {
namespace datasync {

const std::string LOCAL_PRODUCER_MANAGER = "M";
const std::string DEFAULT_ZKADDRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181";
const std::string DEFAULT_CLUSTER = "bj_test1";

class LocalProducerManagerI : virtual public LocalProducerManager, public MyUtil::Singleton<LocalProducerManagerI> {
public:
	
    virtual bool produce(const std::string& dbTable, const std::vector<unsigned char>& data, const Ice::Current& = Ice::Current());
    
    void init(const std::string& zkAddress, const std::string& cluster);

private:
    std::string zkAddress_;
    std::string cluster_;
};

}
}
}
#endif

