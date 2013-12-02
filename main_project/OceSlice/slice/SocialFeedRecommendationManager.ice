#ifndef _FEED_RECOMMENDATION_MANAGER_H_
#define _FEED_RECOMMENDATION_MANAGER_H_ 

#include <Util.ice>

module xce {
module socialgraph {

struct RecommendItem {
	int id;						//id of item, userid in friend recommendation
	int count;				//count of common friends
	MyUtil::IntSeq commons;				//id list of common friends
	float weight;						//score of recommendation
	string type;						//type of item
	string experiment;					//item generated by
	MyUtil::StrSeq explanations;				//explanation of recommendation
};

sequence<RecommendItem> RecommendItemSeq;

interface SocialFeedRecommendationManager {
	  void  NotifySocialFeedUserLogin(int userid);
};

};
};

#endif