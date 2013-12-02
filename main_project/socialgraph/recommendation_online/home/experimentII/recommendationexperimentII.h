#ifndef RECOMMENDATION_EXPERIMENTII_H_
#define RECOMMENDATION_EXPERIMENTII_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

const static std::string EXPERIMENT = "E3";

//---------------------------------------------------------

class RecommendationExperimentII : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentII> {
 public:
	RecommendationExperimentII() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentII>;
};

}}

#endif
