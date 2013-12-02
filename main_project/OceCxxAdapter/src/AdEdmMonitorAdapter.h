#ifndef __AD_EDM_USER_TARGET_CACHE_ADAPTER_H__
#define __AD_EDM_USER_TARGET_CACHE_ADAPTER_H__

#include "AdapterI.h"
#include "AdEdmMonitor.h"

namespace xce{
namespace edm{

using namespace MyUtil;

class AdEdmMonitorAdapter : public MyUtil::ReplicatedClusterAdapterI<AdEdmMonitorPrx>,
  public MyUtil::Singleton<AdEdmMonitorAdapter>{
public:
  AdEdmMonitorAdapter() : MyUtil::ReplicatedClusterAdapterI<AdEdmMonitorPrx>("ControllerAdEdmMonitor",120,60,new EdmChannel) ,server_index_(0) {
    srand(time(NULL));
  }
  
  EdmSeq Filter(const EdmSeq& edmSeq);
    
protected:
  virtual string name(){
    return "ADEDM";
  }
  virtual string endpoints(){
    return "@AdEdmMonitor";
  }
  virtual size_t cluster(){
    return 1;
  }
  AdEdmMonitorPrx getManagerOneway(int server_index);
  AdEdmMonitorPrx getManager(int server_index);
private:
  int server_index_;
  vector<AdEdmMonitorPrx> oneway_proxy_;
  vector<AdEdmMonitorPrx> twoway_proxy_;
};

}
}
#endif
