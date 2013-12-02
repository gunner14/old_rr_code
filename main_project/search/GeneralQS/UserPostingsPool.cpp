/*
 * QueryCache.cpp
 *
 *  Created on: 2012-8-29
 *      Author: cm
 */

#include "UserPostingsPool.h"

UserPostingsPtr UserPostingsPool::Find(int uid,bool relocate) {
		int index = GetIndex(uid);
		boost::mutex::scoped_lock lock(mutexs_[index]);
		UPostings &up = mis_[index];
		UidIndex& uid_index = up.get<UidTag> ();
		UidIndex::iterator it = uid_index.find(uid);
		if (it != uid_index.end()) {
			if (relocate) {
				SeqIndex& seq_index = up.get<SeqTag> ();
				seq_index.relocate(seq_index.begin(), up.project<SeqTag> (it));
				it = uid_index.find(uid);
				if (it != uid_index.end()) {
					return (*it);
				}
			} else {
				return (*it);
			}
		} else {
			return UserPostingsPtr();
		}
		return UserPostingsPtr();
	}

void UserPostingsPool::Insert(int uid, UserPostingsPtr& upostings) {
	int index = GetIndex(uid);
	boost::mutex::scoped_lock lock(mutexs_[index]);
	UPostings &up = mis_[index];
	pair<SeqIndex::iterator, bool> p = up.push_front(upostings);//push seq,check hash also
	if (!p.second) {//insert failure,exist
		up.relocate(up.begin(), p.first);
		*(up.begin()) = upostings;
	} else if ((int) up.size() > kMiCapacity) {//insert success,a newer not exist before
		up.pop_back();
	}
}

UserPostingsPool::UserPostingsPool(map<string,string>& config) {
	map<string,string>::iterator it = config.find("PARTITION_COUNT");
	if(it != config.end()) {
		kCacheSize = atoi(config["PARTITION_COUNT"].c_str());
	}
	it = config.find("CACHE_SIZE_PER_PART");
	if(it != config.end()) {
		kMiCapacity= atoi(config["CACHE_SIZE_PER_PART"].c_str());
	}
}


