#ifndef RECOMMENDATION_EXPERIMENTIV_H_
#define RECOMMENDATION_EXPERIMENTIV_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

const static std::string EXPERIMENT = "E4";

//-------------------------------------------------

class RecommendationExperimentIV : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentIV> {
 public:
	RecommendationExperimentIV() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentIV>;
};

}}

#endif
