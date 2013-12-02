#ifndef RECOMMENDATION_ONLINEI_H_
#define RECOMMENDATION_ONLINEI_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {

class RecommendationOnlineI : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationOnlineI> {
 public:
	RecommendationOnlineI() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationOnlineI>;
};

}}

#endif
