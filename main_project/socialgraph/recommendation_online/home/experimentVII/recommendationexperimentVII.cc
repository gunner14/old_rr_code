#include "recommendationexperimentVII.h"

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
  service.getAdapter()->add(&RecommendationExperimentVII::instance(), service.createIdentity("REVII", ""));
    
  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("RecommendationExperimentVII.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("RecommendationExperimentVII.Interval", 5);
  
  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerRecommendationExperimentVII",
          &RecommendationExperimentVII::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());
  
  ServerStateSubscriber::instance().setStatus(1);

	//Get socialgraph configuration
	ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/people_recommend.conf");
}

//----------------------------------------------------  

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentVII::GetFriendRecommendByHome(int userid, 
																																		 int limit, const Ice::Current& current) {
  MCE_INFO("[RecommendationExperimentVII] GetFriendRecommendByHome userid:" << userid << " limit:" << limit);

	return FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
}

}}
