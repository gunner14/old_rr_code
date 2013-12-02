#include "sortbyclusterI.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "autogroupwithfree.h"
#include <OfferFriendsAdapter.h>
#include "OfferFriends.h" 
  
void MyUtil::initialize() { 
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();   
  service.getAdapter()->add(&SortByClusterI::instance(), service.createIdentity("SGBC", ""));

  ConnectionPoolManager::instance().initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("SortByCluster.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SortByCluster.Interval", 5);

  //向controller注册
	ServerStateSubscriber::instance().initialize("ControllerSortByCluster", &SortByClusterI::instance(), 
																								mod, sInterval, new MyUtil::SocialGraphChannel());
  ServerStateSubscriber::instance().setStatus(1);
}

namespace xce {
namespace socialgraph {

MyUtil::IntSeq SortByClusterI::GetRecommendByCluster(int userid, const MyUtil::IntSeq& idlist, int limit, const Ice::Current& current) {
	MCE_INFO("[SortByClusterI] GetRecommendByCluster userid:" << userid << " idlist size:" << idlist.size() << " limit:" << limit);
	AutoGroupPtr ptr = AutoGroupPtr(new AutoGroupWithFree());
  ptr->Grouping(idlist);

  return ptr->GetBestOffer(limit);
}
//添加了JS前端的排序算法 
MyUtil::IntSeq SortByClusterI::GetFinalRecommendByCluster(int userid, const MyUtil::IntSeq& idlist, int limit, const Ice::Current& current) {
	MCE_INFO("[SortByClusterI] GetFinalRecommendByCluster userid:" << userid << " idlist size:" << testIdlist.size() << " limit:" << limit);
  if (idlist.size() != 0) {
	  AutoGroupPtr ptr = AutoGroupPtr(new AutoGroupWithFree());
    ptr->Grouping(idlist);
    return ptr->GetFinalBestOffer(limit);
  } else {
    xce::offerfriends::OfferDataNSeq offers = xce::offerfriends::adapter::OfferFriendsCacheAdapter::instance().getBestOffer(userid, limit);
    MyUtil::IntSeq result;
    xce::offerfriends::OfferDataNSeq::iterator it = offers.begin();
    for ( ; it != offers.end(); it++) {
      result.push_back(it->userId); 
    }
	  MCE_INFO("[SortByClusterI] GetFinalRecommendByCluster userid: " << userid << " OfferFriends result size: " << result.size() << " limit: " << limit);
    return result;
  }
}

}
}
