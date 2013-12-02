/*
 * FeedNewsAdapter.h
 *
 *  Created on: Oct 15, 2009
 *      Author: antonio
 */

#ifndef FEEDNEWSADAPTER_H_
#define FEEDNEWSADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "FeedGuide.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedGuideAdapter: public MyUtil::AdapterI,
		public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedGuideAdapter> {
public:
	FeedGuideAdapter() {
		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
	}

	FeedDataResPtr GetGuideFeedData(int uid, int begin, int limit);
	FeedDataResPtr GetGuideFeedDataByStype(int uid, const IntSeq& stype, int begin, int limit);

	void addFeed(const FeedItem& item, const FeedConfigPtr& config, const MyUtil::Int2IntMap& uid2weight);
	void setReadById(int uid, Ice::Long feedId);
	void setReadAll(int uid);
  FeedDataSeq GetFeedDataByIds(MyUtil::LongSeq fids);

protected:
	virtual string name() {
		return "FeedGuide";
	}
	virtual string endpoints() {
		return "@FeedGuide";
	}
	virtual size_t cluster() {
		return 1;
    //return 20;
	}

	FeedGuideManagerPrx getManager(int id);
	FeedGuideManagerPrx getManagerOneway(int id);

	vector<FeedGuideManagerPrx> _managersOneway;
	vector<FeedGuideManagerPrx> _managers;
};



}
;
}
;
#endif /* FEEDITEMADAPTER_H_ */
