#ifndef _DISPATCHWORKER_H__
#define _DISPATCHWORKER_H__

#include <ServiceI.h>
#include <MessageData.h>
#include "VersionManager.h"
#include <ClientFactory.h> 
#include <DataSync/share/SyncMessageI.hpp>
#include <DataSync/share/ConfigHolder.hpp>
#include "tripod2/cc/core/tripod_client.h"
#include "boost/thread/mutex.hpp"
#include "boost/shared_ptr.hpp"
#include <set>

namespace com {
namespace renren {
namespace datasync {

const std::string TRIPOD_ZK_ADDRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/Tripod";

const int EXPIRED_TIME = 2147483647;

typedef boost::shared_ptr<xce::tripod::TripodClient> TripodClientPtr;
typedef std::map<std::pair<std::string, std::string>, TripodClientPtr > TripodClientMap;

class DispatchWorker : public MyUtil::Singleton<DispatchWorker> {
    public:
        DispatchWorker();

    public:
        SyncMessageISeq dispatch(const SyncMessageISeq& datas);

    private:
        SyncMessageISeq notify(const SyncMessageISeq& datas);
        com::renren::tripod::CacheManagerClientPtr getCacheMangerClient();
        TripodClientPtr getClient(const std::string &ns, const std::string &biz);

    private:
        VersionManager versionManager_;
        ConfigHolder configHolder_;
        IceUtil::Mutex mutex_;
        com::renren::tripod::CacheManagerClientPtr cm_;
        boost::shared_mutex clientMapMutex_;
        TripodClientMap clientMap_;
        std::set<std::string> nameSpaceFilter_;
};

}
}
}

#endif
