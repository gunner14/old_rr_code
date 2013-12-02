#ifndef RECOMMENDATION_EXPERIMENT_VIIII_ADAPTER_H_
#define RECOMMENDATION_EXPERIMENT_VIIII_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentVIIIIAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>,
  public MyUtil::Singleton<RecommendationExperimentVIIIIAdapter> {
public:
  RecommendationExperimentVIIIIAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>("ControllerRecommendationExperimentVIIII", 120, 1000,


                            new MyUtil::SocialGraphChannel()) {
  }

        RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);
};

}}


#endif

