
#ifndef __FEEDSINKREPLICAADAPTER_H_
#define __FEEDSINKREPLICAADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "FeedSink.h"

namespace xce {
namespace feed {

using namespace MyUtil;

//class FeedSortAdapter : public AdapterI,
//public MyUtil::ReplicatedClusterAdapterI<FeedSortPrx>,
//public MyUtil::Singleton<FeedSortAdapter>

//class FeedSinkReplicaAdapter: public MyUtil::AdapterI,
//    public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedSinkReplicaAdapter>

class FeedSinkReplicaAdapter :public MyUtil::ReplicatedClusterAdapterI<FeedSinkPrx>,
public MyUtil::Singleton<FeedSinkReplicaAdapter> {
public:
	FeedSinkReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<FeedSinkPrx> ("ControllerFeedSinkR",120,300,new XceFeedChannel){
  }
//  FeedSinkReplicaAdapter() {
//    _managers.resize(cluster() > 0 ? cluster() : 1);
//    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
//  }

  void AddFeed(const FeedItem& fitem,
      const FeedConfigPtr& config, const MyUtil::Int2IntMap& id2weight){
  	if(id2weight.empty()){
  		return;
  	}
  	int first = id2weight.begin()->first;
  	FeedSinkPrx::uncheckedCast(getManagerOneway(first)->ice_datagram())->AddFeed(fitem,config,id2weight);
  }
	void	SetRead(int uid, int stype, Ice::Long merge){
  	FeedSinkPrx::uncheckedCast(getManagerOneway(uid)->ice_datagram())->SetRead(uid,stype,merge);
	}
	void	SetReadById(int uid, Ice::Long feedId){
  	FeedSinkPrx::uncheckedCast(getManagerOneway(uid)->ice_datagram())->SetReadById(uid,feedId);
	}
	void	SetReadAll(int uid){
  	FeedSinkPrx::uncheckedCast(getManagerOneway(uid)->ice_datagram())->SetReadAll(uid);
	}
//  FeedSinkPrx getManager(int id) {
//    return locate<FeedSinkPrx> (_managers, "M", id, TWO_WAY);
//  }

  FeedSinkPrx getManagerOneway(int id) {
  	return getProxy(id);
//    return locate<FeedSinkPrx> (_managersOneway, "M", id, ONE_WAY);
  }

protected:
  virtual string name() {
    return "FeedSink";
  }
  virtual string endpoints() {
    return "@FeedSink";
  }
//  virtual size_t cluster() {
//    return 5;
//  }

//  vector<FeedSinkPrx> _managersOneway;
//  vector<FeedSinkPrx> _managers;
};

}
;
}
;
#endif /* __FeedSinkReplicaAdapter_H_ */
