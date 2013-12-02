#include "PingReceiverReplicaAdapter.h"
namespace xce{
namespace ad{
PingReceiverPrx PingReceiverReplicaAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
}

PingReceiverPrx PingReceiverReplicaAdapter::getManager(int server_index){
	return getProxy(server_index);
}

PingReceiverPrx PingReceiverReplicaAdapter::getManagerDatagram(int server_index){
 return getProxyDatagram(server_index);
}


void PingReceiverReplicaAdapter::WriteBatch(const xce::ad::ServerMethodSeq& methodRTimeSeq) {
  server_index_ = (server_index_ + 1) % getCluster();
  //PingReceiverPrx::uncheckedCast(getManagerOneway(server_index_)->ice_datagram())->WriteBatch(methodRTimeSeq);
  getManagerDatagram(server_index_)->WriteBatch(methodRTimeSeq);
}

};
};
