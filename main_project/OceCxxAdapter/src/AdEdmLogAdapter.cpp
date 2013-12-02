#include "AdEdmLogAdapter.h"
namespace xce{
namespace ad{
EdmLogManagerPrx AdEdmLogAdapter::getManagerOneway(int server_index){
	//return getProxy(server_index);
 return locate<EdmLogManagerPrx> (oneway_proxy_, "M", server_index, ONE_WAY);
}

EdmLogManagerPrx AdEdmLogAdapter::getManager(int server_index){
	//return getProxy(server_index);
  return locate<EdmLogManagerPrx> (twoway_proxy_, "M", server_index, TWO_WAY);
}

EdmLogManagerPrx AdEdmLogAdapter::getManagerDatagram(int server_index){
	//return getProxyDatagram(server_index); 
 return locate<EdmLogManagerPrx> (oneway_proxy_, "M", server_index, DATAGRAM);
}
void AdEdmLogAdapter::Send(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->Send(log);
  //getManagerDatagram(server_index_)->Send(log);
}

void AdEdmLogAdapter::Click(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  getManagerOneway(server_index_)->Click(log);
  //getManagerDatagram(server_index_)->Click(log);
}

void AdEdmLogAdapter::Expose(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  //getManagerOneway(server_index_)->Expose(log);
  getManagerDatagram(server_index_)->Expose(log);
}

};
};
