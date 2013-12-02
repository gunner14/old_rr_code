#ifndef FEED_NAIVE_ADAPTERR_H
#define FEED_NAIVE_ADAPTERR_H

#include "FeedNaiveBayes.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedNaiveBayesAdapter : public AdapterI,
	public AdapterISingleton<MyUtil::XceFeedChannel, FeedNaiveBayesAdapter>
{

public:
	FeedNaiveBayesAdapter() {
 		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1); 
		_managersBatchOneway.resize(cluster() > 0 ? cluster() : 1); 
  };

	MyUtil::Int2IntMap GetScoreDict(int fromid,const MyUtil::IntSeq & toids, int stype){
		return getManager(fromid)->GetScoreDict(fromid,  toids,stype);
	}

  int UpdateScoreBatch(const MyUtil::IntSeq & useridids, const MyUtil::IntSeq & featureids, const FloatSeq & scores);

protected:
	virtual string name() {
		return "FeedNaiveBayes";
	}
	virtual string endpoints() {
		return "@FeedNaiveBayes";
	}
	virtual size_t cluster() {
		return 2;
	}
private:

	FeedNaiveBayesPrx getManager(int id);
	FeedNaiveBayesPrx getManagerOneway(int id);
	FeedNaiveBayesPrx getManagerBatchOneway(int id);

	vector<FeedNaiveBayesPrx> _managersBatchOneway;
	vector<FeedNaiveBayesPrx> _managersOneway;
	vector<FeedNaiveBayesPrx> _managers;
};


};
};
#endif
