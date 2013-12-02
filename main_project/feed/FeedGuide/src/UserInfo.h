#ifndef USERINFO_H_
#define USERINFO_H_
#include <list>
#include <boost/circular_buffer.hpp>
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

#include <Date.h>
#include <TaskManager.h>
#include "SharedFeedCache.h"

#include "RFeed.h"
#include "FeedMemcProxy/client/feed_user_property.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "UserNameAdapter.h"
#include "OfferFriendsAdapter.h"
#include "XceLoggerAdapter.h"


using namespace xce::adapter::username;// UserNameAdapter
using namespace xce::offerfriends;
using namespace com::xiaonei::service;

using namespace std;
using namespace boost::multi_index;

namespace xce {
namespace feed {

class UserInfo;
typedef IceUtil::Handle<UserInfo> UserInfoPtr;


class GuideFeedLoadTask : public MyUtil::Task{
public:
	GuideFeedLoadTask(int uid , UserInfo * uinfo): uid_(uid), uinfo_(uinfo) {
		int types[] = {102, 103, 104, 110, 502};
		int len = sizeof(types)/sizeof(types[0]);
		fixed_types_.assign(types, types+len);
	};

	void handle();
	IntSeq GetCommonFriends(int uid) ;
	FeedItemSeq BuildFeedItem(FeedMetaSeq&);

private:
	int uid_;
	UserInfoPtr uinfo_ ;
	IntSeq fixed_types_;
};

// --------------------


class HeavyFeed /*: public Ice::Object */{
private:
	mutable int viewcnt_;
	mutable int time_;
	mutable int weight_;
	SharedFeedPtr sfeed_;
	bool marked_;
public:
	HeavyFeed(const FeedItem & item,const SharedFeedPtr & hdl):
			viewcnt_(0),time_(item.time),weight_(item.weight) ,sfeed_(hdl), marked_(false){}

//	virtual ~HeavyItem(){
//		MCE_INFO("Dtor. fid:" << GetFid() );
//	}
	long GetFid()const {return sfeed_->GetFid();}
	int GetStype()const{ return sfeed_->GetStype(); }
	long GetMerge() const { return sfeed_->GetMerge(); }
	int GetVersion() const { return sfeed_->GetVersion(); }

	SharedFeedPtr & GetSharedFeedPtr(){ return sfeed_;}

	pair<int,long> GetStypeMerge() const {	return sfeed_->GetStypeMerge(); }
	pair<int,long> GetVersionStypeMerge() const {	return sfeed_->GetVersionStypeMerge(); }
	pair<int,int> GetStypeActor() const { return sfeed_->GetStypeActor(); }
	pair<int,int> GetGatherKey() const {
		pair<int,int> res = sfeed_->GetStypeActor();
		if(IsGatherManyStype(0xFFFF & res.first))
			res.second = 0;
		return res;
	}
	time_t GetGatherTimeGap(int stype) const {
    if(IsGatherManyStype(0xFFFF&stype))
			return 60 * 60 * 24;
		else
			return 60 * 5;
	}

	bool SameStypeMerge(int stype,long merge) const { return sfeed_->SameStypeMerge(stype,merge);}
	bool SameStype(int stype) const { return sfeed_->SameStype(stype);}
	
  bool IsGatherManyStype(int stype) const { return stype == 2002 || stype == 3707 || stype== 8182;}
	bool IsUpdateTime() const { return weight_ & 0x02; }
	void SetUpdateTime(bool up){
    if (up)  weight_ = weight_ | 0x02;
    else  weight_ = weight_ & 0xFFFFFFFD;
	}

	bool IsRandomType()const{ return sfeed_->IsRandomType(); }

	int GetWeightId()const{ return weight_ >> 16 & 0xFF; }
	int GetFriendRank()const{ return (weight_ >> 24) & 0x7F;}
	bool IsFocusSource()const{ return weight_ >> 31;}
	bool IncWid(int uid,int stype) const {
		int btype = stype / 100;
		if(5 == btype || 6 == btype || 7 == btype )
			return (7 == uid % 100 ) ;

		return false;
	}

	//	int weight;	//高8位为 friendrank,次高8位为baseWeightId, 低16位为weight
	bool IsMarked() { return marked_; }
	void SetMarked(bool flag) { marked_ = flag; }

	int GetFriendPrefer(){
		return (weight_ >> 28) & 0x7;
	}
	bool IsSite(){
		int stype = sfeed_->GetStype();
		int btype = (stype & 0xFFFF) / 100 ;
		return btype == 37 or btype == 20;
	}

