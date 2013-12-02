#include "DistributionManagerAdapter.h"

namespace xce {
namespace socialgraph {

RecommendItemSeq DistributionManagerAdapter::GetFriendRecommendByHome(int userid, int limit) {
	return getProxy(userid)->GetFriendRecommendByHome(userid, limit);
}

void DistributionManagerAdapter::AddRecommendationFeedAsync(int userid, int limit) {
	getProxyOneway(userid)->AddRecommendationFeedAsync(userid, limit);
}

void DistributionManagerAdapter::ReloadBlackAndWhiteFile(int userid) {
	getProxyOneway(userid)->ReloadBlackAndWhiteFile(userid);
}

}}
