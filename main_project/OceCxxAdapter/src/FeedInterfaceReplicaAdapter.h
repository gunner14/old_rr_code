#ifndef FEED_INTERFACE_ADAPTER_H
#define FEED_INTERFACE_ADAPTER_H

#include "RFeed.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedInterfaceReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedDispatcherPrx>,
	public MyUtil::Singleton<FeedInterfaceReplicaAdapter>
{
public:
	FeedInterfaceReplicaAdapter():MyUtil::ReplicatedClusterAdapterI<FeedDispatcherPrx>("ControllerFeedDispatcherR",120,300,new XceFeedControllerChannel);

  void addFriendFeedToGuide(int userid,const MyUtil::LongSeq & feedids);
   
   FeedItemSeq GetOwnFeeds(const IntSeq & userids, int begin, int limit, const IntSeq & types);
   FeedContentDict GetFeedDict(const LongSeq & feedIds);
  

private:
  FeedInterfacePrx getManager(Ice::Long id);
  FeedInterfacePrx getManagerOneway(Ice::Long id);
};


};
};
#endif
