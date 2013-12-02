#ifndef __LOCALPRODUCERMANAGER_I_H__
#define __LOCALPRODUCERMANAGER_I_H__

#include <DataSync.h>
#include "ServiceI.h"

namespace com {
namespace renren {
namespace datasync {

const std::string LOCAL_PRODUCER_MANAGER = "M";
const std::string DEFAULT_CLUSTER = "bj_test1";
const std::string DEFAULT_ZKADDRESS = "10.22.206.32:2780,10.22.206.32:2781,10.22.206.32:2782";
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

