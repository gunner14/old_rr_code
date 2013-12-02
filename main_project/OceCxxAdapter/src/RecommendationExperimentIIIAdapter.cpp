#include "RecommendationExperimentIIIAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentIIIAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
