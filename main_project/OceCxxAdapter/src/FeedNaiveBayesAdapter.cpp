#include "FeedNaiveBayesAdapter.h"

using namespace MyUtil;
using namespace xce::feed;
FeedNaiveBayesPrx FeedNaiveBayesAdapter::getManager(int id) {
	return locate<FeedNaiveBayesPrx> (_managers, "M", id, TWO_WAY);
}

FeedNaiveBayesPrx FeedNaiveBayesAdapter::getManagerOneway(int id) {
	return locate<FeedNaiveBayesPrx> (_managersOneway, "M", id, ONE_WAY);
}

FeedNaiveBayesPrx FeedNaiveBayesAdapter::getManagerBatchOneway(int id) {
	return locate<FeedNaiveBayesPrx> (_managersBatchOneway, "M", id, BATCH_ONE_WAY);
}

int FeedNaiveBayesAdapter::UpdateScoreBatch(const MyUtil::IntSeq & userids, const MyUtil::IntSeq & featureids, const FloatSeq & scores) {
   return getManager(0)->UpdateScoreBatch(userids, featureids, scores);
   //getManagerOneway(0)->UpdateScoreBatch(froms, tos, scores);
   //getManagerBatchOneway(0)->UpdateScoreBatch(froms, tos, scores);
   //getManagerBatchOneway(1)->UpdateScoreBatch(from, tos, scores);
}









