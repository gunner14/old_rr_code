#include "RecommendationFeedExperimentIAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationFeedExperimentIAdapter::GetRecommendationByFeed(int userid, int limit) {
	return getProxy(userid)->GetRecommendationByFeed(userid, limit);
}

}}
