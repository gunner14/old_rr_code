#include "recommendation_photofriend.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "socialgraph/recommendation_online/common/config_parser.h"
#include "socialgraph/recommendation_online/builder/home_impl/friend_recommend_builder.h"
#include "socialgraph/recommendation_online/builder/home_impl/MyLR/Feature.h"
#include "socialgraph/recommendation_online/builder/home_impl/MyLR/Model.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&RecommendationPhotoFriend::instance(), service.createIdentity("REPH", ""));
    
  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("RecommendationPhotofriend.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("RecommendationPhotofriend.Interval", 5);
  
  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerRecommendationPhotofriend",
          &RecommendationPhotoFriend::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());
  
  ServerStateSubscriber::instance().setStatus(1);

	//Get socialgraph configuration
//	ConfigParser::instance().Initialize("/data/xce/XSocialGraph/sg_conf/people_recommend.conf");
	//load the feat.map and the model.txt files.
	string modelFile = "/data/xce/XSocialGraph/sg_data/model.txt";
        string featFile = "/data/xce/XSocialGraph/sg_data/feat.map";
        Logistic::weightMap.Initialize(modelFile, 0, 1, "\t");
        FeatureMap::featMap.Initialize(featFile, 0, 1, "\t");
	MCE_WARN("printfeat:");
        for(map<string, int>::iterator iter = FeatureMap::featMap.GetInstance()->begin(); iter != FeatureMap::featMap.GetInstance()->end(); ++iter)
              MCE_WARN(iter->first<<":"<<iter->second);
}

//----------------------------------------------------  

namespace xce {
namespace socialgraph {

RecommendItemSeq RecommendationPhotoFriend::GetFriendRecommendByHome(int userid, 
																																		 int limit, const Ice::Current& current) {
  MCE_INFO("[RecommendationPhotoFriend] GetFriendRecommendByHome userid:" << userid << " limit:" << limit);

	return FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
}

}}
