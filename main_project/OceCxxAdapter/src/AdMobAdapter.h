#ifndef __AD_MOB_ADAPTER_H__
#define __AD_MOB_ADAPTER_H__

#include "Singleton.h"
#include "AdapterI.h"
#include "AdMob.h"

namespace xce {
namespace ad {

using namespace MyUtil;

class AdMobAdapter: public MyUtil::ReplicatedClusterAdapterI<AdMobPrx>, 
  public MyUtil::Singleton<AdMobAdapter> { 
public:
  AdMobAdapter() : MyUtil::ReplicatedClusterAdapterI<AdMobPrx> ("ControllerAdMobEngine",120,300,new AdMobChannel) {
    srand(time(NULL));
  }

  AdMobResPara GetAds(const xce::ad::AdMobReqPara& para);
  AdMobResPara GetAdsForTest(const xce::ad::AdMobReqPara& para);
  void PvFeedBack(const xce::ad::AdMobFeedback& para);

private:
  AdMobPrx getManager(int id);
  AdMobPrx getManagerOneway(int id);
  int GetServerIndex(int uid);

  virtual string name() {
    return "M";
  }
  virtual string endpoints() {
    return "@AdMobEngine";
  }
  virtual size_t cluster() {
    return 10;
  }

  vector<AdMobPrx> _managersOneway;
  vector<AdMobPrx> _managers;
};

}
}


#endif
