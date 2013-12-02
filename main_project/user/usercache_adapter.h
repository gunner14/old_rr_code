#ifndef XCE_OLDSLICE_USERCACHE_ADAPTER_H__
#define XCE_OLDSLICE_USERCACHE_ADAPTER_H__

#include "arch/adapter2.h"
#include "OceSlice/slice/UserCache.h"

#include "OceCxxAdapter/src/UserCacheAdapter.h"

namespace xce {

//  SO@endpoint,MyUtil::SubjectObserverPrx->订阅”ClusterState“通知，opera(ClusterStateInfo{ServerStateMap ssm;})
//  M@endpoint,clustercontroller::ControllerManagerPrx, GetServerState() 返回 ClusterStateInfo结构
//inline xce::usercache::UserCacheManagerPrx CreateUserCacheAdapter(int uid) {
//  return Adapter::oce::Create<xce::usercache::UserCacheManagerPrx>("ControllerUserCacheN", uid);
//}


//inline xce::usercache::UserCacheManagerPrx CachedUserCacheAdapter(int uid) {
//  return Adapter::oce::Cached<xce::usercache::UserCacheManagerPrx>("ControllerUserCacheN", uid);
//}

inline xce::usercache::UserCacheAdapter& GetUserCacheAdapter() {
	return xce::usercache::UserCacheAdapter::instance();
}

}
#endif // XCE_OLDSLICE_USERCACHE_ADAPTER_H__
