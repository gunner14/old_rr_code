#include "recommendation_feed_experimentI.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/friendrecommendmanager/common/config_parser2.h"
#include "socialgraph/friendrecommendmanager/builder/feed_recommend_builder.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&RecommendationFeedExperimentI::instance(), service.createIdentity("RFO", ""));

  ConnectionPoolManager::instance().initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("RecommendationFeedExperimentI.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("RecommendationFeedExperimentI.Interval", 5);

  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerRecommendationFeedExperimentI", 
					&RecommendationFeedExperimentI::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);

	//Get socialgraph configuration
	ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/recommendation_feed.conf");
}

//----------------------------------------------------    

namespace xce {
namespace socialgraph {

void RecommendationFeedExperimentI::AddRecommendationFeedAsync(int userid, 
																													 int limit, 
																													 const Ice::Current& current) {
  std::ostringstream oss;
  oss << "[RecommendationFeedExperimentI] AddRecommendationFeedAsync userid:" << userid << " limit:" << limit;

	MyUtil::TimeCost tc = MyUtil::TimeCost::create(oss.str(), MyUtil::TimeCost::LOG_LEVEL_INFO);    //time cost

	//FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
}

}}
