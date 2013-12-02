/*
 * FeedNews50Adapter.h
 *
 *  Created on: Oct 15, 2009
 *      Author: antonio
 */

#ifndef FEEDNEWSF50ADAPTER_H_
#define FEEDNEWSF50ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "RFeed.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedNews50Adapter: public MyUtil::AdapterI,
		public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel,
		//public MyUtil::AdapterISingleton<MyUtil::XceFeedF55Channel,
				FeedNews50Adapter> {
public:
	FeedNews50Adapter() {
		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
	}

	FeedDataResPtr getLiveFeedData(int uid, int begin, int limit);
	FeedDataResPtr getLiveFeedDataByStype(int uid, const MyUtil::IntSeq& stypes,
			int begin, int limit);
	FeedDataResPtr getLiveFeedDataByType(int uid, const MyUtil::IntSeq& stypes, int begin,
			int limit);
	
  FeedDataResPtr getHotFeedData(int uid, int begin, int limit);

	void addFeed(const FeedItem& item, const FeedConfigPtr& config, const MyUtil::Int2IntMap& uid2weight);

	void setRead(int uid, int stype, Ice::Long source);
	void setReadById(int uid, Ice::Long feedId);
	void setReadAll(int uid);

  int GetUnviewedCount(int uid, int type);	
  void load(int uid);

  FeedDataSeq GetFeedDataByIdsWithUid(int uid,const MyUtil::LongSeq& fids, Ice::Long first,int stype);

  //------
  FeedDataResPtr GetHotFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit);
  FeedDataResPtr GetHotFeedDataByType(int uid, MyUtil::IntSeq stypes, int begin, int limit);
  FeedDataSeq GetFeedDataByIds(MyUtil::LongSeq fids);
  bool HasFeed(int uid);
  FeedItemSeq GetOriginalFeedData(int uid);
  //----------
  //void AddFeedReply(const FeedIndexPtr & index);
  void Test(int uid, int begin,int limit) {
      FeedNewsManagerPrx::uncheckedCast(getManagerOneway(uid)->ice_datagram())->GetHotFeedData(uid,begin, limit);
      return;
  }

protected:
	virtual string name() {
		return "FeedNews50";
	}
	virtual string endpoints() {
		return "@FeedNews50";
	}
	virtual size_t cluster() {
		return 0;
	}

	FeedNewsManagerPrx getManager(int id);
	FeedNewsManagerPrx getManagerOneway(int id);

	vector<FeedNewsManagerPrx> _managersOneway;
	vector<FeedNewsManagerPrx> _managers;
};

}
;
}
;
#endif /* FEEDITEMADAPTER_H_ */
