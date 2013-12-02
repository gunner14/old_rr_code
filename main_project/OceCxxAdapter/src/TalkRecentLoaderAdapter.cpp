#include "TalkRecentLoaderAdapter.h"
#include "IceLogger.h"

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::recent::loader;
using namespace MyUtil;


RecentLoaderManagerPrx TalkRecentLoaderAdapter::getManager(int id) {
	return getProxy(id);
}

RecentLoaderManagerPrx TalkRecentLoaderAdapter::getManagerOneway(int id) {
  return getProxyOneway(id);
}

void TalkRecentLoaderAdapter::Update(int actor, const TalkRecentUserPtr &groupinfo) {
	getManagerOneway(actor)->Update(actor, groupinfo);
}

TalkRecentUserSeq TalkRecentLoaderAdapter::LoadById(int owner) {
  return getManager(owner)->LoadById(owner);
}

TalkRecentUserSeqMap TalkRecentLoaderAdapter::LoadBySeq(const MyUtil::IntSeq &owners) {
	if (owners.empty()) {
		return TalkRecentUserSeqMap();
	}
	return getManager(owners.size())->LoadBySeq(owners);
}


