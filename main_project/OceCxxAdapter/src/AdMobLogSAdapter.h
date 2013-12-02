#ifndef __AD_MOB_LOG_S_ADAPTER_H__
#define __AD_MOB_LOG_S_ADAPTER_H__

#include "AdapterI.h"
#include "AdMobLog.h"

namespace xce{
namespace ad{
using namespace MyUtil;
class AdMobLogSAdapter : public MyUtil::ReplicatedClusterAdapterI<AdMobLogPrx>,
  public MyUtil::Singleton<AdMobLogSAdapter>{
public:
  AdMobLogSAdapter() : MyUtil::ReplicatedClusterAdapterI<AdMobLogPrx>("ControllerAdMobLogS",120,300,new AdMobChannel) ,server_index_(0) {
  }
  void Pv(const string& log);
  void Pv(const StrSeq& logs);

  void Click(const string& log);
  void Click(const StrSeq& log);

  void CpmCharge(const string& log);
  void CpmCharge(const StrSeq& log);

  void CpcCharge(const string& log);
  void CpcCharge(const StrSeq& log);
protected:
  virtual string name(){
    return "AdMobLogS";
  }
  virtual string endpoints(){
    return "@AdMobLogS";
  }
  AdMobLogPrx getManagerOneway(int server_index);
  AdMobLogPrx getManager(int server_index);
private:
  int server_index_;
  vector<AdMobLogPrx> oneway_proxy_;
  vector<AdMobLogPrx> twoway_proxy_;
};

};
};
#endif
