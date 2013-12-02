#ifndef __AD_EDM_SENDER_ADAPTER_H__
#define __AD_EDM_SENDER_ADAPTER_H__

#include "AdapterI.h"
#include "AdEdmSender.h"

namespace xce{
namespace ad{
  using namespace MyUtil;
  class AdEdmSenderAdapter : public AdapterI, public AdapterISingleton<MyUtil::EdmChannel, AdEdmSenderAdapter>{
  public:
    AdEdmSenderAdapter() : server_index_(0) {
    }
    void Expose(Ice::Int user_id, Ice::Long adgroup_id, Ice::Int index);
  protected:
    virtual string name(){
      return "ADEDM";
    }
    virtual string endpoints(){
      return "@AdEdmSender";
    }
    virtual size_t getCluster(){
      return 1;
    }
    virtual size_t cluster(){
      return 1;
    }
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
