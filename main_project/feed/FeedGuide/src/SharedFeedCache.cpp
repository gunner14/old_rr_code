

#include "SharedFeedCache.h"
#include "FeedItemI.h"

namespace xce{
namespace feed{

SharedFeedPtr SharedFeedCache::Add(const FeedItem& item) {
  int i = GetIndex(item.feed);
  SharedFeed* sf = NULL;
  {
    IceUtil::Mutex::Lock lock(mutex_[i]);
    FeedMap::iterator it = feed_maps_[i].find(item.feed);
    if (it != feed_maps_[i].end()) {
    	sf = it->second;
    } else {
    	sf = new SharedFeed(item);
    	feed_maps_[i].insert(make_pair(item.feed, sf));
    }
    ++(sf->ref_count_);
  }
  SharedFeedPtr hdl = sf;
  {
    IceUtil::Mutex::Lock lock(mutex_[i]);
    --(sf->ref_count_);
  }
  return hdl;
}

SharedFeedPtr SharedFeedCache::Get(Ice::Long feedid) {
  int i = GetIndex(feedid);
  SharedFeed* sf = NULL;
  {
    IceUtil::Mutex::Lock lock(mutex_[i]);

    FeedMap::iterator it = feed_maps_[i].find(feedid);
    if (it == feed_maps_[i].end()) {
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
    delete sf;
  }
}

int SharedFeedCache::TotalCount() const {
  int count = 0;
  for(int i=0; i<kPoolSize; i++){
    IceUtil::Mutex::Lock lock(mutex_[i]);
    count += feed_maps_[i].size();
  }
  return count;
}

string SharedFeedCache::DumpMaps() const {
	ostringstream os;
	int feedcnt = 0;
	for(int i = 0; i < kPoolSize ; ++i) {
		const FeedMap & amap = feed_maps_[i];
		IceUtil::Mutex::Lock lock(mutex_[i]);
		feedcnt += amap.size();
	}
	os << " sharedfeed cnt:" << feedcnt;
	return os.str();
}

};
};
