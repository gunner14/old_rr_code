#ifndef  _FRIENDRANK_CREATOR_H_
#define  _FRIENDRANK_CREATOR_H_

#include <string>
#include <FriendRankCacheNewAdapter.h>
#include <BuddyByIdReplicaAdapter.h>
#include <UserBasicAdapter.h>

#include "../common/Common.h"
#include "Ranker.h"
using namespace std;

class FriendRankCreator
{
	public:
		FriendRankCreator();
		virtual ~FriendRankCreator();
		int CreateFriendRank(const char* fname);

	private:
		int LoadActiveRankIndex(FILE* fp);
		bool GetNextStaticRank(FILE* fp);
		void SaveFriendRank(int uid, vector<RankScore>& result);
		void LoadWhiteUids();
		string GetUserName(int uid);

	private:
		FILE* fpFriendRank;
		FILE* fpActiveRank;

		Ranker* ranker;
		hash_map<USERID,long> mActiveRankPos;  // pair<active_rank,static_rank>
		
		ActiveRankHead headActiveRank;
		ActiveRank* dataActiveRank;
		StaticRankHead headStaticRank;
		StaticRank* dataStaticRank;
		set<int> whiteUids;
};

#endif

