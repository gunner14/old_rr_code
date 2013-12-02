#ifndef RECOMMENDATION_FEED_EXPERIMENTI_H_
#define RECOMMENDATION_FEED_EXPERIMENTI_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class RecommendationFeedExperimentI : virtual public RecommendationFeedManager,
		virtual public MyUtil::Singleton<RecommendationFeedExperimentI> {
 public:
	RecommendationFeedExperimentI() {}
	
	RecommendItemSeq GetRecommendationByFeed(int, int, const Ice::Current&);          //recommendation in newsfeed

 private:
	friend class MyUtil::Singleton<RecommendationFeedExperimentI>;
};

}}

#endif
