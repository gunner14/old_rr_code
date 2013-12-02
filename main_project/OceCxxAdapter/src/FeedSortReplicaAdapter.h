#ifndef FEED_SORT_REPLICA_ADAPTER_H
#define FEED_SORT_REPLICA_ADAPTER_H

#include "FeedSort.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce{
namespace feed{

using namespace MyUtil;

class FeedSortAdapter : public AdapterI,
public MyUtil::ReplicatedClusterAdapterI<FeedSortPrx>,
public MyUtil::Singleton<FeedSortAdapter>
{

protected:
	virtual string name() {
		return "FeedSort";
	}
	virtual string endpoints() {
		return "@FeedSort";
	}
	virtual size_t cluster() {
		return 0;
	}

public:
	FeedSortAdapter();

	~FeedSortAdapter();

	virtual SortResultPtr Sort(int uid, const ::xce::feed::SortParamPtr & param) {
		return getProxy(uid)->Sort(uid,param);
	}
private:

//	FeedSortPrx getManager(int id);
//	FeedSortPrx getManagerOneway(int id);
//
//	vector<FeedSortPrx> _managersOneway;
//	vector<FeedSortPrx> _managers;
};

class FeedSortReplicaAdapter1 : public MyUtil::ReplicatedClusterAdapterI<FeedSortPrx>, 
    public MyUtil::Singleton<FeedSortReplicaAdapter1>
{
public:
    FeedSortReplicaAdapter1() : 
        MyUtil::ReplicatedClusterAdapterI<FeedSortPrx>(
                "ControllerFeedSortR", 120, 300, new XceFeedChannel())
    {}

	virtual SortResultPtr Sort(int uid, const ::xce::feed::SortParamPtr & param) {
		return getProxy(uid)->Sort(uid,param);
	}
};
class FeedSortReplicaAdapter2 : public MyUtil::ReplicatedClusterAdapterI<FeedSortPrx>, 
    public MyUtil::Singleton<FeedSortReplicaAdapter2>
{
public:
    FeedSortReplicaAdapter2() : 
        MyUtil::ReplicatedClusterAdapterI<FeedSortPrx>(
                "ControllerFeedSort", 120, 300, new XceFeedControllerChannel())
    {}

	virtual SortResultPtr Sort(int uid, const ::xce::feed::SortParamPtr & param) {
		return getProxy(uid)->Sort(uid,param);
	}
};

}
}
#endif
