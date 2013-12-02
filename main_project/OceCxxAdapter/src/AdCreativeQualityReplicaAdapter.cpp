#include "AdCreativeQuality.h"
#include "AdCreativeQualityReplicaAdapter.h"

using namespace MyUtil;
using namespace xce::ad;
using namespace boost;

AdCreativeQualityManagerPrx AdCreativeQualityReplicaAdapter::getManager(int id) {
	return getProxy(id);
}

AdCreativeQualityManagerPrx AdCreativeQualityReplicaAdapter::getManagerOneway(int id){
		return getProxyOneway(id);
}

AdQualityMap AdCreativeQualityReplicaAdapter::GetDemoPool(int index){
	int idx = random()%getCluster();
	MCE_DEBUG("CreativeQuality::GetDemoPool  Idx = " << idx);
	return getManager(idx)->GetDemoPool();
}

FreshAdQualityMap AdCreativeQualityReplicaAdapter::GetFreshAdMap(int index){
	int idx = random()%getCluster();
	MCE_DEBUG("CreativeQuality::GetFreshAdMap Idx = " << idx);
	return getManager(idx)->GetFreshAdMap();
}

