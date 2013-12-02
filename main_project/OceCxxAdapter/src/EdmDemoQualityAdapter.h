#ifndef AD_DEMOQUALITY_ADAPTER_H_
#define AD_DEMOQUALITY_ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "EdmDemoQuality.h"

namespace xce {
namespace ad {

using namespace MyUtil;

class EdmDemoQualityAdapter: public MyUtil::AdapterI,
    public MyUtil::AdapterISingleton<MyUtil::EdmChannel, EdmDemoQualityAdapter> {
public:
  EdmDemoQualityAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }

  EdmQualityMap GetEdmPool(int sindex){
    return getManager(getsindex(sindex))->GetEdmPool();
  }
  EdmQualityMap GetAdEdmPool(int sindex){
    return getManager(getsindex(sindex))->GetAdEdmPool();
  }
private:
  EdmDemoQualityManagerPrx getManager(int id){
      return locate<EdmDemoQualityManagerPrx> (_managers, "M", id, TWO_WAY);
  }
  EdmDemoQualityManagerPrx getManagerOneway(int id){
      return locate<EdmDemoQualityManagerPrx> (_managersOneway, "M", id, ONE_WAY);
  }

  /*int getsindex(int sindex) {
    if(sindex == 18) {
      return 1;
    }else if(sindex == 19) {
      return 2;
    }else if(sindex == 20) {
      return 3;
    }else if(sindex == 21) {
      return 4;
    }else if (sindex == 5 || sindex == 6 || sindex == 7 || sindex == 8 || sindex == 9 || sindex == 10 || sindex == 11 || sindex == 17 ){
      return 5;
    }
    else{
      return 0;
    }
  }*/
  
  //move to cpp
  int getsindex(int sindex);
/*  int getsindex(int sindex) {
    if(sindex <= 17) {
      return 0;
    } else if( sindex >= 18 && sindex <= 21){
      return 1;
    } else if(sindex >=22 && sindex <=25) {
      return 2;
    } else if(sindex >=26 && sindex <=35) {
      return 3;
    } else if(sindex >=36 && sindex <=45) {
      return 4;
    } else {
      return 0;
    }
  }*/
  virtual string name() {
    return "M";
  }
  virtual string endpoints() {
    return "@EdmDemoLoaderCache";
  }
  virtual size_t cluster() {
    return 10; 
  }

  vector<EdmDemoQualityManagerPrx> _managersOneway;
  vector<EdmDemoQualityManagerPrx> _managers;
};

}
}


#endif // AD_DEMOQUALITY_ADAPTER_H_
