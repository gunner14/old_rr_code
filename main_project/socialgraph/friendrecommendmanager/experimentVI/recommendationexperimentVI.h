#ifndef RECOMMENDATION_EXPERIMENTVI_H_
#define RECOMMENDATION_EXPERIMENTVI_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

const static std::string EXPERIMENT = "E6";

//-------------------------------------------------

class RecommendationExperimentVI : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentVI> {
 public:
	RecommendationExperimentVI() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentVI>;
};

}}

#endif
