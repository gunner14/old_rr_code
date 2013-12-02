#ifndef USERINFO_H_
#define USERINFO_H_
#include <list>
#include "RFeed.h"
#include "Common.h"
#include "GetRequest.h"
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
#include <deque>
using namespace std;
using namespace boost::multi_index;
namespace xce {
namespace feed {


typedef map<pair<int, long> , HFeedPtr1D> StypemergeHFeed1DMap;
typedef map<pair<int,long >, double> StypemergeScoreMap;
typedef multimap<double, StypemergeHFeed1DMap::iterator> ScoreIteratorMultiMap;
typedef multimap<int,pair<int,long> > TimeStypeMergeMap;
typedef map<pair<int, long> , SFeedPtr1D> StypemergeSFeed1DMap;
typedef map<pair<int, int> , pair<time_t, int> > StypeactorTimeindexMap;

//class HeavyFeed : public Ice::Object {

class HeavyFeed {
private:
	mutable int viewcnt_;
	mutable long time_;
	mutable int weight_;
	SharedFeedPtr sfeed_;
	bool marked_;
public:
	HeavyFeed(const FeedItem & item,const SharedFeedPtr & hdl):
			viewcnt_(0),time_(item.time),weight_(item.weight) ,sfeed_(hdl),marked_(false){}

//	virtual ~HeavyItem(){
//		MCE_INFO("Dtor. fid:" << GetFid() );
//	}
	void Mark(bool mark){marked_ = mark;}
	bool IsMarked() const { return marked_;}
	long GetFid()const {return sfeed_->GetFid();}
	int GetStype()const{ return sfeed_->GetStype(); }
	long GetMerge() const { return sfeed_->GetMerge(); }
	int GetView()const{ return viewcnt_; }
	void IncView(){ ++viewcnt_;}
	void SetView(int v) { viewcnt_ = v;}
	SharedFeedPtr & GetSharedFeedPtr(){ return sfeed_;}

	pair<int,long> GetVersionStypeMerge() const {	return sfeed_->GetVersionStypeMerge(); }
	pair<int,long> GetStypeMerge() const {	return sfeed_->GetStypeMerge(); }
	pair<int,int> GetStypeActor() const { return sfeed_->GetStypeActor(); }
	bool SameStypeMerge(int stype,long merge) const { return sfeed_->SameStypeMerge(stype,merge);}
	time_t GetGatherTimeGap(int stype) const { return 60 * 5; }

	bool IsUpdateTime() const { return weight_ & 0x02; }
	void SetUpdateTime(bool up){
    if (up)  weight_ = weight_ | 0x02;
    else  weight_ = weight_ & 0xFFFFFFFD;
	}

	pair<int,int> GetGatherKey() const {
		pair<int,int> res = sfeed_->GetStypeActor();
		if(IsGatherManyStype(0xFFFF & res.first))
			res.second = 0;
		return res;
	}
	bool IsGatherManyStype(int stype) const { return stype == 2002 || stype == 3707 ;}

	bool IsEdm()const{ return (time_ < 0 ||time_ > time(NULL)); }
	bool IsRandomType()const{ return sfeed_->IsRandomType(); }

	int GetWeightId()const{ return weight_ >> 16 & 0xFF; }
	int GetFriendRank()const{ return (weight_ >> 24) & 0x7F; }
	bool IsFocusSource()const{ return weight_ >> 31;}

	double GetScore(int uid, bool sort)const{
		if(!sort) return max(GetTime(),(time_t)1);
		double frank;
		frank = GetFriendRank();
		return frank;
	}

	int GetFriendPrefer(){
	    return (GetFriendRank() >> 4) & 0x7;
	}

	int GetFeedNewsMergeType()const { return sfeed_->GetFeedNewsMergeType();}
	int IsMergeReplace()const {return sfeed_->IsMergeReplace();}
	int IsMergeAppend()const {return sfeed_->IsMergeAppend();}
	time_t GetTime() const  { return time_;}
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
		os << " fid:" << GetFid() << " type:" << GetStype() << " time_:" << time_ << " actor_:" << GetActor();
		return os.str();
	}
	int ResetEdmTime(){
		if(time_ > 0) return time_;

		//if(time_ == -1) time_ = time(NULL) + 8 * 60;TODO
		if(time_ == -1) time_ = time(NULL) + 20;
		else time_ = time(NULL) - time_;
		return time_;
	}

