#include "RecommendationExperimentVVAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentVVAdapter::GetFriendRecommendByHome(int userid, int limit) {
        return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}

