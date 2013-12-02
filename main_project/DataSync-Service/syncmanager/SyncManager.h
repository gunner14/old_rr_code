#ifndef _SYNCMANAGER_H
#define _SYNCMANAGER_H

#include "LocalConsumer.h"
#include <ServiceI.h>

namespace com
{
    namespace renren
    {
	namespace datasync
	{
            const std::string LOCAL_ZK_ADDRESS = "10.22.206.32:2780,10.22.206.32:2781,10.22.206.32:2782";
	    const std::string LOCAL_CLUSTER = "bj_test1";
	    class SyncManagerInit : public IceUtil::Thread {
		public :
		    SyncManagerInit(std::string localZkAddress, std::string localCluster); 
		    void run();
		private:
		    std::string zkAddress_;
		    std::string cluster_;
	    };
	    typedef IceUtil::Handle<SyncManagerInit> SyncManagerInitPtr;
	}
    }
}

#endif
