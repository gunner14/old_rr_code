/*
 * FeedGuideAdapter.cpp
 *
 *  Created on: Oct 15, 2009
 *      Author: antonio
 */

#include <cmath>
#include "FeedGuideReplicaAdapter.h"

using namespace MyUtil;
using namespace xce::feed;

FeedGuideManagerPrx FeedGuideReplicaAdapter::getManager(int id) {
  return getProxy(id);
}

FeedGuideManagerPrx FeedGuideReplicaAdapter::getManagerOneway(int id) {
  return getProxyOneway(id);
}

FeedDataResPtr FeedGuideReplicaAdapter::GetGuideFeedData(int uid, int begin, int limit){
	return getManager(uid)->GetGuideFeedData(uid,begin, limit);
}

FeedDataResPtr FeedGuideReplicaAdapter::GetGuideFeedDataByStype(int uid,
		const MyUtil::IntSeq& stypes, int begin, int limit){
	return getManager(uid)->GetGuideFeedDataByStype(uid,stypes,begin, limit);
}

void FeedGuideReplicaAdapter::addFeed(const FeedItem& item,
    const FeedConfigPtr& config, const MyUtil::Int2IntMap& uid2weight) {
    try {
      getManagerOneway(0)->AddFeed(item, config, uid2weight);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedGuideAdapter::addFeed --> call addFeed file:"<<__FILE__<<" err:"<<e);
    }
}

void FeedGuideReplicaAdapter::setReadById(int uid, Ice::Long feedId) {
  getManagerOneway(uid)->SetReadById(uid, feedId);
}

void FeedGuideReplicaAdapter::setReadAll(int uid) {
  getManagerOneway(uid)->SetReadAll(uid);
}

FeedDataSeq FeedGuideReplicaAdapter::GetFeedDataByIds(MyUtil::LongSeq fids){
  if(fids.empty()){
    return FeedDataSeq();
  }
  return getManager(0)->GetFeedDataByIds(fids);
}

