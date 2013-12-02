#ifndef __FEEDMARKREPLICA_ADAPTER_H_
#define __FEEDMARKREPLICA_DAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "Channel.h"
#include "FeedMark.h"
#include "ReplicaCluster/ClusterState/src/client/cxx/ClientInterface.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedMarkReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedMarkPrx>,
  public MyUtil::Singleton<FeedMarkReplicaAdapter>
  {
    public:
      FeedMarkReplicaAdapter() : 
            MyUtil::ReplicatedClusterAdapterI<FeedMarkPrx> 
            ("ControllerFeedMark", 120, 300, new XceFeedControllerChannel()) {
        }

      void MarkFeed(int uid, long fid, long source, int stype, int actor, bool mark);
      MarkInfoPtr GetMarkInfo(int uid);

  };



}

}

#endif


