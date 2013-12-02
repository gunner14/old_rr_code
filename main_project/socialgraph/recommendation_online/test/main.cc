#include <iostream>

#include <boost/foreach.hpp>

#include "FriendFinderFFWAdapter.h"

#include "RecommendationExperimentIAdapter.h"
#include "RecommendationExperimentIIAdapter.h"
#include "RecommendationExperimentIIIAdapter.h"
#include "RecommendationExperimentIVAdapter.h"
#include "RecommendationExperimentVAdapter.h"
#include "RecommendationOnlineAdapter.h"
#include "RecommendationManangerAdapter.h"
#include "DistributionManagerAdapter.h"
#include "RecommendationFeedOnlineAdapter.h"
#include "RecommendationFeedExperimentIAdapter.h"

#include "socialgraph/recommendation_online/builder/base_builder.h"
#include "socialgraph/recommendation_online/builder/home_impl/friend_recommend_builder.h"
#include "DbCxxPool/src/QueryRunner.h"

using namespace xce::socialgraph;

int main(int argc, char* argv[]) {
	using namespace xce::socialgraph;
	using namespace com::xiaonei::xce;
	using namespace MyUtil;

	ConnectionPoolManager::instance().initialize();

	std::cout << "new Experiment Test" << std::endl;

	int userid = 238111132;
	int limit = -1;

	//BaseBuilderPtr ptr(new FriendRecommendBuilder());
	//ptr->GetRecommendData(userid, limit);
	//RecommendItemSeq seq = RecommendationExperimentVAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//RecommendItemSeq seq = RecommendationExperimentIAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//RecommendItemSeq seq = RecommendationExperimentIVAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//RecommendItemSeq seq = RecommendationManangerAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//RecommendItemSeq seq = RecommendationOnlineAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//RecommendItemSeq seq = DistributionManagerAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//DistributionManagerAdapter::instance().AddRecommendationFeedAsync(userid, limit);	
	//RecommendationExperimentIAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//RecommendItemSeq seq = FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
	//RecommendationFeedOnlineAdapter::instance().GetRecommendationByFeed(userid, limit);
	//RecommendationFeedExperimentIAdapter::instance().GetRecommendationByFeed(userid, limit);
	DistributionManagerPrx prx = DistributionManagerAdapter::instance().getProxy(userid);
	std::cout << prx << std::endl;
	try {
		DistributionManagerAdapter::instance().AddRecommendationFeedAsync(userid, limit);
	} catch (...) {
		std::cout << "Error error" << std::endl;	
	}

	//RecommendItemSeq seq = ptr->GetRecommendData(userid, limit);
	//RecommendItemSeq seq = FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
	//BOOST_FOREACH (const RecommendItem& item, seq) {
	//	std::cout << item.id << " : " << item.count << " : " << item.type << " : " << item.experiment << std::endl;
	//}
	//std::cout << "SGTest" << std::endl;

	//int userid = 238111132;
	//int limit = 100;

	//CommonFriendSeq seq = FriendFinderFFWAdapter::instance().GetCommonfriends(userid, limit);
	//for (CommonFriendSeq::iterator iter = seq.begin();
	//		iter != seq.end(); ++iter) {
	//	std::cout << "id:"	<< iter->userId << std::endl;
	//}
}
