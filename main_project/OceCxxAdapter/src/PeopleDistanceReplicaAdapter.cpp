#include "PeopleDistanceReplicaAdapter.h"
//#include "IceExt/Channel.h"
using namespace com::xiaonei::search::distance;
using namespace MyUtil;

PeopleDistanceReplicaAdapter::PeopleDistanceReplicaAdapter():MyUtil::ReplicatedClusterAdapterI<PeopleDistancePrx>("ControllerPeopleDistance", 120, 300, new SearchChannel())
{ }

PeopleDistanceReplicaAdapter::~PeopleDistanceReplicaAdapter() { }

Int2FloatMap PeopleDistanceReplicaAdapter::Get(int a, const vector<int> & others) {
	PeopleDistancePrx proxy = getProxyOneway(0);
	if( proxy )  { 
		return proxy->Get(a,others);
	}  else  {
		MCE_WARN("PeopleDistanceReplicaAdapter::Get. proxy is not valid. a:" << a);
		return Int2FloatMap();
	}
}



