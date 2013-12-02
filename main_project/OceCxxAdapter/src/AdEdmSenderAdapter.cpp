#include "AdEdmSenderAdapter.h"
namespace xce{
namespace ad{
AdEdmSenderPrx AdEdmSenderAdapter::getManagerOneway(int server_index){
	//return getProxy(server_index);
 return locate<AdEdmSenderPrx> (oneway_proxy_, "ADEDM", server_index, ONE_WAY);
}

AdEdmSenderPrx AdEdmSenderAdapter::getManager(int server_index){
	//return getProxy(server_index);
  return locate<AdEdmSenderPrx> (twoway_proxy_, "ADEDM", server_index, TWO_WAY);
}

AdEdmSenderPrx AdEdmSenderAdapter::getManagerDatagram(int server_index){
	//return getProxyDatagram(server_index); 
 return locate<AdEdmSenderPrx> (oneway_proxy_, "ADEDM", server_index, DATAGRAM);
}

void AdEdmSenderAdapter::Expose(Ice::Int user_id, Ice::Long adgroup_id, Ice::Int index){
  server_index_ = (server_index_ + 1) % getCluster();
  //getManagerOneway(server_index_)->Expose(log);
  getManagerDatagram(server_index_)->Expose(user_id, adgroup_id, index);
}

};
};
