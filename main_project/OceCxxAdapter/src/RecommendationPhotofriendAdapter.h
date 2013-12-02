#ifndef _RECOMMENDATION_PHOTOFRIEND_H_
#define _RECOMMENDATION_PHOTOFRIEND_H_
#include "RecommendationManager.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"


namespace xce {
namespace socialgraph {

class RecommendationPhotofriendAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>,
  					public MyUtil::Singleton<RecommendationPhotofriendAdapter> {
public:
  	RecommendationPhotofriendAdapter():
	MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::RecommendationManagerPrx>("ControllerRecommendationPhotofriend", 120, 1000, new MyUtil::SocialGraphChannel()) {
  }
	RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);
	virtual ~RecommendationPhotofriendAdapter(){}
};
}}




#endif

