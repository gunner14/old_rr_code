#include <iostream>

#include <boost/foreach.hpp>

#include "FriendFinderFFWAdapter.h"

#include "RecommendationExperimentIAdapter.h"
#include "RecommendationExperimentIIAdapter.h"
#include "RecommendationExperimentIIIAdapter.h"
#include "RecommendationManangerAdapter.h"

#include "socialgraph/friendrecommendmanager/builder/base_builder.h"
#include "socialgraph/friendrecommendmanager/builder/friend_recommend_builder.h"
#include "DbCxxPool/src/QueryRunner.h"

using namespace xce::socialgraph;

int main(int argc, char* argv[]) {
	using namespace xce::socialgraph;
	using namespace com::xiaonei::xce;
	using namespace MyUtil;

	ConnectionPoolManager::instance().initialize();

	std::cout << "Experiment Test" << std::endl;

	int userid = 238111132;
	int limit = 10;

	//BaseBuilderPtr ptr(new FriendRecommendBuilder());
	//ptr->GetRecommendData(userid, limit);
	//RecommendationManangerAdapter::instance().GetFriendRecommendByHome(3, limit);	
	//RecommendationExperimentIIIAdapter::instance().GetFriendRecommendByHome(userid, limit);	
	//RecommendItemSeq seq = FriendRecommendBuilder::instance().GetRecommendData(userid, limit);

	//RecommendItemSeq seq = ptr->GetRecommendData(userid, limit);
	//RecommendItemSeq seq = FriendRecommendBuilder::instance().GetRecommendData(userid, limit);
	//BOOST_FOREACH (const RecommendItem& item, seq) {
	//	std::cout << item.id << " : " << item.count << " : " << item.type << std::endl;
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
