#include "RecommendationExperimentVAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentVAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
