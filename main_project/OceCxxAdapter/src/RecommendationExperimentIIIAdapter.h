#ifndef RECOMMENDATION_EXPERIMENT_III_ADAPTER_H_
#define RECOMMENDATION_EXPERIMENT_III_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentIIIAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>,
  public MyUtil::Singleton<RecommendationExperimentIIIAdapter> {
public:
  RecommendationExperimentIIIAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>("ControllerRecommendationExperimentIII", 120, 1000,
																																									new MyUtil::SocialGraphChannel()) {
  }

	RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);
};

}}


#endif
