#ifndef RECOMMENDATION_EXPERIMENTIII_H_
#define RECOMMENDATION_EXPERIMENTIII_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

const static std::string EXPERIMENT = "E4";

//-------------------------------------------------

class RecommendationExperimentIII : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationExperimentIII> {
 public:
	RecommendationExperimentIII() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationExperimentIII>;
};

}}

#endif
