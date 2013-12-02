#include "RmessageGateAdapter.h"

using namespace xce::notify;
using namespace MyUtil;

RmessageGatePrx RmessageGateAdapter::getManager(int id){
  return locate<RmessageGatePrx> (_managers, "M", id, TWO_WAY);
}

