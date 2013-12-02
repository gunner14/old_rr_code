/*
 * FeedNews50Adapter.cpp
 *
 *  Created on: Oct 15, 2009
 *      Author: antonio
 */

#include <cmath>
#include "FeedNews50Adapter.h"

using namespace MyUtil;
using namespace xce::feed;

FeedNewsManagerPrx FeedNews50Adapter::getManager(int id) {
	return locate<FeedNewsManagerPrx> (_managers, "FIM", id, TWO_WAY);
}

FeedNewsManagerPrx FeedNews50Adapter::getManagerOneway(int id) {
	return locate<FeedNewsManagerPrx> (_managersOneway, "FIM", id, ONE_WAY);
}

FeedDataResPtr FeedNews50Adapter::getLiveFeedDataByStype(int uid,
		const MyUtil::IntSeq& stypes, int begin, int limit){
	return getManager(uid)->GetLiveFeedDataByStype(uid,stypes,begin, limit);
}

FeedDataResPtr FeedNews50Adapter::getLiveFeedDataByType(int uid,
		const MyUtil::IntSeq& types, int begin, int limit){
	return getManager(uid)->GetLiveFeedDataByType(uid,types,begin, limit);
}

FeedDataResPtr FeedNews50Adapter::getLiveFeedData(int uid, int begin, int limit){
	return getManager(uid)->GetLiveFeedData(uid,begin, limit);
}

FeedDataResPtr FeedNews50Adapter::getHotFeedData(int uid, int begin, int limit){
	return getManager(uid)->GetHotFeedData(uid,begin, limit);
}

void FeedNews50Adapter::addFeed(const FeedItem& item, const FeedConfigPtr& config, const MyUtil::Int2IntMap& uid2weight){
  if(uid2weight.empty()){
    return;
  }
  getManagerOneway(0)->AddFeed(item, config, uid2weight);
  return;


	map<int , MyUtil::Int2IntMap> tmap;
	for(MyUtil::Int2IntMap::const_iterator it = uid2weight.begin(); it != uid2weight.end(); ++it){
    MCE_INFO("aa");
		tmap[abs(it->first) % _cluster][it->first] = it->second;
    MCE_INFO("aa");
	}
  MCE_INFO(__LINE__ << " tmap size:" << tmap.size());
	for(map<int , MyUtil::Int2IntMap>::iterator it = tmap.begin(); it != tmap.end(); ++it){
		//if(it->first%_cluster != 11){
    //  continue;
    //}
    MCE_INFO(__LINE__ );
    try{
      MCE_INFO(__LINE__ );
		  //FeedNewsManagerPrx::uncheckedCast(getManagerOneway(it->first)->ice_datagram())->AddFeed(item, it->second);
		  getManagerOneway(it->first)->AddFeed(item, config, it->second);
		}catch(Ice::Exception& e){
			MCE_WARN("FeedNews50Adapter::addFeed --> call addFeed file:"<<__FILE__<<" err:"<<e);
		}
    MCE_INFO(__LINE__ );
	}
}

//void FeedNews50Adapter::AddFeedReply(const FeedIndexPtr & index ) {
//  getManager(index->actor)->AddFeedReply(index);
//}

int FeedNews50Adapter::GetUnviewedCount(int uid, int type){
  return getManager(uid)->GetUnviewedCount(uid, type);
}

void FeedNews50Adapter::setRead(int uid, int stype, Ice::Long source){
	/*if(uid%_cluster != 11){
    return;
  }*/
  getManagerOneway(uid)->SetRead(uid,stype,source);
}
void FeedNews50Adapter::setReadById(int uid, Ice::Long feedId){
	/*if(uid%_cluster != 11){
    return;
  }*/
  getManagerOneway(uid)->SetReadById(uid,feedId);
}
void FeedNews50Adapter::setReadAll(int uid){
	/*if(uid%_cluster != 11){
    return;
  }*/
  getManagerOneway(uid)->SetReadAll(uid);
}

void FeedNews50Adapter::load(int uid){
	//return FeedInteractionPrx::uncheckedCast(locate<FeedInteractionPrx>(_managers, "M", id, ONE_WAY)->ice_datagram());
  //if(uid%cluster() == 11){
	  FeedNewsManagerPrx::uncheckedCast(getManagerOneway(uid)->ice_datagram())->load(uid);
  //}
}

FeedDataSeq FeedNews50Adapter::GetFeedDataByIdsWithUid(int uid,const MyUtil::LongSeq& fids, Ice::Long first,int stype){
  return getManager(uid)->GetFeedDataByIdsWithUid(uid,fids,first,stype);

}



FeedDataResPtr FeedNews50Adapter::GetHotFeedDataByStype(int uid, MyUtil::IntSeq stypes, int begin, int limit){
  return getManager(uid)->GetHotFeedDataByStype(uid, stypes,begin,limit);
}
FeedDataResPtr FeedNews50Adapter::GetHotFeedDataByType(int uid, MyUtil::IntSeq stypes, int begin, int limit){
  return getManager(uid)->GetHotFeedDataByType(uid, stypes,begin,limit);
}
FeedDataSeq FeedNews50Adapter::GetFeedDataByIds(MyUtil::LongSeq fids){
  if(fids.empty()){
    return FeedDataSeq();
  }
  return getManager(fids.at(0))->GetFeedDataByIds(fids);

}
bool FeedNews50Adapter::HasFeed(int uid){
  return getManager(uid)->HasFeed(uid);
}


FeedItemSeq FeedNews50Adapter::GetOriginalFeedData(int uid){
  return getManager(uid)->GetOriginalFeedData(uid);
}