	double GetScore(int uid, bool sort, time_t nowtime){
		double score;
		int zone;
		int friendprefer =  GetFriendPrefer(); // guide == 7   other == 0

    if (true) {//if (uid%10 !=6 ) {
      if (friendprefer == 7) {
        SetMarked(true);
      }
      return GetTime();
    }

		if (IsSite() == true) {
			score = GetTime();
			zone = 3;
		} else if (friendprefer == 7) {
			score = GetTime() + nowtime;
			SetMarked(true);
			zone = 2;
		} else {
			score = GetTime() + 2 * nowtime;
			zone = 1;
		}

		if (uid == 263734453 ) {
//			MyUtil::Date date(time_);
//			MCE_INFO("@@.GS. u:" << UserNameAdapter::instance().getUserName( GetActor() )->name()
//					<< " fd:" << GetFid()
//					<< " t:" << GetStype()
//					<< " a:"<< GetActor()
//					<< " m:" << GetMerge()
//					<< " zone:" << zone
//					<< " score:" << score
//					<< " time:" << time_
//			);
		}
		return score;
	}

	int GetFeedNewsMergeType()const { return sfeed_->GetFeedNewsMergeType();}
	int IsMergeReplace()const {return sfeed_->IsMergeReplace();}
	int IsMergeAppend()const {return sfeed_->IsMergeAppend();}
	long GetTime() const  { return time_;}
	int GetActor() const { return sfeed_->GetActor() ;}

	void SetWeight(int weight){ weight_ = weight;}

	FeedItem ToFeedItem() const {
		FeedItem item;
		item.feed = sfeed_->GetFid();
		item.merge = sfeed_->GetMerge();
		item.type = sfeed_->GetOrigType();
		item.actor = sfeed_->GetActor();
		item.time = time_;
		item.weight = weight_;
		return item;
	}

	string ToString() const {
		ostringstream os;
		MyUtil::Date date(time_);
		os << " fid:" << GetFid()
				<< " t:" << GetStype()
				<< " time_:" << GetTime()
				<< " a:" << GetActor()
				<< " mg:" << GetMerge()
				<< " t:" << date.toDateTime();
		return os.str();
	}

};

inline static ostream &operator<<(ostream &stream, const HeavyFeed & i) {
	stream << "["<< i.ToString() <<"]";
  return stream;
}

//----------------------------------------------------

class HeavyFeed;
typedef HeavyFeed* HeavyFeedPtr;
typedef vector<HeavyFeedPtr> HFeed1D;
typedef vector<HFeed1D> HFeed2D;
typedef vector<HFeed2D> HFeed3D;

typedef map<pair<int, long> , HFeed1D> StypemergeHFeed1DMap;
typedef map<pair<int,long >, double> StypemergeScoreMap;
typedef multimap<double, StypemergeHFeed1DMap::iterator> ScoreIteratorMultiMap;

typedef map<pair<int, long> , SFeed1D> StypemergeSFeed1DMap;

class GetRequest;
class UserInfo: public Ice::Object {
private:
	typedef boost::circular_buffer<HeavyFeed> HBuf;
	HBuf hbuf_;//顺序是从大到小
	HBuf guide_hbuf_;//顺序是从大到小
	int uid_;
  int expire_date_;
	bool inited_;

	IceUtil::Mutex mutex_;
	IceUtil::Mutex guide_mutex_;

public:
	const static int kHbufSize = 250;
	const static int kGuideHbufSize = 110;
	friend class ResetHelper;
	friend class GuideFeedLoadTask;

	UserInfo(int uid):
			hbuf_(kHbufSize),
			guide_hbuf_(kGuideHbufSize),
			uid_(uid),
			inited_(false) {
        time_t seconds = time(NULL);
		    struct tm * local = localtime(&seconds);
		    expire_date_ =  local->tm_mday;
      }

//	virtual ~UserInfo(){
//		MaxFidUploader::instance().Add(uid_,max_fid_);
//	}
	bool Init(){
		if(inited_){
			MCE_WARN("UserInfo::Init. should be init once. uid:" << uid_);
			return true;
		}
    //if(!LoadFeedCache()) return false;

		LoadGuideFeedCache(); 
		inited_ = true;
		return true;
	}
	bool LoadFeedCache();
	bool LoadGuideFeedCache();
	void AddHeavyItem(int uid,HeavyFeed & hitem/*,bool updatemaxfid= false*/);

