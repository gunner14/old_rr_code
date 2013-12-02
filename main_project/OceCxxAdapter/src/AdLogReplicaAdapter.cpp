#include "AdLogReplicaAdapter.h"
namespace xce{
namespace ad{
AdLogManagerPrx AdLogReplicaAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
  //return locate<AdLogManagerPrx> (oneway_proxy_, "M", server_index, ONE_WAY);
}

AdLogManagerPrx AdLogReplicaAdapter::getManager(int server_index){
	return getProxy(server_index);
  //return locate<AdLogManagerPrx> (twoway_proxy_, "M", server_index, TWO_WAY);
}

void AdLogReplicaAdapter::Pv(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->Pv(log);
}

void AdLogReplicaAdapter::PvBySeq(const MyUtil::StrSeq & logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->PvBySeq(logs);
}

void AdLogReplicaAdapter::ClickForBp(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->ClickForBp(log);
}
void AdLogReplicaAdapter::ClickForCharge(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->ClickForCharge(log);
}
void AdLogReplicaAdapter::PvForCharge(const MyUtil::StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->PvForCharge(logs);
}
};
};
