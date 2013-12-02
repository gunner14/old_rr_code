#ifndef __ADLOG_ADAPTER_H__
#define __ADLOG_ADAPTER_H__

#include "AdapterI.h"
#include "AdLog.h"

namespace xce{
namespace ad{
  using namespace MyUtil;
  class AdLogReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<AdLogManagerPrx>,
  public MyUtil::Singleton<AdLogReplicaAdapter>{
  public:
    AdLogReplicaAdapter() : MyUtil::ReplicatedClusterAdapterI<AdLogManagerPrx>("ControllerAdLog",120,300,new EdmChannel) ,server_index_(0) {
    }
    void Pv(const string& log);
    void PvBySeq(const MyUtil::StrSeq & logs);
    void ClickForBp(const string& log);
    void ClickForCharge(const string& log);
    void PvForCharge(const ::MyUtil::StrSeq& logs);
  protected:
    virtual string name(){
      return "AdLog";
    }
    virtual string endpoints(){
      return "@AdLog";
    }
    /*virtual size_t cluster(){
      return 3;
    }*/
    AdLogManagerPrx getManagerOneway(int server_index);
    AdLogManagerPrx getManager(int server_index);
  private:
    int server_index_;
    vector<AdLogManagerPrx> oneway_proxy_;
    vector<AdLogManagerPrx> twoway_proxy_;
  };
};
};
#endif
