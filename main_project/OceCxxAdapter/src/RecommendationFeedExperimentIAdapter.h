#ifndef RECOMMENDATION_FEED_EXPERIMENTI_ADAPTER_H_
#define RECOMMENDATION_FEED_EXPERIMENTI_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationFeedExperimentIAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationFeedManagerPrx>,
  public MyUtil::Singleton<RecommendationFeedExperimentIAdapter> {
public:
  RecommendationFeedExperimentIAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationFeedManagerPrx>("ControllerRecommendationFeedExperimentI", 120, 1000,
																																									new MyUtil::SocialGraphChannel()) {
  }

	RecommendItemSeq GetRecommendationByFeed(int userid, int limit);
};

}}


#endif
