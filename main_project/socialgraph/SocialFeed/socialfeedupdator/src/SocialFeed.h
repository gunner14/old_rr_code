#ifndef _SOCIAL_FEED_H_
#define _SOCIAL_FEED_H_
//#include <Util.ice>
#include "Common.h"
#include "MD5Util.h"
//#include <SocialFeedCacheAdapter.h>

//last format before set to tcache
//namespace xce {
//namespace socialgraph {
   struct Feed {
        std::string          type;
//	MyUtil::IntSeq friendIds;
	std::vector<USERID>  friendIds;
	std::vector<USERID>  newCommonIds;
        std::vector<int>     commonNums; 
//	MyUtil::IntSeq newCommonIds;
        std::string          recommendation;
   };
// }
//}

//orignial format in process
class  SocialFeed
{
	public:
              //  SocialFeed(){};//for test 
		SocialFeed(USERID userid, const string type, const vector<USERID>& friends, const vector<USERID>& strangers,
			   const map<int,float>& offerfriend_sort);
		char GetType() const;
		long Finger()  const;
                void CommonNumbers(USERID userid);
		int  Transfer(Feed& feed) const;
		int  Print(USERID userid) const;
	public:
                string     _type;
		vector<pair<USERID,float> > _friends;
		vector<pair<USERID,float> > _strangers;
                vector<int> _common_numbers;
                string      _recommendation;
                bool        _notNormal;
};

//compared by weight in the feed itself
struct  FeedInnerComp {
	bool operator()(const pair<USERID,float>& p1, const pair<USERID,float>& p2)	{
	     return (p1.second > p2.second);
	}
};

//sort priority bettwen feeds: type > NoF > NoS 
struct  SocialFeedComp {
	bool operator()(const SocialFeed& p1, const SocialFeed& p2) {
		char typep1 = p1.GetType();
		char typep2 = p2.GetType();
		if (typep1 != typep2){
		   return (typep1 > typep2);
                }

		if (p1._friends.size() != p2._friends.size())
		   return (p1._friends.size() > p2._friends.size());
		
		float max_stranger_p1 = p1._strangers[0].second;
		float max_stranger_p2 = p2._strangers[0].second;
		return (max_stranger_p1 > max_stranger_p2);
	}
};

#endif

