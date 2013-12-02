#ifndef FEED_DISPATCHER_NEW_ADAPTER_H_
#define FEED_DISPATCHER_NEW_ADAPTER_H_

#include <vector>
#include "Singleton.h"
#include "AdapterI.h"
#include "RFeed.h"


namespace xce {
namespace feed {
using namespace MyUtil;
class FeedDispatchNewReplicaAdapter: public MyUtil::ReplicatedClusterAdapterI<FeedDispatchNewPrx>
    , public MyUtil::Singleton<FeedDispatchNewReplicaAdapter>{
public:
	FeedDispatchNewReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<FeedDispatchNewPrx> ("ControllerFeedDispatchNewR",120,300,new XceFeedControllerChannel() ) {
	  }
	void AddBuddyNotify(int host, int guest);
	void JoinPageNotify(int page, int fans);
	void BuddyApplyNotify(int host, int guest);
  void GuideBuddyReplyNotify(int newUser, int buddy);
	void AddMiniSiteFeed(int siteid, int userid);
	void AddMiniSiteFeedToGuides(long source,int type,int siteid, const vector<int> & guides);
  void Reload(LoadType type, int id);
  void Reload2(LoadType type,long id);
  void ReloadBatch(LoadType type, const vector<int> & ids);
	void RemoveFeedByTime(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Int time);
	void RemoveFeedById(Ice::Long feedId);
	void RemoveFeedBySource(Ice::Long source, int stype);
	void RemoveFeedBySourceHS(Ice::Long source, int stype);
	void RemoveFeedByParent(Ice::Long parent, int ptype);
	void RemoveFeed(Ice::Long source, int stype, int actor);
	void RemoveFeedReply(Ice::Long source, int stype, int actor, Ice::Long removeReplyId, const FeedReplyPtr& reply);
	void RemoveFeedReplyById(int stype, Ice::Long feedid, Ice::Long removeReplyId, const FeedReplyPtr& reply);
	void Dispatch(const FeedSeedPtr& seed);
	void DispatchEDM(const FeedSeedPtr& seed);
	void DispatchWithReply(const FeedSeedPtr& seed, const FeedReplyPtr& reply);

	void AddFeedReply(Ice::Long source, int stype, int actor, const FeedReplyPtr & reply);
	void AddFeedReplyById(int stype, Ice::Long feedid, const FeedReplyPtr & reply);
  void AddFeedReplyWithConfig(Ice::Long source, Ice::Int stype,
      Ice::Int actor, const FeedReplyPtr& reply, const map<string,string> & reply_config);

	void CacheSync(Ice::Long source, int stype, int actor);
	Ice::Long GetFeedId(Ice::Long source, int stype, int actor);
	void DispatchAgain(Ice::Long source, int stype, int actor, int toid);
	void ReplaceXML(Ice::Long source, int stype, int actor,const string & xml );
	void DirectedSend(Ice::Long source, int stype, int actor,const vector<int> & targets);

protected:
	FeedDispatchNewPrx getManager(Ice::Long id);
	FeedDispatchNewPrx getManagerOneway(Ice::Long id);
};

}
}

#endif /*BUDDYCOREADAPTER_H_*/
