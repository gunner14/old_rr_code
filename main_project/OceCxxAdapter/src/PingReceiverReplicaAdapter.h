#ifndef __PINGRECEIVER_REPLICA_ADAPTER_H__
#define __PINGRECEIVER_REPLICA_ADAPTER_H__

#include "AdapterI.h"
#include "PingReceiver.h"

namespace xce{
namespace ad{
  using namespace MyUtil;
  class PingReceiverReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<PingReceiverPrx>,
  public MyUtil::Singleton<PingReceiverReplicaAdapter>{
  public:
    PingReceiverReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<PingReceiverPrx>("ControllerPingReceiver",120,300,new AdMonitorChannel),
      server_index_(0) {}
    void WriteBatch(const xce::ad::ServerMethodSeq& methodRTimeSeq);
  protected:
    virtual string name(){
      return "PingReceiver";
    }
    virtual string endpoints(){
      return "@PingReceiver";
    }
    /*virtual size_t cluster(){
      return 3;
    }*/
    PingReceiverPrx getManagerOneway(int server_index);
    PingReceiverPrx getManager(int server_index);
    PingReceiverPrx getManagerDatagram(int server_index);
  private:
    int server_index_;
    vector<PingReceiverPrx> oneway_proxy_;
    vector<PingReceiverPrx> twoway_proxy_;
  };
};
};
#endif
