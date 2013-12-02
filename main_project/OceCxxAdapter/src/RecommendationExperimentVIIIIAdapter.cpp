#include "RecommendationExperimentVIIIIAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentVIIIIAdapter::GetFriendRecommendByHome(int userid, int limit) {
        return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}

