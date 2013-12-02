#ifndef TALKRECENTLOADERADAPTER_H_
#define TALKRECENTLOADERADAPTER_H_

#include "AdapterI.h"
#include "TalkRecentLoader.h"

#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {

using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::recent::loader;

class TalkRecentLoaderAdapter : public MyUtil::ReplicatedClusterAdapterI<RecentLoaderManagerPrx>, public MyUtil::Singleton<TalkRecentLoaderAdapter> {

public:

	TalkRecentLoaderAdapter() : MyUtil::ReplicatedClusterAdapterI<RecentLoaderManagerPrx> ("ControllerTalkRecentLoader",120,300,new TalkChannel) {
		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
	}


  void Update(int actor, const TalkRecentUserPtr &groupinfo);
  TalkRecentUserSeq LoadById(int owner);
  TalkRecentUserSeqMap LoadBySeq(const MyUtil::IntSeq &owners);

protected:
	virtual string name() {
		return "TalkRecentLoader";
	}
	virtual string endpoints() {
		return "@TalkRecentLoader";
	}
	;
	virtual size_t cluster() {
		return 10;
	}

	RecentLoaderManagerPrx getManagerOneway(int id);
	RecentLoaderManagerPrx getManager(int id);

	vector<RecentLoaderManagerPrx> _managersOneway;
	vector<RecentLoaderManagerPrx> _managers;
};

}
;
}
;
}
;

#endif
