#ifndef RECOMMENDATION_EXPERIMENTI_H_
#define RECOMMENDATION_EXPERIMENTI_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

const static std::string EXPERIMENT = "E2";

//-----------------------------------------------------

class RecommendationExperimentI : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentI> {
 public:
	RecommendationExperimentI() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentI>;
};

}}

#endif
