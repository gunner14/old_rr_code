#include "RecommendationPhotofriendAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationPhotofriendAdapter::GetFriendRecommendByHome(int userid, int limit) {
        return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
