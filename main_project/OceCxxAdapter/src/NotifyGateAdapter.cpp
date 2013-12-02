#include "NotifyGateAdapter.h"

using namespace xce::notify;
using namespace MyUtil;

NotifyGatePrx NotifyGateAdapter::getManager(int id){
  return locate<NotifyGatePrx> (_managers, "M", id, TWO_WAY);
}

StrSeq NotifyGateAdapter::getNotifySeq2(int uid, int view, long lastNotifyId, int limit){
  return getManager(uid)->getNotifySeq2(uid, view, lastNotifyId, limit);
}
StrSeq NotifyGateAdapter::getRMsgByBigtype(int uid, int view, 
    int bigtype, int begin, int limit) {
  return getManager(uid)->getRMsgByBigtype(uid, view, bigtype, begin, limit);
}
StrSeq NotifyGateAdapter::getRMsgByType(int uid, int view, 
    int type, int begin, int limit) {
  return getManager(uid)->getRMsgByType(uid, view, type, begin, limit);
}

