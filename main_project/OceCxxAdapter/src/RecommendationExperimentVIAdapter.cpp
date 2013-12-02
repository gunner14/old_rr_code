#include "RecommendationExperimentVIAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentVIAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
