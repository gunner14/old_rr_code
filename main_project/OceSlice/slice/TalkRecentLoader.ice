#ifndef __TALK_RECENT_LOADER_ICE__
#define __TALK_RECENT_LOADER_ICE__

#include <TalkCommon.ice>

module com
{
module xiaonei
{
module talk
{
module recent 
{
module loader
{

	class TalkRecentUser
	{
		int id;
    int type;
		string subject;
		string info;
	};

	sequence<TalkRecentUser> TalkRecentUserSeq;
  dictionary<int, TalkRecentUser> TalkRecentUserMap;
  dictionary<int, TalkRecentUserSeq> TalkRecentUserSeqMap;

	class RecentLoaderManager{
		void Update(int actor, TalkRecentUser groupinfo);
    void Delete(int actor, int id, int type);
		TalkRecentUserSeq LoadById(int owner);
		TalkRecentUserSeqMap LoadBySeq(MyUtil::IntSeq owners);
	};

};
};
};
};
};

#endif
