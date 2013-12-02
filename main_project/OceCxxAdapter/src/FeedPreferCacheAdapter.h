#ifndef FEED_PREFER_CACHE_ADAPTER_H
#define FEED_PREFER_CACHE_ADAPTER_H

#include "FeedPreferCache.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedPreferCacheAdapter : public AdapterI,
	public AdapterISingleton<MyUtil::XceFeedChannel, FeedPreferCacheAdapter>
{

protected:
	virtual string name() {
		return "FeedPreferCache";
	}
	virtual string endpoints() {
		return "@FeedPreferCache";
	}
	virtual size_t cluster() {
		return 5;
	}

public:
	FeedPreferCacheAdapter();

	~FeedPreferCacheAdapter();

	MyUtil::IntSeq  Get(int uid,int weeks);

private:

	FeedPreferCachePrx getManager(int id);
	FeedPreferCachePrx getManagerOneway(int id);

	vector<FeedPreferCachePrx> _managersOneway;
	vector<FeedPreferCachePrx> _managers;
};


};
};
#endif
