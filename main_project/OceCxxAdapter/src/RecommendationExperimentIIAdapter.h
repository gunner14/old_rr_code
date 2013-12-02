#ifndef RECOMMENDATION_EXPERIMENT_II_ADAPTER_H_
#define RECOMMENDATION_EXPERIMENT_II_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentIIAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>,
  public MyUtil::Singleton<RecommendationExperimentIIAdapter> {
public:
  RecommendationExperimentIIAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>("ControllerRecommendationExperimentII", 120, 1000,
																																									new MyUtil::SocialGraphChannel()) {
  }

	RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);
};

}}


#endif
