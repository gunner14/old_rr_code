#include "RecommendationFeedOnlineAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationFeedOnlineAdapter::GetRecommendationByFeed(int userid, int limit) {
	return getProxy(userid)->GetRecommendationByFeed(userid, limit);
}

}}
