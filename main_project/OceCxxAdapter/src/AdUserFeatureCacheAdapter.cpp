#include "AdUserFeatureCacheAdapter.h"

using namespace std;
using namespace MyUtil;
using namespace xce::ad;
using namespace boost;

const int BASE_UNIT = 100000;

AdUserFeatureCachePrx AdUserFeatureCacheAdapter::getManagerOneway(int server_index){
  return getProxyOneway(server_index);
}

AdUserFeatureCachePrx AdUserFeatureCacheAdapter::getManager(int server_index){
  return getProxy(server_index);
}

string AdUserFeatureCacheAdapter::GetFeature(Ice::Long userID){
  int index = userID;
  if (userID < 10){
    index = rand() % BASE_UNIT; 
  } 
  index = index % getCluster();
  return getManager(index)->GetFeature(userID);
}

bool AdUserFeatureCacheAdapter::Rollback(const string& fileName){
  bool rollResult = true;
  for (int i = 0; i < getCluster(); ++i){
    if (!getManager(i)->Rollback(fileName)){
	rollResult = false;
 	MCE_WARN("AdUserFeatureCacheAdapter::Rollback the " << i << " sevice failed!");	
    } else {
	MCE_INFO("AdUserFeatureCacheAdapter::Rollback the " << i << " service success!");
    }//end if (!GetManager(i)->RollBack(fileName))
  }//end for (int i = 0; i < getCluster(); ++i)
  
  return rollResult;
}
