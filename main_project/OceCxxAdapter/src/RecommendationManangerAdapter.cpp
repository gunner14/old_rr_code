#include "RecommendationManangerAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationManangerAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

}}
