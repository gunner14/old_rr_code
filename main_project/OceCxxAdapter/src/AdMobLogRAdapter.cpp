#include "AdMobLogRAdapter.h"

using namespace xce::ad;

AdMobLogRPrx AdMobLogRAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
}

AdMobLogRPrx AdMobLogRAdapter::getManager(int server_index){
	return getProxy(server_index);
}

void AdMobLogRAdapter::ReqLog(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->ReqLog(log);
}
