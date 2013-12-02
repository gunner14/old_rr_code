/*
 * FeedNewsAdapter.h
 *
 *  Created on: Oct 15, 2009
 *      Author: antonio
 */

#ifndef FEEDGUIDEREPLICADAPTER_H_
#define FEEDGUIDEREPLICAADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "FeedGuide.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedGuideReplicaAdapter: public MyUtil::ReplicatedClusterAdapterI<FeedGuideManagerPrx>,
  public MyUtil::Singleton<FeedGuideReplicaAdapter> {
public:
  FeedGuideReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<FeedGuideManagerPrx> ("ControllerFeedGuideR",120,300,new XceFeedControllerChannel) {
  }

	FeedDataResPtr GetGuideFeedData(int uid, int begin, int limit);
	FeedDataResPtr GetGuideFeedDataByStype(int uid, const IntSeq& stype, int begin, int limit);

	void addFeed(const FeedItem& item, const FeedConfigPtr& config, const MyUtil::Int2IntMap& uid2weight);
	void setReadById(int uid, Ice::Long feedId);
	void setReadAll(int uid);
  FeedDataSeq GetFeedDataByIds(MyUtil::LongSeq fids);

protected:

	FeedGuideManagerPrx getManager(int id);
	FeedGuideManagerPrx getManagerOneway(int id);

};



}
;
}
;
#endif /* FEEDITEMADAPTER_H_ */
