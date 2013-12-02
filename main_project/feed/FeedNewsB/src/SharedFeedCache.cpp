/*
 * SharedFeedCache.cpp
 *
 *  Created on: Oct 21, 2011
 *      Author: yejingwei
 */


#include "SharedFeedCache.h"

#include "FeedItemI.h"
#include "../../FeedMemcProxy/client/feed_interaction_client_n.h"

//#include "PerformanceStat.h"

namespace xce{
namespace feed{

SharedFeedPtr SharedFeedCache::Add(const FeedItem& item) {
  int i = GetIndex(item.feed);
  SharedFeed* sf = NULL;
  {
    IceUtil::Mutex::Lock lock(mutex_[i]);
    FeedMap::iterator it = feed_maps_[i].find(item.feed);
    if(it != feed_maps_[i].end()){
    	sf = it->second;
    }else{
    	sf = new SharedFeed(item);
//    	sf = pools_[i].malloc();
//    	sf = pools_[i].construct(item);
    	feed_maps_[i].insert(make_pair(item.feed, sf));
    }
    ++(sf->ref_count_);
  }
  SharedFeedPtr hdl = sf;
  {
    IceUtil::Mutex::Lock lock(mutex_[i]);
    --(sf->ref_count_);
  }
  if(hdl->reply_ == -1 && hdl->range_ == -1)
  	InteractionDownloader::instance().Add(item.feed);

  return hdl;
}

SharedFeedPtr SharedFeedCache::Get(Ice::Long feedid) {
  int i = GetIndex(feedid);
  SharedFeed* sf = NULL;
  {
    IceUtil::Mutex::Lock lock(mutex_[i]);

    FeedMap::iterator it = feed_maps_[i].find(feedid);
    if (it == feed_maps_[i].end()){
    	return sf;
    }
    sf = it->second;
    ++(sf->ref_count_);
  }
  SharedFeedPtr hdl = sf;
  {
    IceUtil::Mutex::Lock lock(mutex_[i]);
    --(sf->ref_count_);
  }
  return hdl;
}

void SharedFeedCache::Inc(SharedFeed*  sf) {
  if (sf == NULL) return;
  int i = GetIndex(sf->id_);
  IceUtil::Mutex::Lock lock(mutex_[i]);
  ++(sf->ref_count_);
}

void SharedFeedCache::Dec(SharedFeed*  sf) {
  if (sf == NULL)  return;
  int i = GetIndex(sf->id_);
  IceUtil::Mutex::Lock lock(mutex_[i]);
  if (--(sf->ref_count_) == 0) {
    feed_maps_[i].erase(sf->id_);
//  	pools_[i].free(sf);
    delete sf;
  }
}

void SharedFeedCache::AddInteraction(long fid,int reply,int range){
	SharedFeedPtr sfp = Get(fid);
	if(sfp.GetRawPtr() == NULL)
		return;
	if(reply == -1 and range == -1){
		sfp->reply_ = 0;
		sfp->range_ = 0;
	}else {
		sfp->reply_ = reply;
		sfp->range_ = range;
	}
}
int SharedFeedCache::TotalCount()const {
  int count = 0;
  for(int i=0; i<kPoolSize; i++){
    IceUtil::Mutex::Lock lock(mutex_[i]);
    count += feed_maps_[i].size();
  }
  return count;
}

string SharedFeedCache::DumpMaps() const {
	ostringstream os;
	int feedcnt = 0,ref = 0;

	for(int i = 0; i < kPoolSize ; ++i){
		const FeedMap & amap = feed_maps_[i];
//		IceUtil::Mutex::Lock lock(mutex_[i]);
		using namespace boost;
		BOOST_FOREACH(const FeedMap::value_type & vt,amap){
			++feedcnt;
			ref += vt.second->ref_count_;
		}
	}

	os << " sharedfeed cnt:" << feedcnt << " ref:" << ref;
	return os.str();
}

void InteractionDownloader::handle(){
	set<long> fids_swap;
  {
  	IceUtil::Mutex::Lock lock(mutex_);
  	fids_swap.swap(fids_);
  }
  const static int batch_size = 100;
  set<long> fids_batch;
  int cnt = 0;
  int unhandled = fids_swap.size();
	FeedInterMap intermap;
  BOOST_FOREACH(long fid,fids_swap){
  	fids_batch.insert(fid);
  	--unhandled;
  	++cnt;
  	if(cnt >= batch_size or unhandled == 0){
  		FeedInteractionMemcClientN::instance().GetFeedInterMap(fids_batch,intermap);
  		cnt = 0;
  		fids_batch.clear();
  	}
  }

  set<long> fids_fail = fids_swap;
  BOOST_FOREACH(FeedInterMap::value_type & p ,intermap){
  	SharedFeedCache::instance().AddInteraction(p.first,p.second->GetReply(),p.second->GetRange());
  	fids_fail.erase(p.first);
  }

//  Add(fids_fail);
  MCE_INFO("InteractionDownloader::handle. fids_swap size:" << fids_swap.size()
  		<< " intermap:" << intermap.size()
  		<< " fids_fail:" << fids_fail.size());
  TaskManager::instance().schedule(&InteractionDownloader::instance());
}

//
//SharedFeed* SharedFeedCache::Add(const FeedItem& item) {
//  int i = GetIndex(item.feed);
//  SharedFeed* sf = NULL;
//  {
//    IceUtil::Mutex::Lock lock(mutex_[i]);
//
//    FeedMap::iterator it = feed_maps_[i].find(item.feed);
//
//    if (it == feed_maps_[i].end()) {
//      sf = new SharedFeed(item);
//      feed_maps_[i].insert(make_pair(item.feed, sf));
//    } else {
//      sf = it->second;
//    }
//
//    ++(sf->ref_count_);
//  }
//
//  SharedFeed* ptr = sf;
//  {
//    IceUtil::Mutex::Lock lock(mutex_[i]);
//    --(sf->ref_count_);
//  }
//  return ptr;
//}
//
//SharedFeed* SharedFeedCache::Get(Ice::Long feedid) {
//  int i = GetIndex(feedid);
//  SharedFeed* sf = NULL;
//  {
//    IceUtil::Mutex::Lock lock(mutex_[i]);
//
//    FeedMap::iterator it = feed_maps_[i].find(feedid);
//    if (it == feed_maps_[i].end()) return NULL;
//
//    ++(it->second->ref_count_);
//    sf = it->second;
//  }
//
//  SharedFeed* ptr = sf;
//  {
//    IceUtil::Mutex::Lock lock(mutex_[i]);
//    --(sf->ref_count_);
//  }
//  return ptr;
//}
//
//void SharedFeedCache::Inc(SharedFeed* sf) {
//  if (sf == NULL) return;
//
//  int i = GetIndex(sf->id_);
//  IceUtil::Mutex::Lock lock(mutex_[i]);
//  ++(sf->ref_count_);
//}
//
//void SharedFeedCache::Release(SharedFeed* sf) {
//  if (sf == NULL)  return;
//
//  int i = GetIndex(sf->id_);
//  IceUtil::Mutex::Lock lock(mutex_[i]);
//  if (--(sf->ref_count_) == 0) {
//    feed_maps_[i].erase(sf->id_);
//    delete sf;
//  }
//}
//
//int SharedFeedCache::TotalCount() {
//  int count = 0;
//  for(int i=0; i<kPoolSize; i++){
//    IceUtil::Mutex::Lock lock(mutex_[i]);
//    count += feed_maps_[i].size();
//  }
//  return count;
//}

};
};
