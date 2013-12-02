#ifndef _DISTRIBUTION_MANAGER_H_
#define _DISTRIBUTION_MANAGER_H_

#include <Util.ice>
#include <RecommendationManager.ice>

module xce {
module socialgraph {

interface DistributionManager {
	RecommendItemSeq GetFriendRecommendByHome(int userid, int limit);		//recommendation in home
	void AddRecommendationFeedAsync(int userid, int limit);					//recommendation in newsfeed

	void ReloadBlackAndWhiteFile(int userid);
};

};
};

#endif
