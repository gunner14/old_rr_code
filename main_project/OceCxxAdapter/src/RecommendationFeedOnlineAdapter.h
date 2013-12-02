#ifndef RECOMMENDATION_FEED_ONLINE_ADAPTER_H_
#define RECOMMENDATION_FEED_ONLINE_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationFeedOnlineAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationFeedManagerPrx>,
  public MyUtil::Singleton<RecommendationFeedOnlineAdapter> {
public:
  RecommendationFeedOnlineAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationFeedManagerPrx>("ControllerRecommendationFeedOnline", 120, 1000,
																																									new MyUtil::SocialGraphChannel()) {
  }

	RecommendItemSeq GetRecommendationByFeed(int userid, int limit);
};

}}


#endif
