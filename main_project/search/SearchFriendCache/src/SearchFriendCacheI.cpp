/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "SearchFriendCacheI.h"
#include "LogWrapper.h"
#include "ServiceI.h"
#include "TaskManager.h"
#include "QueryRunner.h"
#include "boost/foreach.hpp"
#include "PeopleDistanceReplicaAdapter.h"
#include "BuddyFriendsAdapter.h"
#include "BuddyByIdReplicaAdapter.h"
#include "FeedNaiveBayesReplicaAdapter.h"
#include "UserNameAdapter.h"
#include "Ice/Object.h"
#include <map>
#include <vector>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"


using namespace xce::feed;
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace boost;
using namespace xce::buddy::adapter;
using namespace xce::buddy;
//using namespace xce::usercache;
using namespace xce::adapter::username;
using namespace com::renren::search::friendcache;
using namespace boost::multi_index;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&SearchFriendCacheI::instance(), service.createIdentity("M", ""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt( "SearchFriendCache.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault( "SearchFriendCache.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize( "ControllerSearchFriendCache", &SearchFriendCacheI::instance(), mod, interval,
			    new SearchControllerChannel());
    	xce::serverstate::ServerStateSubscriber::instance().setStatus(1);
}
class TimeStat {
public:
	TimeStat() {  reset(); }
	void reset() { gettimeofday(&_begin, NULL); }
	float Get(bool clear = true) {
		gettimeofday(&_end, NULL);
		float timeuse = 1000000 * (_end.tv_sec - _begin.tv_sec)
			+( _end.tv_usec - _begin.tv_usec);
		if(clear) reset();
		return timeuse / 1000;
	}
private:
	timeval _begin, _end;
};
class UserInfoPool: public Singleton<UserInfoPool>{
	struct SeqTag{};
	struct UidTag{};
	typedef multi_index_container<
		UserInfoPtr,
		indexed_by<
			sequenced<tag<SeqTag> > ,
			hashed_unique<tag<UidTag>,BOOST_MULTI_INDEX_CONST_MEM_FUN(UserInfo,int,GetUid) >
			>
			>UserInfoMI;
	typedef UserInfoMI::index<SeqTag>::type SeqIndex;
	typedef UserInfoMI::index<UidTag>::type UidIndex;

	private:
	const static int kCacheSize = 20;
	const static int kMiCapacity = 500;

	UserInfoMI mis_[kCacheSize];
	IceUtil::Mutex	mutexs_[kCacheSize];
	public:
	//void Put(int uid,UserInfoPtr & uinfo){
	//	int index = GetIndex(uid);
	//	IceUtil::Mutex::Lock lock(mutexs_[index]);
	//	UserInfoMI & mi = mis_[index];
	//	pair<SeqIndex::iterator, bool> p = mi.push_front(uinfo);
	//	if (!p.second) {
	//		mi.relocate(mi.begin(), p.first);
	//	} else if ((int) mi.size() > kMiCapacity) {
	//		mi.pop_back();
	//	}
	//}
	UserInfoPtr Find(int uid,bool relocate = true){
		int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);
		UserInfoMI & mi = mis_[index];
		UidIndex& uid_index = mi.get<UidTag> ();
		UidIndex::iterator it = uid_index.find(uid);
		if (it != uid_index.end()) {
			if (relocate) {
				SeqIndex& seq_index = mi.get<SeqTag> ();
				seq_index.relocate(seq_index.begin(), mi.project<SeqTag> (it));
				it = uid_index.find(uid);
				if (it != uid_index.end()) {
					return (*it);
				}
			} else {
				return (*it);
			}
		} else {
			UserInfoPtr uinfo = new UserInfo(uid);
			pair<SeqIndex::iterator, bool> p = mi.push_front(uinfo);
			if (!p.second) {
				mi.relocate(mi.begin(), p.first);
			} else if ((int) mi.size() > kMiCapacity) {
				mi.pop_back();
			}
			return uinfo;
		}
		return NULL;
	}
	bool Erase(int uid){
		int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);
		UserInfoMI & mi = mis_[index];
		UidIndex& uid_index = mi.get<UidTag> ();
		UidIndex::iterator it = uid_index.find(uid);
		if (it != uid_index.end()) {
			uid_index.erase(it);
			return true;
		} else {
			return false;
		}
	}
	string ToString() const {
		ostringstream os;
		os << " UserInfoPool. ";
		int idx = 0;
		long total = 0;
		BOOST_FOREACH(const UserInfoMI & mi,mis_){
			//			os << " idx:" << idx << " count:" << mi.size();
			++idx;
			total += mi.size();
		}
		os << " total:" << total;
		return os.str();
	}
	private:
	int GetIndex(long uid){
		return abs(uid / 100 % kCacheSize);
	}
};


