#include "AdEdmMonitorAdapter.h"

using namespace std;
using namespace MyUtil;
using namespace xce::edm;

const int BASE_UNIT = 10000;

AdEdmMonitorPrx AdEdmMonitorAdapter::getManagerOneway(int server_index){
  return getProxyOneway(server_index);
}

AdEdmMonitorPrx AdEdmMonitorAdapter::getManager(int server_index){
  return getProxy(server_index);
}

EdmSeq AdEdmMonitorAdapter::Filter(const EdmSeq& edmSeq){
  int index = rand() % getCluster();
  return getManager(index)->Filter(edmSeq);
}
