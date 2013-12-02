#ifndef __AD_MOB_LOG_R_ADAPTER_H__
#define __AD_MOB_LOG_R_ADAPTER_H__

#include "AdapterI.h"
#include "AdMobLogR.h"

namespace xce{
namespace ad{
using namespace MyUtil;
class AdMobLogRAdapter : public MyUtil::ReplicatedClusterAdapterI<AdMobLogRPrx>,
  public MyUtil::Singleton<AdMobLogRAdapter>{
public:
  AdMobLogRAdapter() : MyUtil::ReplicatedClusterAdapterI<AdMobLogRPrx>("ControllerAdMobLogR",120,300,new AdMobChannel) ,server_index_(0) {
  }
	void ReqLog(const string& log);
protected:
  virtual string name(){
    return "AdMobLogR";
  }
  virtual string endpoints(){
    return "@AdMobLogR";
  }
  AdMobLogRPrx getManagerOneway(int server_index);
  AdMobLogRPrx getManager(int server_index);
private:
  int server_index_;
  vector<AdMobLogRPrx> oneway_proxy_;
  vector<AdMobLogRPrx> twoway_proxy_;
};

};
};
#endif
