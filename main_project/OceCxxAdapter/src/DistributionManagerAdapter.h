#ifndef DISTRIBUTION_MANAGER_ADAPTER_H_
#define DISTRIBUTION_MANAGER_ADAPTER_H_

#include "DistributionManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class DistributionManagerAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::DistributionManagerPrx>,
  public MyUtil::Singleton<DistributionManagerAdapter> {
public:
  DistributionManagerAdapter():
    MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::DistributionManagerPrx>("ControllerDistributionManager", 120, 1000,
																																									new MyUtil::SocialGraphChannel()) {
  }

	RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);				//recommendation in home
	void AddRecommendationFeedAsync(int userid, int limit);					//recommendation in newsfeed

	void ReloadBlackAndWhiteFile(int userid);
};

}}


#endif
