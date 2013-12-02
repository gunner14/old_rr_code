#include "AdEdmLogLeftAdapter.h"

using namespace xce::ad;

AdLogManagerPrx AdEdmLogLeftAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
}

AdLogManagerPrx AdEdmLogLeftAdapter::getManager(int server_index){
	return getProxy(server_index);
}

void AdEdmLogLeftAdapter::Click(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->ClickForBp(log);
}

void AdEdmLogLeftAdapter::CpcCharge(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->ClickForCharge(log);
}
