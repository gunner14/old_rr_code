#include "guide_recommend_contentI.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"

#include "commondbhelper.h"
#include "create_feed_helper.h"

void MyUtil::initialize() {
  using namespace xce::serverstate;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::socialgraph;

  ServiceI& service = ServiceI::instance();
 	service.getAdapter()->add(&GuideRecommendContentI::instance(), service.createIdentity("GRC", ""));

  ConnectionPoolManager::instance().initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("GuideRecommendContent.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("GuideRecommendContent.Interval", 5);

  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerGuideRecommendContent", 
					&GuideRecommendContentI::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);
}

//----------------------------------------------------  

namespace xce {
namespace socialgraph {

void GuideRecommendContentI::TriggerContentNewsFeed(int userid, int friendid, const Ice::Current& current) {
	MCE_INFO("[GuideRecommendContentI] TriggerContentNewsFeed userid(" << userid << ") friendid(" << friendid << ")");
	int limit = 30;				//number of recommendation feed
	std::vector<int> page_seq = CommonDbHelper::GetUserFollowPages(friendid, limit);
	//std::vector<int> site_seq = CommonDbHelper::GetUserFollowSites(friendid, limit);
	//std::map<int, std::string> result = CommonDbHelper::GetSiteUrls(site_seq);
	//CreateFeedHelper::CreateGuidePageRecommendation(userid, friendid, page_seq);
	//CreateFeedHelper::CreateGuideMinisiteRecommendationWithUrl(userid, friendid, result);
	//std::vector<int> page_seq = CommonDbHelper::GetUserFollowPages(friendid, limit);

	if (!page_seq.empty()) {
		CreateFeedHelper::CreateGuidePageRecommendation(userid, friendid, page_seq);
	} else {
		std::vector<int> site_seq = CommonDbHelper::GetUserFollowSites(friendid, limit);
		std::map<int, std::string> result = CommonDbHelper::GetSiteUrls(site_seq);
		if (!result.empty()) {
			CreateFeedHelper::CreateGuideMinisiteRecommendationWithUrl(userid, friendid, result);
		}
	}
}

void GuideRecommendContentI::Print(const std::map<int, std::string>& obj) {
	MCE_INFO("obj size : " << obj.size());
	for (std::map<int, std::string>::const_iterator iter = obj.begin();
			iter != obj.end(); ++iter) {
		MCE_INFO(iter->first << " : " << iter->second);
	}
}

}}
