#ifndef RECOMMENDATION_EXPERIMENTVIIIII_H_
#define RECOMMENDATION_EXPERIMENTVIIIII_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class RecommendationExperimentVIIII : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentVIIII> {
 public:
	RecommendationExperimentVIIII() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentVIIII>;
};

}}

#endif
