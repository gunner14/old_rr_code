#include "IndexServiceReplicaAdapter.h"

namespace xce{
namespace ad{

IndexServicePrx IndexServiceReplicaAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
}

IndexServicePrx IndexServiceReplicaAdapter::getManager(int server_index){
	return getProxy(server_index);
}

void IndexServiceReplicaAdapter::Close(int uid, Ice::Long creative, bool env_test) {
  return getManagerOneway(GetServerIndex(uid, env_test))->Close(uid, creative);
}

int IndexServiceReplicaAdapter::trigger(const TriggerInputInfo &input, GroupDict &groups, bool env_test){
  return getManager(GetServerIndex(input.uid, env_test))->trigger(input, groups);
}


int IndexServiceReplicaAdapter::GetServerIndex(int uid, bool env_test) {
  if (uid <= 1000) {
    uid = rand() % 1001;
    MCE_DEBUG("IndexServiceReplicaAdapter::GetServerIndex: set uid use rand id = " << uid);
  }
  return  uid % getCluster();
}


};
};
