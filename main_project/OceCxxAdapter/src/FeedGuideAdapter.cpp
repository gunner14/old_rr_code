/*
 * FeedGuideAdapter.cpp
 *
 *  Created on: Oct 15, 2009
 *      Author: antonio
 */

#include <cmath>
#include "FeedGuideAdapter.h"

using namespace MyUtil;
using namespace xce::feed;

FeedGuideManagerPrx FeedGuideAdapter::getManager(int id) {
	return locate<FeedGuideManagerPrx> (_managers, "FIM", id, TWO_WAY);
}

FeedGuideManagerPrx FeedGuideAdapter::getManagerOneway(int id) {
	return locate<FeedGuideManagerPrx> (_managersOneway, "FIM", id, ONE_WAY);
}

FeedDataResPtr FeedGuideAdapter::GetGuideFeedData(int uid, int begin, int limit){
	return getManager(uid)->GetGuideFeedData(uid,begin, limit);
}

FeedDataResPtr FeedGuideAdapter::GetGuideFeedDataByStype(int uid,
		const MyUtil::IntSeq& stypes, int begin, int limit){
	return getManager(uid)->GetGuideFeedDataByStype(uid,stypes,begin, limit);
}

void FeedGuideAdapter::addFeed(const FeedItem& item,
    const FeedConfigPtr& config, const MyUtil::Int2IntMap& uid2weight) {
    try {
      getManagerOneway(0)->AddFeed(item, config, uid2weight);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedGuideAdapter::addFeed --> call addFeed file:"<<__FILE__<<" err:"<<e);
    }
}

void FeedGuideAdapter::setReadById(int uid, Ice::Long feedId) {
  getManagerOneway(uid)->SetReadById(uid, feedId);
}

void FeedGuideAdapter::setReadAll(int uid) {
  getManagerOneway(uid)->SetReadAll(uid);
}

FeedDataSeq FeedGuideAdapter::GetFeedDataByIds(MyUtil::LongSeq fids){
  if(fids.empty()){
    return FeedDataSeq();
  }
  return getManager(0)->GetFeedDataByIds(fids);
}

