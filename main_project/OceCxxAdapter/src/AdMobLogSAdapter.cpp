#include "AdMobLogSAdapter.h"

using namespace xce::ad;

AdMobLogPrx AdMobLogSAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
}

AdMobLogPrx AdMobLogSAdapter::getManager(int server_index){
	return getProxy(server_index);
}

void AdMobLogSAdapter::Pv(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->Pv(log);
}

void AdMobLogSAdapter::Pv(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->PvBatch(logs);
}

void AdMobLogSAdapter::Click(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->Click(log);
}

void AdMobLogSAdapter::Click(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->ClickBatch(logs);
}

void AdMobLogSAdapter::CpmCharge(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpmCharge(log);
}

void AdMobLogSAdapter::CpmCharge(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpmChargeBatch(logs);
}

void AdMobLogSAdapter::CpcCharge(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpcCharge(log);
}

void AdMobLogSAdapter::CpcCharge(const StrSeq& logs){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->CpcChargeBatch(logs);
}
