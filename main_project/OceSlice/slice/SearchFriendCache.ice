#ifndef SEARCH_FRIEND_CACHE 
#define SEARCH_FRIEND_CACHE
#include <Util.ice>

module com
{
	module renren 
	{
		module search
		{
			module friendcache{
				dictionary<int,int> Int2IntMap;
				interface SearchFriendCache{
					Int2IntMap GetSecond(int uid);
					MyUtil::IntSeq GetSecondFriends(int uid);
				};
			};
		};
	};
};
#endif
