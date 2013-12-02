
#ifndef __FEEDITEMWRAPPER_CACHE_ADAPTER_H_
#define __FEEDITEMWRAPPER_CACHE_ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "Channel.h"
#include "FeedItemBlockCache.h"

namespace xce {
namespace feed {
    using namespace MyUtil;
    
    class FeedItemBlockCacheAdapter : public MyUtil::AdapterI,
            public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedItemBlockCacheAdapter> 
    {
    public:
        FeedItemBlockCacheAdapter();
        IntSeq getUserBlockTypes(Ice::Int userid);
        void addUserBlockType(Ice::Int userid, Ice::Int type);
        void removeUserBlockType(Ice::Int userid, Ice::Int type);
        bool hasUserBlockType(Ice::Int userid, Ice::Int type);
        
        IntSeq getUserBlockUsers(Ice::Int userid);
        void addUserBlockUser(Ice::Int userid, Ice::Int block_userid);
        void removeUserBlockUser(Ice::Int userid, Ice::Int block_userid);
        bool hasUserBlockUser(Ice::Int userid, Ice::Int block_userid);
        
        LongSeq getUserBlockFeeds(Ice::Int userid);
        void addUserBlockFeed(Ice::Int userid, Ice::Long feedid);
        void removeUserBlockFeed(Ice::Int userid, Ice::Long feedid);
        bool hasUserBlockFeed(Ice::Int userid, Ice::Long feedid);

        UserBlockInfoPtr getAll(Ice::Int userid);

        FeedItemBlockCachePrx getManager(Ice::Int userid);
        FeedItemBlockCachePrx getManagerOneway(Ice::Int userid);
    protected:
        virtual string name() { return "FeedItemBlockCache"; }
        virtual string endpoints() { return "@FeedItemBlockCache"; }
        virtual size_t cluster() { return 1; }

        vector<FeedItemBlockCachePrx> _managersOneway;
        vector<FeedItemBlockCachePrx> _managers;
    };
    typedef FeedItemBlockCacheAdapter* FeedItemBlockCacheAdapterPtr;
}
}
#endif /* __FEEDITEMWRAPPER_CACHE_ADAPTER_H_ */