	bool IsUgc(){
		int stype = sfeed_->GetStype();
		int btype = stype / 100 ;
		return btype == 5 or btype == 6 or btype == 7;
	}

};

//typedef IceUtil::Handle<HeavyItem> HeavyItemPtr;
typedef HeavyFeed* HeavyFeedPtr;
typedef vector<HeavyFeedPtr> HFeedPtr1D;
typedef vector<HFeedPtr1D> HFeedPtr2D;
typedef vector<HFeedPtr2D> HFeedPtr3D;

typedef vector<HeavyFeed> HFeed1D;
class UserInfo;
typedef IceUtil::Handle<UserInfo> UserInfoPtr;

class MaxFidUploader : public Timer, public Singleton<MaxFidUploader>{
public:
	MaxFidUploader() : Timer(30*1000, 2){}
	void Add(int uid,long fid){
		IceUtil::Mutex::Lock lock(mutex_);
		map_[uid] = fid;
	}
  void handle();
private:
  map<int,long> map_;
	IceUtil::Mutex mutex_;
};

class GetRequest;
class UserInfo: public Ice::Object {
private:
	typedef boost::circular_buffer<HeavyFeed > HFeedBuf;
	typedef deque<HeavyFeed> HDeq;
	HFeedBuf sorted_;//排序过的
	HFeed1D edms_;//edms
//	HFeed1D comments_;//评论
	HDeq recent_;//新来的
	int uid_;
	long max_fid_;//内存中的最大id
	long pre_max_read_fid_;//上次看过的最大id
	bool inited_;
	IceUtil::Mutex mutex_;
	const static int kHbufSize = 250;
	map<time_t,long> refreshes_;
	map<int,bool> marked_friends_;
public:
	UserInfo(int uid):
			sorted_(kHbufSize),
			uid_(uid),
			max_fid_(0),
			pre_max_read_fid_(0),
			inited_(false)
	{	}
//	virtual ~UserInfo(){ MaxFidUploader::instance().Add(uid_,max_fid_); }
	bool Init();
	void GetFeeds(GetRequest & req);
	void MergeFeed(GetRequest & req);
	set<long> GetMarkedFids(const set<long> & fids);
	int GetUid() const { return uid_;}

	void LoadMark();
	bool LoadEdms();
	bool LoadFeedCache();

	void AddHeavyItem(int uid,HeavyFeed & hitem/*,bool updatemaxfid= false*/);

	vector<long> SetRead(int stype,long merge);
	vector<long> SetReadById(long fid);
	void SetReadAll();
	void SetReadByIdsExactly(const set<long> & fids,bool remove_feedcache = true);

	long DownloadMaxRead();
	void UploadMaxRead()const;
	long GetMaxRead()const;
	long GetPreMaxRead()const;
	void SetMaxRead(long fid);
	void RefreshPreMaxRead();
	void AddRefresh(time_t time,long fid);
	map<long,long> GetRefreshes() const ;
	void PushHFeedPtr1DToSorted(HFeedPtr1D & hfeedptr1d,bool mark);

	void GatherFeed(GetRequest & req);
	void GatherFeedSimple(GetRequest & req);

	void MarkFeed(long fid,long source,int stype,int actor,bool marked);
	FeedItemSeq GetOrigFeedItems();
	int GetUnviewCount(const set<int> & types);
	int Size() const;
	string ToString()const;
	void DumpSorted(){
//		MCE_INFO("UserInfo::DumpSorted. uid:" << uid_ << " sorted_ size:" << sorted_.size());
//		int idx = 0;
//		BOOST_FOREACH(HFeed1D & hfeed1d,sorted_){
//			ostringstream os;
//			BOOST_FOREACH(HeavyFeed & hfeed,hfeed1d){
//				os << " [fid:" << hfeed.GetFid() << " a:" << hfeed.GetActor() << " t:" << hfeed.GetStype() << " mg:" << hfeed.GetMerge()
//						<< " mk:" << hfeed.IsMarked() << "]";
//			}
//			MCE_INFO("\t" << idx++ << os.str());
//		}
	}
	void MakeScoreHFeedMap(multimap<double,HeavyFeed * > & score_map,time_t nowtime);
	bool FindSameStypeActor(HeavyFeedPtr & item,StypeactorTimeindexMap & stypeactor_timeindex,StypeactorTimeindexMap::iterator& ti){
		pair<int,int> gatherkey = item->GetGatherKey();
		ti = stypeactor_timeindex.find(gatherkey);
		return (ti != stypeactor_timeindex.end());
	}

	bool TimeDiffLessThanLimit(time_t a,time_t b){
		static time_t LIMIT =  60 * 5;
		time_t diff = abs(a - b);
		return diff < LIMIT ;//TODO 根据不同类型有区别
	}
private:
//	void MoveRecentToSorted();
	void MoveRecentToSorted(time_t nowtime);
	typedef map<pair<int,long>,HFeedPtr1D>  StypeMergeMap;
	typedef multimap<double,pair<int,HFeedPtr1D * > > ScoreMap;
	typedef multimap<int,HFeedPtr1D *> TimelineMap;
	void MakeStypeMergeMap(StypeMergeMap &,ScoreMap &);

	bool HandleEdm(GetRequest & req);

	void AddFeedItemsFromFeedCache(FeedItemSeq & fitems);
	void SeperateByPreMaxFid(FeedItemSeq & fitems,HDeq & next);
	int GetTopCount(int blocksize);

	long GetOldestFid();
	void ClearOldComments();
	void GetShowedComments(StypemergeSFeed1DMap& commentmap);
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
	const static int kMiCapacity = 1000;

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

class TypeFilter:public Singleton<TypeFilter>{
public:
	void Init(){
		int a[] = {1,5,6,7,20};
		btypes_.insert(a,a+sizeof(a)/sizeof(a[0]));
		MCE_INFO("TypeFilter::Init. btypes_:" << PrintHelper::Print(btypes_));
	}
	bool Check(int uid,int stype){
		if(uid % 100 == 27 || uid == 220678672){
			return btypes_.count(stype / 100);
		}
		return true;
	}
private:
	set<int> btypes_;
};
}
}

#endif /* USERINFO_H_ */
