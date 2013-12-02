#include "AdExperiment.h"
#include "AdExperimentAdapter.h"

namespace xce{
namespace ad{

int AdExperimentAdapter::getsindex(int sindex) {
  
  if(sindex <= 16) {
    return 0;
  } else if(sindex >= 17 && sindex <= 33){
    return 1;
  } else if(sindex >= 34 && sindex <= 49) {
    return 2;
  } else {
    return 0;
  }

}

}
}
