
#ifndef __FEEDITEMWRAPPER_REPLICAADAPTER_H_
#define __FEEDITEMWRAPPER_REPLICAADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "Channel.h"
#include "UserFeeds.h"
#include "ReplicaCluster/ClusterState/src/client/cxx/ClientInterface.h"

namespace xce {
namespace feed {
    using namespace MyUtil;
    using namespace xce::feed;
    
    class FeedItemWrapperReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedItemWrapperPrx>,
            public MyUtil::Singleton<FeedItemWrapperReplicaAdapter> 
    {
    public:
        FeedItemWrapperReplicaAdapter() : 
            MyUtil::ReplicatedClusterAdapterI<FeedItemWrapperPrx>(
                "ControllerFeedItemWrapper", 120, 300, new XceFeedControllerChannel())
        {}
        
        FeedItemSeq getItemSeq(Ice::Int userid, Ice::Int begin, Ice::Int limit);

        FeedMetaSeq getLessThanFeedid(Ice::Int userid, Ice::Long feedid, Ice::Int limit);
        FeedMetaSeq get(int userid, int begin, int limit);

        FeedMetaSeq getLessThanFeedidByTypes(Ice::Int userid, Ice::Long feedid, Ice::Int limit, const MyUtil::IntSeq& types);
        FeedMetaSeq getByTypes(int userid, int begin, int limit, const MyUtil::IntSeq& types);
        FeedMetaSeq getByTypesWithFriends(int userid, const MyUtil::IntSeq & friends, int begin, int limit, const MyUtil::IntSeq& types);
        FeedMetaSeq GetFeeds(int userid, int begin, int limit, const MyUtil::IntSeq& types);
        FeedMetaSeq GetFeedsByGroupId(int userid, int id, int begin, int limit, const MyUtil::IntSeq& types);
        FeedMetaSeq GetFeedsByGroupName(int userid, const string& name, int begin, int limit, const MyUtil::IntSeq& types);
        FeedMetaSeq GetFeedsByFocus(int userid, int begin, int limit, const MyUtil::IntSeq& types);
        
        MyUtil::LongSeq GetFriendsFeedsList(int userid);

    protected:
        FeedItemWrapperPrx getManager(Ice::Int userid);
        FeedItemWrapperPrx getManagerOneway(Ice::Int userid);
    };
}
}
#endif /* __FEEDITEMWRAPPER_REPLICAADAPTER_H_ */
