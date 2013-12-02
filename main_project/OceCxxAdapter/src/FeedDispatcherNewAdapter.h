#ifndef FEEDDISPATCHERADAPTER_H_
#define FEEDDISPATCHERADAPTER_H_

#include "RFeed.h"
#include <vector>
#include "Singleton.h"
#include "AdapterI.h"


namespace xce {
namespace feed {

class FeedDispatcherNewAdapter: public MyUtil::AdapterI,
		public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedDispatcherNewAdapter> {
public:
	FeedDispatcherNewAdapter(){
		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
	}
	void addBuddyNotify(int host, int guest);
	void joinPageNotify(int page, int fans);
	void buddyApplyNotify(int host, int guest);
	void addMiniSiteFeed(int siteid, int userid);
  void guideBuddyReplyNotify(int newUser, int buddy);
	void addMiniSiteFeedToGuides(long source,int type,int siteid, const vector<int> & guides);
  void Reload(LoadType type, int id);
  void Reload2(LoadType type,long id);
  void ReloadBatch(LoadType type, const vector<int> & ids);



protected:
	virtual string name() {
		return "FeedDispatcherNew";
	}
	virtual string endpoints() {
		return "@FeedDispatcherNew";
	}
	virtual size_t cluster() {
		return 10;
	}

	FeedDispatcherNewPrx getManager(int id);
	FeedDispatcherNewPrx getManagerOneway(int id);

	vector<FeedDispatcherNewPrx> _managersOneway;
	vector<FeedDispatcherNewPrx> _managers;
};

};
};

#endif /*BUDDYCOREADAPTER_H_*/
