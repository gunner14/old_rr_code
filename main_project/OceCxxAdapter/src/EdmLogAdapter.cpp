#include "EdmLogAdapter.h"
namespace xce{
namespace ad{
AdLogManagerPrx EdmLogAdapter::getManagerOneway(int server_index){
	//return getProxy(server_index);
 return locate<AdLogManagerPrx> (oneway_proxy_, "M", server_index, ONE_WAY);
}

AdLogManagerPrx EdmLogAdapter::getManager(int server_index){
	//return getProxy(server_index);
  return locate<AdLogManagerPrx> (twoway_proxy_, "M", server_index, TWO_WAY);
}

AdLogManagerPrx EdmLogAdapter::getManagerDatagram(int server_index){
	//return getProxyDatagram(server_index); 
 return locate<AdLogManagerPrx> (oneway_proxy_, "M", server_index, DATAGRAM);
}
void EdmLogAdapter::Pv(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  //getManagerOneway(server_index_)->Pv(log);
  getManagerDatagram(server_index_)->Pv(log);
}

void EdmLogAdapter::PvBySeq(const MyUtil::StrSeq & logs){
  server_index_ = (server_index_ + 1) % getCluster();
  //getManagerOneway(server_index_)->PvBySeq(logs);
  getManagerDatagram(server_index_)->PvBySeq(logs);
}

void EdmLogAdapter::ClickForBp(const string& log){
  server_index_ = (server_index_ + 1) % getCluster();
  //getManagerOneway(server_index_)->ClickForBp(log);
  getManagerDatagram(server_index_)->ClickForBp(log);
}

};
};
