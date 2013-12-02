/*
 * QueryCache.h
 *
 *  Created on: 2012-8-29
 *      Author: cm
 */

#ifndef QUERYCACHE_H_
#define QUERYCACHE_H_

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "boost/foreach.hpp"
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
#include "UserPostings.h"

typedef boost::shared_ptr<UserPostings> UserPostingsPtr;
class UserPostingsPool {
	struct SeqTag{};
	struct UidTag{};

	typedef boost::multi_index_container <
			UserPostingsPtr,boost::multi_index::indexed_by<
			boost::multi_index::sequenced<boost::multi_index::tag<SeqTag> > ,
			boost::multi_index::hashed_unique<boost::multi_index::tag<UidTag>,BOOST_MULTI_INDEX_CONST_MEM_FUN(UserPostings,int,GetUid) >
			>
			>UPostings;
	typedef UPostings::index<SeqTag>::type SeqIndex;
	typedef UPostings::index<UidTag>::type UidIndex;

public:
	 UserPostingsPool(map<string,string>& config);
	 UserPostingsPtr Find(int uid,bool relocate = true);
	 void Insert(int uid,UserPostingsPtr& up);
	 bool Erase(int uid);

private:
	 int kCacheSize;
	 int kMiCapacity;

	UPostings mis_[kCacheSize];
	boost::mutex mutexs_[kCacheSize];
	int GetIndex(long uid){
		return abs(uid / 100 % kCacheSize);
	}
};

#endif /* QUERYCACHE_H_ */
