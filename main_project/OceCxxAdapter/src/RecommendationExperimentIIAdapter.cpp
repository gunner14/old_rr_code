#include "RecommendationExperimentIIAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentIIAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
