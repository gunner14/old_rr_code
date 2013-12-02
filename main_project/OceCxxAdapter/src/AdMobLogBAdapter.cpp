#include "AdMobLogBAdapter.h"

using namespace xce::ad;

AdMobLogPrx AdMobLogBAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
}

AdMobLogPrx AdMobLogBAdapter::getManager(int server_index){
	return getProxy(server_index);
}

void AdMobLogBAdapter::Pv(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->Pv(log);
}

void AdMobLogBAdapter::Pv(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->PvBatch(logs);
}

void AdMobLogBAdapter::Click(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->Click(log);
}

void AdMobLogBAdapter::Click(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->ClickBatch(logs);
}

void AdMobLogBAdapter::CpmCharge(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpmCharge(log);
}

void AdMobLogBAdapter::CpmCharge(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpmChargeBatch(logs);
}

void AdMobLogBAdapter::CpcCharge(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpcCharge(log);
}

void AdMobLogBAdapter::CpcCharge(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpcChargeBatch(logs);
}
