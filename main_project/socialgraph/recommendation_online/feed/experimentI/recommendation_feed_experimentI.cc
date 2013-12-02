#include "recommendation_feed_experimentI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/recommendation_online/common/config_parser2.h"
#include "socialgraph/recommendation_online/builder/feed_impl/feed_recommend_builder.h"

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

RecommendItemSeq RecommendationFeedExperimentI::GetRecommendationByFeed(int userid,
																																				int limit, 
																																				const Ice::Current& current) {
  std::ostringstream oss;
  oss << "[RecommendationFeedExperimentI] GetRecommendationByFeed userid:" << userid << " limit:" << limit;

	MyUtil::TimeCost tc = MyUtil::TimeCost::create(oss.str(), MyUtil::TimeCost::LOG_LEVEL_INFO);    //time cost
        RecommendItemSeq item_list = FeedRecommendBuilder::instance().GetRecommendData(userid, limit);
        if (!item_list.empty()) {
		MCE_INFO("user_id : " << userid << " item_list.size() : " << item_list.size() << "\n");
                //if (limit < 15) {
		//	item_list.resize(limit);
		//}
		//if (limit > 15) {
		//	item_list.resize(15);
		//}
        	//CreateFeedHelper::CreatePageFeedRecommendation(userid, item_list);
	}
	return item_list;
}

}}
