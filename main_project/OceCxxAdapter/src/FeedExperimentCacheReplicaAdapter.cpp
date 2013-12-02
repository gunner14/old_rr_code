/*************************************************
 *
 *  @Copyright    Renren Inc.
 *  @Brief        FeedExperimentCache服务的ReplicaAdaptor
 *  @Filename     FeedExperimentCacheReplicaAdapter.cpp 
 *  @Author       rui.qiu
 *  @Email        rui.qiu@renren-inc.com
 *  @Date         2012-11-21
 *  @History
 *
 *************************************************/

#include "FeedExperimentCacheReplicaAdapter.h"

using namespace MyUtil;
using namespace xce::feed;

FeedExperimentManagerPrx FeedExperimentCacheReplicaAdapter::getManager(int index) {
  return getProxy(index);
}

FeedExperimentManagerPrx FeedExperimentCacheReplicaAdapter::getManagerOneway(int index) {
  return getProxyOneway(index);
}

BayesModelList FeedExperimentCacheReplicaAdapter::GetBayesModels(int mod, int begin, int size) {
  index_ = (index_ + 1) % getCluster();
  MCE_DEBUG("FeedExperimentCacheReplicaAdapter::GetBayesModels --> FeedNews" << mod << ", begin:" << begin << ", size:" << size);
  return getManager(index_)->GetBayesModels(mod, begin, size);
}

