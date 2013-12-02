#include "RecommendationOnlineAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationOnlineAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
