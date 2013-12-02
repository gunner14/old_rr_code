#include "FeedPreferCacheAdapter.h"

using namespace xce::feed;


FeedPreferCacheAdapter::FeedPreferCacheAdapter()
{
	_managers.resize(cluster() > 0 ? cluster() : 1);
	_managersOneway.resize(cluster() > 0 ? cluster() : 1);
}

FeedPreferCacheAdapter::~FeedPreferCacheAdapter()
{

}

int GetIndex(int weeks){
	switch (weeks){
	case 1: return 0;
	case 2:	return 1;
	case 4: return 2;
	case 6: return 3;
	case 8: return 4;
	default: return 1;
	}
}
MyUtil::IntSeq  FeedPreferCacheAdapter::Get(int uid,int weeks){
	return getManager(GetIndex(weeks))->Get(uid,weeks);
}
FeedPreferCachePrx FeedPreferCacheAdapter::getManager(int id)
{
	return locate<FeedPreferCachePrx> (_managers, "M", id, TWO_WAY);
}

FeedPreferCachePrx FeedPreferCacheAdapter::getManagerOneway(int id)
{
	return locate<FeedPreferCachePrx> (_managersOneway, "M", id, ONE_WAY);
}

