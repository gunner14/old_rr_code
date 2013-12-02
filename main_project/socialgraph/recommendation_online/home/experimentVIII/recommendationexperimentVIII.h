#ifndef RECOMMENDATION_EXPERIMENTVIII_H_
#define RECOMMENDATION_EXPERIMENTVIII_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentVIII : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentVIII> {
 public:
	RecommendationExperimentVIII() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentVIII>;
};

}}

#endif
