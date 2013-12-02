/*
 * FeedMarkI.h
 *
 *  Created on: Feb 07, 2012
 *      Author: yejingwei
 */

#ifndef FEED_MARK_I_H_
#define FEED_MARK_I_H_

//#include <queue>
#include "FeedMark.h"
#include "Singleton.h"
#include "TaskManager.h"
//#include "FeedNaiveBayes.h"
//#include "ScoreCache.h"
#include "ServiceI.h"
//#include "FeedPreferCache.h"
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
#include <boost/foreach.hpp>

//#include "QueryRunner.h"


using namespace std;
using namespace MyUtil;
using namespace xce::feed;
using namespace boost;
using namespace boost::multi_index;

namespace xce {
namespace feed {

class UserMarkStatus: public Ice::Object {
public:
  UserMarkStatus(int uid): uid_(uid) {};
	int GetUid() const { return uid_; }
	
  void Add(int friendid, bool mark) {
		IceUtil::Mutex::Lock lock(mutex_);
		
    if (mark) {
			marks_.insert(friendid);
			unmarks_.erase(friendid);
		} else {
			marks_.erase(friendid);
			unmarks_.insert(friendid);
		}
	}

  bool IsSameMark(int friendid, bool mark) const {
		IceUtil::Mutex::Lock lock(mutex_);
    return mark ? marks_.count(friendid) : unmarks_.count(friendid);
  }

	MarkInfoPtr MakeMarkInfo() const {
		IceUtil::Mutex::Lock lock(mutex_);
		
    MarkInfoPtr info = new MarkInfo;
    info->marks.reserve(marks_.size());
		info->marks.assign(marks_.begin(), marks_.end());
    info->unmarks.reserve(unmarks_.size());
		info->unmarks.assign(unmarks_.begin(), unmarks_.end());
    return info;
  }

  string ToString() {
    ostringstream os;
    os << "User: " << uid_ << "  Has Marked: " << marks_.size() 
      << " Unmarked: " << unmarks_.size();
    return os.str();
  }

  void ShowMarkState() {
    ostringstream os;
    os << "  marks: ";
    BOOST_FOREACH(int mit, marks_) {
      os << mit << ",";
    }
    os << "         unmarks:";
    BOOST_FOREACH(int umit, unmarks_) {
      os << umit << ",";
    }
    MCE_INFO("---->FeedMarkI::ShowMarkState: " << uid_<< os.str());
  }

private:
	int uid_;
	set<int> marks_;
	set<int> unmarks_;

  IceUtil::Mutex mutex_;
};
typedef IceUtil::Handle<UserMarkStatus> UserMarkStatusPtr;

//=====================

class UserMarkStatusPool: public Singleton<UserMarkStatusPool> {
	struct SeqTag{};
	struct UidTag{};
	typedef multi_index_container<
		UserMarkStatusPtr,
		indexed_by<
			sequenced<tag<SeqTag> > ,
			hashed_unique<tag<UidTag>, BOOST_MULTI_INDEX_CONST_MEM_FUN(UserMarkStatus, int, GetUid) >
		>
	>UserMarkStatusMI;
	typedef UserMarkStatusMI::index<SeqTag>::type SeqIndex;
	typedef UserMarkStatusMI::index<UidTag>::type UidIndex;

private:
	const static int kCacheSize = 100;
	const static int kMiCapacity = 1000;

	UserMarkStatusMI mis_[kCacheSize];
	IceUtil::Mutex	mutexs_[kCacheSize];
public:
	void Put(int uid, UserMarkStatusPtr & uinfo) {
    int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);

	  UserMarkStatusMI & mi = mis_[index];
    pair<SeqIndex::iterator, bool> p = mi.push_front(uinfo);
	  if (!p.second) {//LRU
	  	mi.relocate(mi.begin(), p.first);
	  } else if ((int) mi.size() > kMiCapacity) {
	  	mi.pop_back();
	  }
	}

	UserMarkStatusPtr Find(int uid, bool relocate = false) {
	  int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);

	  UserMarkStatusMI & mi = mis_[index];
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

	bool Erase(int uid) {
	  int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);
	  
    UserMarkStatusMI & mi = mis_[index];
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
		os << "---->UserMarkStatusPool. Has User:   ";
		int idx = 0;
		long total = 0;
		BOOST_FOREACH(const UserMarkStatusMI & mi, mis_) {
			BOOST_FOREACH(const UserMarkStatusPtr & uminfo, mi) {
        os << uminfo->GetUid() << ", ";
      }
      ++idx;
			total += mi.size();
		}
		os << "total size: " << total << "\n";
		return os.str();
	}

private:
	int GetIndex(long uid) {
		return abs(uid / 100 % kCacheSize);
	}
};

//======================


class WriteTask: public MyUtil::Task {
public:
  WriteTask(int uid, int actor, bool mark) : 
      Task(), uid_(uid), actor_(actor), mark_(mark) {
  }
  ;

  virtual void handle() ;

private:
  int uid_, actor_;
  bool mark_;
};
//=====================

class FeedMarkI: public FeedMark, public MyUtil::Singleton<FeedMarkI> {
public:
  virtual void MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark, const Ice::Current& = Ice::Current());
  virtual MarkInfoPtr GetMarkInfo(int uid, const Ice::Current& = Ice::Current());

private:
  UserMarkStatusPtr Locate(int uid);
 
};




}
}
#endif /* FEEMARKI_H_ */
