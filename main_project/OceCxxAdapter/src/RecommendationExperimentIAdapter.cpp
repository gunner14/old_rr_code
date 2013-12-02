#include "RecommendationExperimentIAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentIAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
