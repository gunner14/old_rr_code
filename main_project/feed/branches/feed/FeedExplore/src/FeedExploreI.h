/*
 * FeedExploreI.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rong.tang
 */

#ifndef FEED_EXPLORE_I_H_
#define FEED_EXPLORE_I_H_

#include "FeedExplore.h"

#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
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

#include "Singleton.h"
#include "TaskManager.h"
#include "ServiceI.h"


using namespace std;
using namespace MyUtil;
using namespace xce::feed;
using namespace boost;
using namespace boost::multi_index;

namespace xce {
namespace feed {

class UserExploreStatus: public Ice::Object {
public:
  UserExploreStatus(int uid, int date): uid_(uid), date_(date) {};
  int GetUid() const {return uid_;}
  int GetDate() const {return date_;}
  void SetDate(int date) {date_ = date;}

private:
	int uid_;
	int date_;
};
typedef IceUtil::Handle<UserExploreStatus> UserExploreStatusPtr;


class ExploreStatusPool: public Singleton<ExploreStatusPool> {
	struct SeqTag{};
	struct UidTag{};

	typedef multi_index_container<
		UserExploreStatusPtr,
		indexed_by<
			sequenced<tag<SeqTag> > ,
			hashed_unique<tag<UidTag>, BOOST_MULTI_INDEX_CONST_MEM_FUN(UserExploreStatus, int, GetUid) >
		>
	>ExploreStatusMI;

	typedef ExploreStatusMI::index<SeqTag>::type SeqIndex;
	typedef ExploreStatusMI::index<UidTag>::type UidIndex;

private:
	const static int kCacheSize = 100;
	const static int kMiCapacity = 10000;

	ExploreStatusMI mis_[kCacheSize];
	IceUtil::Mutex	mutexs_[kCacheSize];

public:
	bool ShouldAdd(int uid);
	int GetUserDate(int uid);

private:
//	void UpdateDate(int uid)
	UserExploreStatusPtr Find(int uid, bool relocate = false) {
		int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);

		ExploreStatusMI & mi = mis_[index];
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
		}
		return NULL;
	}

	void Put(int uid) {
		UserExploreStatusPtr uinfo = new UserExploreStatus(uid, GetMDay());
		int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);

		ExploreStatusMI & mi = mis_[index];
		pair<SeqIndex::iterator, bool> p = mi.push_front(uinfo);
		if (!p.second) {//LRU
			mi.relocate(mi.begin(), p.first);
		} else if ((int) mi.size() > kMiCapacity) {
			mi.pop_back();
	  }
	}

	bool Erase(int uid) {
		int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);

		ExploreStatusMI & mi = mis_[index];
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
		os << " ---->ExploreStatusPool.    ";
		long total = 0;
		BOOST_FOREACH(const ExploreStatusMI & mi, mis_) {
			total += mi.size();
		}
		os << "total size: " << total;
		return os.str();
	}

//private:
	int GetIndex(long uid) {
		return abs(uid / 100 % kCacheSize);
	}

	int GetMDay() {
		time_t seconds = time(NULL);
		struct tm * local = localtime(&seconds);
		return local->tm_mday;
	}
};

//======================

class FeedExploreI: public FeedExplore, public MyUtil::Singleton<FeedExploreI> {
public:
	virtual void AddExploreFeed(int uid, const Ice::Current& = Ice::Current());
private:
	MyUtil::Str2StrMap CreateProperty(int uid, time_t time);
};




}
}
#endif /* FEEMARKI_H_ */

