#ifndef FEED_NAIVEBAYES_REPLICA_ADAPTER_H_
#define FEED_NAIVEBAYES_REPLICA_ADAPTER_H_

#include "FeedNaiveBayes.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedNaiveBayesReplicaAdapter :
	public MyUtil::ReplicatedClusterAdapterI<FeedNaiveBayesPrx>,
	public MyUtil::Singleton<FeedNaiveBayesReplicaAdapter>
{
public:
	FeedNaiveBayesReplicaAdapter(): MyUtil::ReplicatedClusterAdapterI <FeedNaiveBayesPrx> ("ControllerFeedNaiveBayesR", 120, 300, new XceFeedControllerChannel()) {
  }

	MyUtil::Int2IntMap GetScoreDict(int fromid, const MyUtil::IntSeq & toids, int stype);
	MyUtil::Int2IntMap GetScoreDictReverse(int toid, const MyUtil::IntSeq & fromids, int stype);
	int GetSendFrequency(int fromid);

protected:
	FeedNaiveBayesPrx getManager(int id);
	FeedNaiveBayesPrx getManagerOneway(int id);
};


};
};
#endif
