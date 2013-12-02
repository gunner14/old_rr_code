#ifndef RECOMMENDATION_EXPERIMENTV_H_
#define RECOMMENDATION_EXPERIMENTV_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

const static std::string EXPERIMENT = "E5";

//-------------------------------------------------

class RecommendationExperimentV : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentV> {
 public:
	RecommendationExperimentV() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentV>;
};

}}

#endif
