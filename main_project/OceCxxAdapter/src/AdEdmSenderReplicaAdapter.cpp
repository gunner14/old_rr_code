#include "AdEdmSenderReplicaAdapter.h"
namespace xce{
namespace ad{
AdEdmSenderPrx AdEdmSenderReplicaAdapter::getManagerOneway(int server_index){
	return getProxyOneway(server_index);
  //return getProxy(server_index);
 //return locate<AdEdmSenderPrx> (oneway_proxy_, "ADEDM", server_index, ONE_WAY);
}

AdEdmSenderPrx AdEdmSenderReplicaAdapter::getManager(int server_index){
  return getProxy(server_index);
  //return getProxy(server_index);
  //return locate<AdEdmSenderPrx> (twoway_proxy_, "ADEDM", server_index, TWO_WAY);
}

AdEdmSenderPrx AdEdmSenderReplicaAdapter::getManagerDatagram(int server_index){
	return getProxyDatagram(server_index); 
 //return locate<AdEdmSenderPrx> (oneway_proxy_, "ADEDM", server_index, DATAGRAM);
}

void AdEdmSenderReplicaAdapter::Expose(Ice::Int userid, Ice::Long adgroupid, Ice::Int index){
  if (userid <= 1000) {
    server_index_ = rand() % 1001;
  } else {
    server_index_ = userid;
  }
  server_index_ = server_index_ % getCluster();
  //getManagerOneway(server_index_)->Expose(userid, adgroupid, index);
  getManagerDatagram(server_index_)->Expose(userid, adgroupid, index);
}

};
};
