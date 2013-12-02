/*
 * SharedFeedCache.cpp
 *
 *  Created on: Oct 21, 2011
 *      Author: yejingwei
 */


#include "SharedFeedCache.h"

#include "FeedItemI.h"
//#include "PerformanceStat.h"
#include "../../FeedMemcProxy/client/feed_interaction_client_n.h"


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

  //rong.tang 120321  热度
//  MCE_INFO("@@@  "<<hdl->reply_<<"  "<<hdl->range_);
  if (hdl->reply_ == -1 && hdl->range_ == -1) {
	  InteractionDownloader::instance().Add(item.feed);
  }
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

//rong.tang 120321  热度
void SharedFeedCache::AddInteraction(long fid, int reply, int range) {
	SharedFeedPtr sfp = Get(fid);
	if (sfp.GetRawPtr() == NULL ) {
		MCE_WARN("@@@  SharedFeedCache::AddInteraction   sharedfeed did not exit!   fid:" << fid);
		return;
	}
//	int old_reply = sfp->reply_;
//	int old_range = sfp->range_;

	if (reply == -1 && range == -1) {
		sfp->reply_ = 0;
		sfp->range_ = 0;
	} else {
		sfp->reply_ = reply;
		sfp->range_ = range;
	}

//	MCE_INFO("@@@  SharedFeedCache::AddInteraction   fid:"<< fid
//			<<"  Before:"<<old_reply << " "<<old_range
//			<<"  After:"<<sfp->reply_<< " "<<sfp->range_);
}

//rong.tang 120321  热度
void SharedFeedCache::AddFeedReply(long fid, int reply) {
	SharedFeedPtr sfp = Get(fid);
	if (sfp.GetRawPtr() == NULL) {
		MCE_WARN("@@@  SharedFeedCache::AddFeedReply.  sharedfeed did not exit!  fid:" << fid);
		return;
	}

	int old_reply = sfp->reply_;
	sfp->reply_ = reply;

//	MCE_INFO("@@@  SharedFeedCache::AddFeedReply.  fid:"<<fid
//			<<"   Before:"<<old_reply << "  After:"<<sfp->reply_);
}


int SharedFeedCache::TotalCount()const {
  int count = 0;
  for(int i=0; i<kPoolSize; i++){
    IceUtil::Mutex::Lock lock(mutex_[i]);
    count += feed_maps_[i].size();
  }
  return count;
}

//string SharedFeedCache::DumpMaps() const {
//	ostringstream os;
//	int feedcnt = 0,ref = 0;
//
//	for(int i = 0; i < kPoolSize ; ++i){
//		const FeedMap & amap = feed_maps_[i];
////		IceUtil::Mutex::Lock lock(mutex_[i]);
//		using namespace boost;
//		BOOST_FOREACH(const FeedMap::value_type & vt,amap){
//			++feedcnt;
//			ref += vt.second->ref_count_;
//		}
//	}
//
//	os << " sharedfeed cnt:" << feedcnt << " ref:" << ref;
//	return os.str();
//}

string SharedFeedCache::DumpMaps() const {
	ostringstream os;
	int feedcnt = 0;

	for(int i = 0; i < kPoolSize ; ++i){
		const FeedMap & amap = feed_maps_[i];
		IceUtil::Mutex::Lock lock(mutex_[i]);
		feedcnt += amap.size();
	}

	os << " sharedfeed cnt:" << feedcnt;
	return os.str();
}


//rong.tang 120321  热度
void InteractionDownloader::handle() {
	set<long> fids_swap;
	{
		IceUtil::Mutex::Lock lock(mutex_);
		fids_swap.swap(fids_);
	}
	const static int 	batch_size = 100;
	set<long> fids_batch;
	int cnt = 0;
	int unhandled = fids_swap.size();
	FeedInterMap intermap;
	BOOST_FOREACH(long fid, fids_swap) {
		fids_batch.insert(fid);
		--unhandled;
		++cnt;
		if (cnt >= batch_size || unhandled == 0) {
			FeedInteractionMemcClientN::instance().GetFeedInterMap(fids_batch, intermap);
			cnt = 0;
			fids_batch.clear();
		}
	}

	set<long> fids_fail = fids_swap;
	BOOST_FOREACH(FeedInterMap::value_type &  p, intermap) {
		SharedFeedCache::instance().AddInteraction( p.first, p.second->GetReply(), p.second->GetRange());
		fids_fail.erase(p.first);
	}

	if (!fids_swap.empty()) {
		MCE_INFO("@@@  InteractionDownloader::handle.  size   fids_swap:" << fids_swap.size()
				<< " intermap:" <<intermap.size() << " fids_fail:" <<fids_fail.size() );
	}
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
