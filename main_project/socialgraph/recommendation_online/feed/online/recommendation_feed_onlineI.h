#ifndef RECOMMENDATION_FEED_ONLINEI_H_
#define RECOMMENDATION_FEED_ONLINEI_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class RecommendationFeedOnlineI : virtual public RecommendationFeedManager,
		virtual public MyUtil::Singleton<RecommendationFeedOnlineI> {
 public:
	RecommendationFeedOnlineI() {}
	
	virtual RecommendItemSeq GetRecommendationByFeed(int, int, const Ice::Current&);					//recommendation in newsfeed

 private:
	friend class MyUtil::Singleton<RecommendationFeedOnlineI>;
};

}}

#endif