map<int,int> SearchFriendCacheI::GetSecond(int uid,const Ice::Current&){
	MCE_INFO("SearchFriendCacheI::GetSecond. DO NOT CALL THIS FUNCTION. uid:" << uid );
	return map<int,int>();
}
string GetIp(const Ice::Current& current){
	if(!current.con){
		MCE_WARN("Current con is false")
			return "NULL";
	}
	string ip = current.con->toString();
	size_t found = ip.find('\n');
	if (found == string::npos) {
		return ip;
	}
	string res ( ip.begin() + found + 1,ip.end());
	return res;
}


vector<int> SearchFriendCacheI::GetSecondFriends(int uid,const Ice::Current& cur){
	static int round = 0;
	if(++round%100 == 0){
		MCE_INFO("SearchFriendCacheI::GetSecondFriends. cache info cnt. " << UserInfoPool::instance().ToString() << " ip:" << GetIp(cur));
		//round = 0;
	}
	using namespace boost;
	TimeStat tc;
	UserInfoPtr uinfo = UserInfoPool::instance().Find(uid);
	if(!uinfo){
		MCE_INFO("SearchFriendCacheI::GetSecondFriends. Find fail. uid : " << uid );
		return vector<int> ();
	}
	vector<int> res = uinfo->Load();
	MCE_INFO("SearchFriendCacheI::GetSecondFriends. uid:" << uid << " res:" << res.size() << " cost:" << tc.Get() << " ip:" << GetIp(cur));
	return res;
}

vector<int> UserInfo::Load(){
	IceUtil::Mutex::Lock lock(mutex_);
	if(loaded_ && !IsExpired()){
		return secondfriends_;	
	}

	using namespace boost;
	TimeStat ts;
	vector<int> first_friends = BuddyFriendsCacheAdapter::instance().getFriendListAsc(uid_,0,-1);
	float first_cost = ts.Get();
	//MyUtil::Int2IntSeqMap fof = BuddyFriendsCacheAdapter::instance().getFriendLists(first_friends);
	MyUtil::Int2IntSeqMap fof =BuddyByIdCacheAdapter::instance().getFriendLists(first_friends);
	float second_cost = ts.Get();

	set<int> first_set(first_friends.begin(),first_friends.end());
	first_set.insert(uid_);

	map<int,int> fof_common;
	BOOST_FOREACH(Int2IntSeqMap::value_type p,fof){
		BOOST_FOREACH(int f,p.second){
			if(!first_set.count(f)){
				fof_common[f]++;
				//secondfriends.insert(f);
				//secondfriends[f]++;
			}		
		}
	}
	multimap<int,int> common2friend;
	typedef map<int,int>::value_type mvt;
	BOOST_FOREACH(mvt & p,fof_common){
		common2friend.insert(make_pair(p.second,p.first));	
	}
	typedef multimap<int,int>::value_type mmvt;
	int cnt = 0 ;
	vector<int> secondfriends_swap;
	BOOST_REVERSE_FOREACH(mmvt & p , common2friend){
		const static int kSize = 100000;
		if(cnt++ >= kSize)
			break;
		//secondfriends.insert(p.first);
		secondfriends_swap.push_back(p.second);
	}
	timestamp_ = time(0);
	loaded_ = true;
	MCE_INFO("UserInfo::Load. uid:" << uid_
			<< " first_friends:" << first_friends.size()
			<< " fof:" << fof.size()
	//		<< " fof_common:" << fof_common.size()
			<< " secondfriends:" << secondfriends_swap.size()
			<< " first_cost:" << first_cost
			<< " second_cost:" << second_cost
			);

	secondfriends_swap.swap(secondfriends_);
	return secondfriends_;
	//return true;
}
