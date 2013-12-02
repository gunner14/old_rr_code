#ifndef RECOMMENDATION_EXPERIMENT_VI_ADAPTER_H_
#define RECOMMENDATION_EXPERIMENT_VI_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentVIAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>,
  public MyUtil::Singleton<RecommendationExperimentVIAdapter> {
public:
  RecommendationExperimentVIAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>("ControllerRecommendationExperimentVI", 120, 1000,
																																									new MyUtil::SocialGraphChannel()) {
  }

	RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);
};

}}


#endif
