#ifndef RECOMMENDATION_EXPERIMENTVV_H_
#define RECOMMENDATION_EXPERIMENTVV_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentVV : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentVV> {
 public:
	RecommendationExperimentVV() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentVV>;
};

}}

#endif
