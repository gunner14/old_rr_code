#include "recommendationexperimentIII.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/recommendation_online/common/config_parser.h"
#include "socialgraph/recommendation_online/builder/home_impl/friend_recommend_builder.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&RecommendationExperimentIII::instance(), service.createIdentity("REIII", ""));
    
  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("RecommendationExperimentIII.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("RecommendationExperimentIII.Interval", 5);
  
  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerRecommendationExperimentIII",
          &RecommendationExperimentIII::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());
  
  ServerStateSubscriber::instance().setStatus(1);

	//Get socialgraph configuration
	ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/people_recommend.conf");
}

//----------------------------------------------------  

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentIII::GetFriendRecommendByHome(int userid, 
																																		 int limit, const Ice::Current& current) {
  MCE_INFO("[RecommendationExperimentIII] GetFriendRecommendByHome userid:" << userid << " limit:" << limit);

	return FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
}

}}
