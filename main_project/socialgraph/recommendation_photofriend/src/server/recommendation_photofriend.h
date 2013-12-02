#ifndef RECOMMENDATION_PHOTOFRIEND_H_
#define RECOMMENDATION_PHOTOFRIEND_H_

#include "RecommendationManager.h"

#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {


//-------------------------------------------------

class RecommendationPhotoFriend : virtual public RecommendationManager,
		virtual public MyUtil::Singleton<RecommendationPhotoFriend> {
 public:
	RecommendationPhotoFriend() {}
	
	virtual RecommendItemSeq GetFriendRecommendByHome(int, int, const Ice::Current&);

 private:
	friend class MyUtil::Singleton<RecommendationPhotoFriend>;
};

}}

#endif
