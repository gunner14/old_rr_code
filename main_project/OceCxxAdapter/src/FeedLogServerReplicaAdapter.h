/*************************************************
 *
 *  @Copyright    Renren Inc.
 *  @Brief        FeedLogServer服务的ReplicaAdaptor
 *  @Filename     FeedLogServerReplicaAdapter.h 
 *  @Author       rui.qiu
 *  @Email        rui.qiu@renren-inc.com
 *  @Date         2012-11-16
 *  @History
 *
 *************************************************/

#ifndef __FEED_LOG_SERVER_REPLICA_ADAPTER_H__
#define __FEED_LOG_SERVER_REPLICA_ADAPTER_H__

#include "Singleton.h"
#include "AdapterI.h"
#include "FeedLogServer.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedLogServerReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedLogServerPrx>,
    public MyUtil::Singleton<FeedLogServerReplicaAdapter>{

public:
  FeedLogServerReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<FeedLogServerPrx> ("ControllerFeedLogServer", 120, 300, new FeedLogChannel), index_(0){
  }

  void PutLog(string log_name, string log);
  void PutLogBatch(string log_name, MyUtil::StrSeq log_seq);
  void PutLogsMap(LogsMap logs_map);

protected:
  FeedLogServerPrx getManager(int index);
  FeedLogServerPrx getManagerOneway(int index);

private:
  int index_;

  virtual string name() {
    return "FLS";
  }

  virtual string endpoints() {
    return "@FeedLogServer";
  }

};

};
};

#endif
