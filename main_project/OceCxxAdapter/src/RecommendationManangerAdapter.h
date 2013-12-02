#ifndef RECOMMENDATION_MANAGER_ADAPTER_H_
#define RECOMMENDATION_MANAGER_ADAPTER_H_

#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class RecommendationManangerAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>,
  public MyUtil::Singleton<RecommendationManangerAdapter> {
public:
  RecommendationManangerAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>("ControllerRecommendationManager", 120, 1000,
																																									new MyUtil::SocialGraphChannel()) {
  }

	RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);
};

}}


#endif
