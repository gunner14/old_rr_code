
#ifndef __FEEDITEMWRAPPER_CACHE_REPLICAADAPTER_H_
#define __FEEDITEMWRAPPER_CACHE_REPLICAADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "Channel.h"
#include "FeedItemBlockCache.h"
#include "ReplicaCluster/ClusterState/src/client/cxx/ClientInterface.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "BuddyByOnlineTimeReplicaAdapter.h"
#include "BuddyByIdReplicaAdapter.h"

namespace xce {
namespace feed {
    using namespace MyUtil;
    
    class FeedItemBlockCacheReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedItemBlockCachePrx>,
            public MyUtil::Singleton<FeedItemBlockCacheReplicaAdapter> 
    {
    public:
        FeedItemBlockCacheReplicaAdapter() : 
            MyUtil::ReplicatedClusterAdapterI<FeedItemBlockCachePrx>(
                "ControllerFeedItemBlockCache", 120, 300, new XceFeedControllerChannel())
        {}

		MyUtil::IntSeq getUserBlockTypes(Ice::Int userid);
        void addUserBlockType(Ice::Int userid, Ice::Int type);
        void removeUserBlockType(Ice::Int userid, Ice::Int type);
        bool hasUserBlockType(Ice::Int userid, Ice::Int type);
        
		MyUtil::IntSeq getUserBlockUsers(Ice::Int userid);
        void addUserBlockUser(Ice::Int userid, Ice::Int block_userid);
        void removeUserBlockUser(Ice::Int userid, Ice::Int block_userid);
        bool hasUserBlockUser(Ice::Int userid, Ice::Int block_userid);
        
        LongSeq getUserBlockFeeds(Ice::Int userid);
        void addUserBlockFeed(Ice::Int userid, Ice::Long feedid);
        void removeUserBlockFeed(Ice::Int userid, Ice::Long feedid);
        bool hasUserBlockFeed(Ice::Int userid, Ice::Long feedid);

        void readAllFeeds(Ice::Int userid);

        UserBlockInfoPtr getAll(Ice::Int userid);

        // reload
        void reloadUserBlockTypes(Ice::Int userid);
        void reloadUserBlockUsers(Ice::Int userid);
        void reloadUserBlockFeeds(Ice::Int userid);
        void reloadAll(Ice::Int userid);

    protected:
        FeedItemBlockCachePrx getManager(Ice::Int userid);
        FeedItemBlockCachePrx getManagerOneway(Ice::Int userid);
    };
}
}
#endif /* __FEEDITEMWRAPPER_CACHE_REPLICAADAPTER_H_ */
