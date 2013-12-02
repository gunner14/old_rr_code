/*************************************************
 *
 *  @Copyright    Renren Inc.
 *  @Brief        FeedExperimentCache服务的ReplicaAdaptor
 *  @Filename     FeedExperimentCacheReplicaAdapter.h 
 *  @Author       rui.qiu
 *  @Email        rui.qiu@renren-inc.com
 *  @Date         2012-11-18
 *  @History
 *
 *************************************************/

#ifndef __FEED_EXPERIMENT_CACHE_REPLICA_ADAPTER_H__
#define __FEED_EXPERIMENT_CACHE_REPLICA_ADAPTER_H__

#include "Singleton.h"
#include "AdapterI.h"
#include "FeedExperiment.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedExperimentCacheReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedExperimentManagerPrx>,
    public MyUtil::Singleton<FeedExperimentCacheReplicaAdapter>{

public:
  FeedExperimentCacheReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<FeedExperimentManagerPrx> ("ControllerFeedExperimentCache", 120, 300, new XceFeedControllerChannel), index_(0){
  }

  BayesModelList GetBayesModels(int mod, int begin, int size);

protected:
  FeedExperimentManagerPrx getManager(int index);
  FeedExperimentManagerPrx getManagerOneway(int index);

private:
  int index_;

  virtual string name() {
    return "FEC";
  }

  virtual string endpoints() {
    return "@FeedExperimentCache";
  }

};

};
};

#endif
