#include "GuideRecommendContentAdapter.h"

namespace xce {
namespace socialgraph {

void GuideRecommendContentAdapter::TriggerContentNewsFeed(int userid, int friendid) {
	getProxy(userid)->TriggerContentNewsFeed(userid, friendid);
}

}}
