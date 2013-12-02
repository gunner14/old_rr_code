#include "recommendation_feed_onlineI.h"

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
  service.getAdapter()->add(&RecommendationFeedOnlineI::instance(), service.createIdentity("RFO", ""));

  ConnectionPoolManager::instance().initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("RecommendationFeedOnline.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("RecommendationFeedOnline.Interval", 5);

  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerRecommendationFeedOnline", 
					&RecommendationFeedOnlineI::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);

	//Get socialgraph configuration
	ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/recommendation_feed.conf");
}

//----------------------------------------------------    

namespace xce {
namespace socialgraph {

void RecommendationFeedOnlineI::AddRecommendationFeedAsync(int userid, 
																													 int limit, 
																													 const Ice::Current& current) {
  std::ostringstream oss;
  oss << "[RecommendationFeedOnlineI] AddRecommendationFeedAsync userid:" << userid << " limit:" << limit;

	MyUtil::TimeCost tc = MyUtil::TimeCost::create(oss.str(), MyUtil::TimeCost::LOG_LEVEL_INFO);    //time cost
	//int value = ConfigParser::instance().GetConfigAsInt("F_O", "v3");
	//MCE_INFO("value : " << value);

	//FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
}

}}
