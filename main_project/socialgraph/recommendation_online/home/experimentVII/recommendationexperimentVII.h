#ifndef RECOMMENDATION_EXPERIMENTVII_H_
#define RECOMMENDATION_EXPERIMENTVII_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentVII : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentVII> {
 public:
	RecommendationExperimentVII() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentVII>;
};

}}

#endif
