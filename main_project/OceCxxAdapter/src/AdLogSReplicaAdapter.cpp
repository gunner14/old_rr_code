#include "AdLogSReplicaAdapter.h"
namespace xce{
namespace ad{
AdLogManagerPrx AdLogReplicaAdapter::getManagerOneway(int index){
	return getProxyOneway(index);
}

AdLogManagerPrx AdLogReplicaAdapter::getManager(int index){
	return getProxy(index);
}

void AdLogReplicaAdapter::PvBatch(const ::MyUtil::StrSeq& logs) {
  index_= (index_+1)%getCluster();
  getManagerOneway(index_)->PvBatch(logs);
}

void AdLogReplicaAdapter::ClickForBp(const string& log){
  index_= (index_+1)%getCluster();
  getManagerOneway(index_)->ClickForBp(log);
}
void AdLogReplicaAdapter::ClickForCharge(const string& log){
  //getManagerOneway(0)->ClickForCharge(log);
  index_= (index_+1)%getCluster();
  getManagerOneway(index_)->ClickForCharge(log);
}
void AdLogReplicaAdapter::PvForCharge(const ::MyUtil::StrSeq& logs){
  //getManagerOneway(3)->PvForCharge(logs);
  index_= (index_+1)%getCluster();
  getManagerOneway(index_)->PvForCharge(logs);
}
};
};