	void MergeAndSort(GetRequest & req);
	void MergeByStypeMergeAndGetScore(GetRequest & req,StypemergeScoreMap & stypemerge_score );

	//-----------
	vector<long> SetRead(int stype,long merge);
	vector<long> SetReadById(long fid);
	void SetReadAll();
	void SetReadAllFeedCache();
	void SetReadByIdsExactly(const set<long> & fids,bool remove_feedcache = true);

	int GetUid() const { return uid_;}
	int GetExpireDate() const { return expire_date_;}
	FeedItemSeq GetOrigFeedItems();
	int Size() const;
	string ToString()const;
	void DumpItems()const{
		BOOST_FOREACH(const HBuf::value_type & f,hbuf_){
			MCE_INFO(f.ToString());
		}
	};

	void GetSharedFeeds(FeedItemSeq & fitems);
	void AddBuffer4Guide();

  // 用30,29,28标示   7:guide  3:normal
	void SetFriendPrefer(FeedItemSeq & seq, bool guide) {
		for(FeedItemSeq::iterator it = seq.begin();
				it != seq.end(); ++it) {
			if (guide) it->weight |= 0x70000000;
			else {
				it->weight &= 0x00FFFFFF;
				it->weight |= 0x30000000;
			}
		}
	}

private:
	void MergeFeed(GetRequest & req);
	void GatherFeed(GetRequest & req);
	void GatherFeedSimple(GetRequest & req);

	void ReplaceScoreByCommentScore(GetRequest & req,StypemergeHFeed1DMap & comment_map_,
				StypemergeScoreMap & stypemerge_score );
	void MakeScoreIteratorMultiMap(
			GetRequest & req,StypemergeScoreMap & stypemerge_score);

	void GetSharedFeedsAndAddBuffer(FeedItemSeq & fitems,bool normalfeed);

//	IntSeq GetCommonFriends(int uid);
//	FeedItemSeq BuildFeedItem(FeedMetaSeq& metas);

	template<typename T>
	void SetReadInFeedCache(const T & fids);

	typedef map<pair<int, int> , pair<time_t, int> > StypeactorTimeindexMap;
	bool FindSameStypeActor(HeavyFeedPtr & item, StypeactorTimeindexMap & stypeactor_timeindex,
			StypeactorTimeindexMap::iterator& ti){
		pair<int,int> gatherkey = item->GetGatherKey();
		ti = stypeactor_timeindex.find(gatherkey);
		return (ti != stypeactor_timeindex.end());
	}
	bool TimeDiffLessThanLimit(time_t a,time_t b,int stype){
		if(stype == 8182)return true;
		static time_t LIMIT =  60 * 5;
		time_t diff = abs(a - b);
		return diff < LIMIT ;//TODO 根据不同类型有区别
	}

	bool CheckUniq();
	bool CheckOrder(FeedItemSeq & fitems);

};


//-------------------------------------------------------------------

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
	const static int kCacheSize = 100;
	const static int kMiCapacity = 3000;

	UserInfoMI mis_[kCacheSize];
	IceUtil::Mutex	mutexs_[kCacheSize];
public:
	void Put(int uid,UserInfoPtr & uinfo){
	  int index = GetIndex(uid);
		IceUtil::Mutex::Lock lock(mutexs_[index]);
	  UserInfoMI & mi = mis_[index];
	  pair<SeqIndex::iterator, bool> p = mi.push_front(uinfo);
	  if (!p.second) {
	  	mi.relocate(mi.begin(), p.first);
	  } else if ((int) mi.size() > kMiCapacity) {
	  	mi.pop_back();
	  }
	}
	UserInfoPtr Find(int uid,bool relocate = false){
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
		long total = 0;
		for(int i = 0; i < kCacheSize;++i){
			IceUtil::Mutex::Lock lock(mutexs_[i]);
		  const UserInfoMI & mi = mis_[i];
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


}
}


//--------------------------------
// socialgraph log
/*
 
 class LogStatsTask : public MyUtil::Task {
public:
  LogStatsTask(Ice::Int userid, const std::string& param, int level = 5):
      Task(level), userid_(userid), param_(param) {}

  static void InitLogPath();
  virtual void handle();

private:
  static com::xiaonei::service::XceLoggerPtr remote_logger_; 
  static std::vector<std::string> log_paths_;

  int userid_;
  std::string param_;
};

*/


#endif /* USERINFO_H_ */
