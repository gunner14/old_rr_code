#include "recommendationexperimentIV.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/friendrecommendmanager/common/config_parser.h"
#include "socialgraph/friendrecommendmanager/builder/friend_recommend_builder.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&RecommendationExperimentIV::instance(), service.createIdentity("REIV", ""));
    
  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("RecommendationExperimentIV.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("RecommendationExperimentIV.Interval", 5);
  
  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerRecommendationExperimentIV",
          &RecommendationExperimentIV::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());
  
  ServerStateSubscriber::instance().setStatus(1);

	//Get socialgraph configuration
	ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/people_recommend.conf");
}

//----------------------------------------------------  

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationExperimentIV::GetFriendRecommendByHome(int userid, 
																																		 int limit, const Ice::Current& current) {
  MCE_INFO("[RecommendationExperimentIV] GetFriendRecommendByHome userid:" << userid << " limit:" << limit);

	return FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
}

}}
