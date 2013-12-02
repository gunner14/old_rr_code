#include "FriendRankCacheNewAdapter.h"
#include <BuddyByIdReplicaAdapter.h>
using namespace xce::buddy::adapter;

using namespace std;
using namespace MyUtil;
using namespace xce::socialgraph;

FriendRankCacheAdapter::~FriendRankCacheAdapter()
{
}

bool FriendRankCacheAdapter::isValid(const Ice::ObjectPrx& proxy)
{
	return true;
}

RankSeqIntNew FriendRankCacheAdapter::GetRankWithFactor(int userId, float factor)
{
        try {
                return getProxy(userId)->GetRankWithFactor(userId, factor);
        } catch (Ice::TimeoutException& e) {
                MCE_ERROR("GetRank timeout exception" << e);
        } catch (Ice::Exception& e) {
                MCE_ERROR("GetRank ice::exception " << e.what());
        } catch (...) {
                MCE_ERROR("GetRank exception");
        }
        return RankSeqIntNew();
}

RankSeqNew FriendRankCacheAdapter::GetRank(int userId)
{
	try {
		return getProxy(userId)->GetRank(userId);
	} catch (Ice::TimeoutException& e) {
		MCE_ERROR("GetRank timeout exception" << e);
	} catch (Ice::Exception& e) {
		MCE_ERROR("GetRank ice::exception " << e.what());
	} catch (...) {
		MCE_ERROR("GetRank exception");
	}
	return RankSeqNew();
}

RankMapNew FriendRankCacheAdapter::GetRevRank(int userId)
{
	RankMapNew rankmap;
	try {
		vector<int> userFriend = 
			BuddyByIdCacheAdapter::instance().getFriendListAsc(userId,2000);
		for (int i=0; i<10; ++i)
		{
			vector<int> guestIds;
			for (vector<int>::const_iterator iter = userFriend.begin();
					iter != userFriend.end(); ++iter)
			{
				if (*iter % 10 == i)
					guestIds.push_back(*iter);
			}
			if (guestIds.size()>0)
			{
				RankMapNew thisrank = getProxy(i)->GetRevRank(userId, guestIds);
				for (RankMapNew::const_iterator iter = thisrank.begin();
						iter != thisrank.end(); ++iter)
				{
					rankmap.insert(*iter);
				}
			}
		}
	} catch (Ice::TimeoutException& e) {
		MCE_ERROR("GetRevRank timeout exception" << e);
	} catch (Ice::Exception& e) {
		MCE_ERROR("GetRevRank ice::exception " << e.what());
	} catch (...) {
		MCE_ERROR("GetRevRank exception");
	}
	return rankmap;
}

void FriendRankCacheAdapter::SetRank(int userId, const FriendRankDataNewPtr& list)
{
	try {
		getProxy(userId)->SetRank(userId, list);
	} catch (Ice::TimeoutException& e) {
		MCE_ERROR("SetRank timeout exception" << e);
	} catch (Ice::Exception& e) {
		MCE_ERROR("SetRank ice::exception " << e.what());
	} catch (...) {
		MCE_ERROR("SetRank exception");
	}
}
void FriendRankCacheAdapter::Load(const map<int,vector<RSscore> >& rankdatas)
{
	try {
		
		for (int i=0; i<10; ++i)
		{
			map<int,vector<RSscore> > thismod;
			for (map<int,vector<RSscore> >::const_iterator iter = rankdatas.begin();
					iter != rankdatas.end(); ++iter)
			{
				if (iter->first % 10 == i)
					thismod.insert(*iter);
			}
			if (thismod.size()>0)
			{
				getProxy(i)->Load(thismod);
			}
		}
	} catch (Ice::TimeoutException& e) {
		MCE_ERROR("Load timeout exception" << e);
	} catch (Ice::Exception& e) {
		MCE_ERROR("Load ice::exception " << e.what());
	} catch (...) {
		MCE_ERROR("Load exception");
	}
}

