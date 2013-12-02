/*
 * FeedNewsReplicaAdapter.h
 *
 *  Created on: Oct 15, 2009
 *      Author: antonio
 */

#ifndef FEEDNEWSREPLICAADAPTER_H_
#define FEEDNEWSREPLICAADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "RFeed.h"

namespace xce {
namespace feed {

using namespace MyUtil;


class FeedNewsReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedNewsManagerPrx>,
  public MyUtil::Singleton<FeedNewsReplicaAdapter>{

public:

  FeedNewsReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<FeedNewsManagerPrx> ("ControllerFeedNewsR",120,300,new XceFeedChannel){
  }

	FeedDataResPtr getLiveFeedData(int uid, int begin, int limit);
	FeedDataResPtr getLiveFeedDataByStype(int uid, const MyUtil::IntSeq& stypes,
			int begin, int limit);
	FeedDataResPtr getLiveFeedDataByType(int uid, const MyUtil::IntSeq& stypes, int begin,
			int limit);
	
  FeedDataResPtr getHotFeedData(int uid, int begin, int limit);

  FeedDataResPtr GetFriendOriginal(int uid, const MyUtil::IntSeq& friends, int begin, int limit);
  FeedDataResPtr GetFeedDataByGroupName(int uid, string name, int begin, int limit);
  FeedDataResPtr GetFeedDataByGroupId(int uid, int id, int begin, int limit);

	void addFeed(const FeedItem& item, const FeedConfigPtr& config, const MyUtil::Int2IntMap& uid2weight);

	void setRead(int uid, int stype, Ice::Long source);
	void setReadById(int uid, Ice::Long feedId);
	void setReadAll(int uid);

  int GetUnviewedCount(int uid, int type);	
  void load(int uid);

  FeedDataSeq GetFeedDataByIdsWithUid(int uid,const MyUtil::LongSeq& fids, Ice::Long first,int stype);


//--------------新加
  FeedDataResPtr GetHotFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit);
  FeedDataResPtr GetHotFeedDataByType(int uid, MyUtil::IntSeq stypes, int begin, int limit);
  FeedDataSeq GetFeedDataByIds(MyUtil::LongSeq fids);
  bool HasFeed(int uid);
  FeedItemSeq GetOriginalFeedData(int uid);
//-----------------
  void MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark);
  void LoadExperiment(int totalsize, int idx);


protected:


	FeedNewsManagerPrx getManager(long id);
	FeedNewsManagerPrx getManagerOneway(int id);


};

}
;
}
;
#endif /* FEEDITEMADAPTER_H_ */
