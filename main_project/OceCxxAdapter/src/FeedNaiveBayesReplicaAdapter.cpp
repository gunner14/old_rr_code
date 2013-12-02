
#include "FeedNaiveBayesReplicaAdapter.h"

using namespace xce::feed;

FeedNaiveBayesPrx FeedNaiveBayesReplicaAdapter::getManager(int id)
{
  return getProxy(id);
}

FeedNaiveBayesPrx FeedNaiveBayesReplicaAdapter::getManagerOneway(int id)
{
  return getProxyOneway(id);
}

MyUtil::Int2IntMap FeedNaiveBayesReplicaAdapter::GetScoreDict(int fromid, const MyUtil::IntSeq & toids, int stype) {
	return getManager(fromid)->GetScoreDict(fromid, toids, stype);
}

MyUtil::Int2IntMap FeedNaiveBayesReplicaAdapter::GetScoreDictReverse(int toid, const MyUtil::IntSeq & fromids, int stype) {
	return getManager(toid)->GetScoreDictReverse(toid, fromids, stype);
}

int FeedNaiveBayesReplicaAdapter::GetSendFrequency(int fromid){
	return getManager(fromid)->GetSendFrequency(fromid);
}

