#ifndef GUIDE_RECOMMEND_CONTENT_ADAPTER_H_
#define GUIDE_RECOMMEND_CONTENT_ADAPTER_H_

#include "GuideRecommendContent.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace socialgraph {

class GuideRecommendContentAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::GuideRecommendContentPrx>,
  public MyUtil::Singleton<GuideRecommendContentAdapter> {
public:
	GuideRecommendContentAdapter(): 
		MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::GuideRecommendContentPrx>("ControllerGuideRecommendContent", 120, 1000, 
																																									new MyUtil::SocialGraphChannel()) { 
	}

	void TriggerContentNewsFeed(int userid, int friendid);
};

}}

#endif
