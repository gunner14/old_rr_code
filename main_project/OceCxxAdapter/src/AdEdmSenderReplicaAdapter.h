#ifndef __AD_EDM_SENDER_ADAPTER_H__
#define __AD_EDM_SENDER_ADAPTER_H__

#include "AdapterI.h"
#include "AdEdmSender.h"
#include "Singleton.h"

namespace xce{
namespace ad{
  using namespace MyUtil;
  class AdEdmSenderReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<AdEdmSenderPrx>,
  public MyUtil::Singleton<AdEdmSenderReplicaAdapter>{
  public:
    AdEdmSenderReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<AdEdmSenderPrx>("ControllerAdEdmSender",120,300,new EdmChannel),server_index_(0) {
      srand(time(NULL));
    }
    void Expose(Ice::Int userid, Ice::Long adgroupid, Ice::Int index);
  protected:
    virtual string name(){
      return "ADEDM";
    }
    virtual string endpoints(){
      return "@AdEdmSender";
    }
    /*virtual size_t getCluster(){
      return 1;
    }
    virtual size_t cluster(){
      return 1;
    }*/
    AdEdmSenderPrx getManagerOneway(int server_index);
    AdEdmSenderPrx getManager(int server_index);
    AdEdmSenderPrx getManagerDatagram(int server_index);
  private:
    int server_index_;
    vector<AdEdmSenderPrx> oneway_proxy_;
    vector<AdEdmSenderPrx> twoway_proxy_;
  };
};
};
#endif
