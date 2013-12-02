#ifndef __AD_EDM_LOG_LEFT_REPLICA_ADAPTER_H__
#define __AD_EDM_LOG_LEFT_REPLICA_ADAPTER_H__

#include "AdapterI.h"
#include "AdLog.h"

namespace xce{
namespace ad{
using namespace MyUtil;
class AdEdmLogLeftAdapter : public MyUtil::ReplicatedClusterAdapterI<AdLogManagerPrx>,
  public MyUtil::Singleton<AdEdmLogLeftAdapter>{
public:
  AdEdmLogLeftAdapter() : MyUtil::ReplicatedClusterAdapterI<AdLogManagerPrx>("ControllerAdEdmLogLeft",120,300,new EdmChannel),server_index_(0) {
  }

  void Click(const string& log);

  void CpcCharge(const string& log);
protected:
  virtual string name(){
    return "AdEdmLogLeft";
  }
  virtual string endpoints(){
    return "@AdEdmLogLeft";
  }
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
