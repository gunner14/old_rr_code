#include "RecommendationExperimentIVAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentIVAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
