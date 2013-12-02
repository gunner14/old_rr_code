#ifndef RECOMMENDATION_EXPERIMENT_VV_ADAPTER_H_
#define RECOMMENDATION_EXPERIMENT_VV_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentVVAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>,
  public MyUtil::Singleton<RecommendationExperimentVVAdapter> {
public:
  RecommendationExperimentVVAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>("ControllerRecommendationExperimentVV", 120, 1000,


                            new MyUtil::SocialGraphChannel()) {
  }

        RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);
};

}}


#endif

