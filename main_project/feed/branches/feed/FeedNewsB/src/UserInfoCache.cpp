/*
 * UserInfo.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: yejingwei
 */

#include "UserInfoCache.h"
#include "GetRequest.h"
#include <boost/foreach.hpp>
//#include "FeedCacheAdapter.h"
#include "cmath"
#include <algorithm>
#include "FeedMemcProxy/client/feed_user_property.h"
#include "FeedCacheNAdapter.h"
//#include "FeedCacheAdapter.h"
#include "FeedCacheLogicAdapter.h"
#include "FeedMarkReplicaAdapter.h"
#include "Date.h"
#include "EDMFeed.h"
#include <climits>
#include <map>
using namespace boost;
using namespace xce::feed;
using namespace std;

bool UserInfo::Init(){
	if(inited_){
		MCE_WARN("UserInfo::Init. should be init once. uid:" << uid_);
		return true;
	}
	DownloadMaxRead();
	LoadEdms();
	LoadMark();

	//读取FeedCache失败，就废弃这个UserInfo
	if(!LoadFeedCache()) return false;
	inited_ = true;
	return true;
}
void UserInfo::LoadMark(){
	MarkInfoPtr minfo;
	try{
		minfo = FeedMarkReplicaAdapter::instance().GetMarkInfo(uid_);
	}catch(Ice::Exception& e){
		MCE_WARN("UserInfo::LoadMark. uid:" << uid_ << " error:" << e );
		return;
	}
	if(!minfo) {
		MCE_WARN("UserInfo::LoadMark. uid:" << minfo << " return empty minfo");
		return;
	}

	BOOST_FOREACH(int id, minfo->marks){
		marked_friends_[id] = true;
	}
	BOOST_FOREACH(int id, minfo->unmarks){
		marked_friends_[id] = false;
	}
	MCE_INFO("UserINfo::LoadMark. uid:" << uid_ << " marks:" << PrintHelper::Print(minfo->marks)
		<< " unmarks:" << PrintHelper::Print(minfo->unmarks));
}
long UserInfo::DownloadMaxRead(){
	FeedUserPropertyData data = FeedUserPropertyClient::instance().Get(uid_);
	pre_max_read_fid_ = data.GetMaxFeedId();
	MCE_INFO("UserInfo::DownloadMaxRead. && uid:" << uid_
			<< " downloaded pre_max_read_fid:"	<< pre_max_read_fid_);
	return pre_max_read_fid_;
}
void UserInfo::UploadMaxRead()const{
	FeedUserPropertyData data = FeedUserPropertyClient::instance().Get(uid_);
	long maxfix_memc = data.GetMaxFeedId();
	if(max_fid_ > data.GetMaxFeedId()){
		data.SetMaxFeedId(max_fid_);
		FeedUserPropertyClient::instance().Set(uid_,data);
	}
	MCE_INFO("UserInfo::UploadMaxRead. uid:" << uid_
			<< " pre_max_read_fid_:" << pre_max_read_fid_
			<< " max_fid_:" << max_fid_
			<< " maxfix_memc:" << maxfix_memc
	);
//	MaxFidUploader::instance().Add(uid_,max_fid_);
}

long UserInfo::GetMaxRead( )const { return max_fid_; }

long UserInfo::GetPreMaxRead( )const{ return pre_max_read_fid_; }

void UserInfo::SetMaxRead(long fid){
	if(pre_max_read_fid_ >= fid) return;
	pre_max_read_fid_ = fid;
}

void UserInfo::RefreshPreMaxRead(){
	MCE_INFO("UserInfo::RefreshPreMaxRead. uid:" << uid_ << " pre_max_read_fid_:" << pre_max_read_fid_ << " max_fid_:" << max_fid_);
	pre_max_read_fid_ = max_fid_;
}

void AddSFeed1D(SFeedPtr2D & sfeed2d,HFeedPtr1D & hfeed1d){
//	SFeedPtr1D sfeed1d;
//	BOOST_FOREACH(HeavyFeed & hfeed, hfeed1d){
//		sfeed1d.push_back(hfeed.GetSharedFeedPtr());
//	}
//	sfeed2d.push_back(sfeed1d);
}
void UserInfo::AddRefresh(time_t time,long fid){
	IceUtil::Mutex::Lock lock(mutex_);
	refreshes_[time] = fid;
}
//map<long,long> UserInfo::GetRefresh() const {
//	IceUtil::Mutex::Lock lock(mutex_);
//	return refreshes_;
//}

bool UserInfo::HandleEdm(GetRequest & req){
	if(edms_.empty())
		return false;

	bool has_new_edm = false;
	HeavyFeed & hfeed = edms_.at(0);

	if(hfeed.GetTime() > 0 && hfeed.GetTime() < time(NULL) ){
		MCE_INFO("UserInfo::HandleEdm. clear edm fid:" << hfeed.GetFid() << " stype:" << hfeed.GetStype() );
		edms_.clear();//过了指定时间
		return has_new_edm;
	}

	if(hfeed.GetTime() < 0){
		hfeed.ResetEdmTime();
		has_new_edm = true;
	}

	if(req.CheckType(hfeed.GetStype())){
		SFeedPtr1D sfeed1d;
		sfeed1d.push_back(hfeed.GetSharedFeedPtr());
		req.sfeed2d_.push_back(sfeed1d);
	}
	return has_new_edm;
}
void UserInfo::ClearOldComments(){
//	long fid = GetOldestFid();
//	vector<long> removed;
//	for(HFeedPtr1D::iterator it = comments_.begin(); it != comments_.end();/**/){
//		if(it->GetFid() < fid){
//			removed.push_back(it->GetFid());
//			it = comments_.erase(it);
//		}else{
//			++it;
//		}
//	}
//	MCE_INFO("UserInfo::ClearOldComments. uid:" << uid_
//			<< " oldest fid:" << fid
//			<< " removed:" << PrintHelper::Print(removed));
}

void UserInfo::GetShowedComments(StypemergeSFeed1DMap& commentmap){

//	BOOST_FOREACH(HFeedPtr1D::value_type & cmt,comments_){
//		pair<int,long> key = cmt.GetStypeMerge();
//		key.first = MergeConfig::instance().Comment2Main(key.first);
//		StypemergeSFeed1DMap::iterator it = commentmap.find(key);
//		if(it != commentmap.end()){
//			it->second.push_back(cmt.GetSharedFeedPtr());
//		}
//	}
//	for(StypemergeSFeed1DMap::iterator it = commentmap.begin(); it != commentmap.end();){
//		if(it->second.empty()){
//			commentmap.erase(it++);
//		}else{
//			++it;
//		}
//	}
//	MCE_INFO("@@ UserInfo::GetShowedComments. showed commentmap size:" << commentmap.size() << " comments:" << comments_.size());
}

set<long> UserInfo::GetMarkedFids(const set<long> & fids){
	set<long> res;
//	IceUtil::Mutex::Lock lock(mutex_);
//	MoveRecentToSorted(time(NULL));
//	BOOST_FOREACH(HFeedPtr1D& hfeed1d,sorted_){
//		BOOST_FOREACH(HeavyFeed & hfeed,hfeed1d){
//			if(!fids.count(hfeed.GetFid()))
//				continue;
//			if(hfeed.IsMarked());
//				res.insert(hfeed.GetFid());
//		}
//	}
	return res;
}

void UserInfo::GetFeeds(GetRequest & req){
	bool has_new_edm = false;
	{
		IceUtil::Mutex::Lock lock(mutex_);
		//把最近收到新鲜事，作成一个块，然后放入sorted中
		MoveRecentToSorted(req.GetNowTime());

//		if(req.ShowEdm())
//			has_new_edm = HandleEdm(req);

		MergeFeed(req);
		if(req.IsSort()){
			GatherFeed(req);
		} else {
			GatherFeedSimple(req);
		}

		req.GetShowedFids();
//		if(req.ByBlock())
//			GetFeedsByBlock(req);
//		else
//			GetFeedsByTime(req);

//		ClearOldComments();
//
//		if(req.ShowComment())
//			GetShowedComments(req.showed_comment_map_);
	}
	if(has_new_edm){
		MCE_INFO("UserInfo::GetFeeds. uid:" << uid_ << " call removeUser");
		EDMFeedManager::instance().removeUser(uid_);
	}else{
		MCE_INFO("UserInfo::GetFeeds. uid:" << uid_ << " not call removeUser");
	}
}

//三个  StypemergeScore(函数内用)  StypemergeHFeed1DMap ScoreIteratorMultimap
void UserInfo::MergeFeed(GetRequest & req){
	BOOST_REVERSE_FOREACH(HeavyFeed & hfeed, sorted_){
		pair<int,long> key = hfeed.GetVersionStypeMerge();
		StypemergeHFeed1DMap::iterator mit = req.stypemerge_item1d_map_.find(key);
		if (mit != req.stypemerge_item1d_map_.end()) {
			mit->second.push_back(&hfeed);
		} else {
			req.stypemerge_item1d_map_[key].push_back(&hfeed);
			double score;
			if(req.ByBlock()){
				score = - req.stypemerge_item1d_map_.size();
			} else {
				score = hfeed.GetTime();
			}
			StypemergeHFeed1DMap::iterator it = req.stypemerge_item1d_map_.find(key);
			req.score_iterator_multimap_.insert(make_pair(score,it));//TODO
		}
	}
	MCE_INFO("UserInfo::MergeFeed. uid:" << req.uid_ );
}

void UserInfo::GatherFeed(GetRequest & req){
	StypeactorTimeindexMap stypeactor_timeindex;

	BOOST_REVERSE_FOREACH(ScoreIteratorMultiMap::value_type & p,req.score_iterator_multimap_){
		StypemergeHFeed1DMap::iterator & tmpitr = p.second;
		HFeedPtr1D & item1d = tmpitr->second;
		HeavyFeedPtr hfeed = item1d.front();

		StypeactorTimeindexMap::iterator ti;
		if(FindSameStypeActor(hfeed,stypeactor_timeindex,ti)
				&& (TimeDiffLessThanLimit(hfeed->GetTime(),ti->second.first))){
			req.hfeed3d_[ti->second.second].push_back(item1d);
		}else{
			HFeedPtr2D item2d;
			item2d.push_back(item1d);
			req.hfeed3d_.push_back(item2d);
			stypeactor_timeindex[hfeed->GetGatherKey()]
			                     = make_pair(hfeed->GetTime(), req.hfeed3d_.size()-1);
		}
	}
	MCE_INFO("UserInfo::GatherFeed. uid:" << req.uid_ << " item3d:" << req.hfeed3d_.size());
}


void UserInfo::GatherFeedSimple(GetRequest & req){
	MCE_INFO("UserInfo::GatherFeedSimple.begin. scoremap size:" << req.score_iterator_multimap_.size());

	BOOST_REVERSE_FOREACH(ScoreIteratorMultiMap::value_type & p,req.score_iterator_multimap_){
		StypemergeHFeed1DMap::iterator & tmpitr = p.second;
		HFeedPtr1D & item1d = tmpitr->second;

		HFeedPtr2D item2d;
		item2d.push_back(item1d);

		req.hfeed3d_.push_back(item2d);
	}
}

int GetFriendPrefer(int frank){
    return (frank >> 4) & 0x7;
}

void UserInfo::MakeScoreHFeedMap(multimap<double,HeavyFeed * > & score_map, time_t nowtime){
	BOOST_FOREACH(HeavyFeed & hfeed ,recent_){
//		double score = hfeed.GetScore(0,true);//TODO
		int friendprefer = hfeed.GetFriendPrefer();
//		double orig_score = score;
		double score;
		bool top = false;
		map<int,bool>::iterator it = marked_friends_.find(hfeed.GetActor());
		int mark_state = 0;
		if (it != marked_friends_.end()) {
			if (it->second ) {//将mark
				mark_state = 1;
				score = 2 * nowtime + hfeed.GetTime();
				top = true;
			} else {
				mark_state = -1;
				score = hfeed.GetTime();
			}
		} else {
			if (hfeed.IsFocusSource() or friendprefer >= 7 ) {
				score = 2 * nowtime + hfeed.GetTime();
				top = true;
			} else if (hfeed.IsUgc()) {
				score = nowtime + hfeed.GetTime();
				top = true;
			} else {
				score = hfeed.GetTime();
			}
		}
		MCE_INFO("MM.uid:" << GetUid() << " fid:" << hfeed.GetFid() << " st:" << hfeed.GetStype() << " a:" << hfeed.GetActor()
				<< " score:" << score << " IsUgc:" << hfeed.IsUgc()
				<< " fr:" << hfeed.GetFriendRank()
				<< " fprefer:" << friendprefer
				<< " mark_state:" << mark_state << " fc:" << hfeed.IsFocusSource() << " top:" << top );

		score_map.insert(make_pair(score,&hfeed));
	}
}

void UserInfo::MoveRecentToSorted(time_t nowtime){
	multimap<double,HeavyFeed * > score_map;
	MakeScoreHFeedMap(score_map,nowtime);
	int marked = 0, not_marked = 0;
	typedef map<double,HeavyFeed *>::value_type vt;
	BOOST_FOREACH(vt & p,score_map){
		HeavyFeed & hf = *(p.second);
		if(p.first >= 2 * nowtime){
			hf.Mark(true);
			++marked;
		} else {
			hf.Mark(false);
			++not_marked;
		}
		sorted_.push_front(*(p.second));
	}
	recent_.clear();
	MCE_INFO("UserInfo::MoveRecentToSorted. after get top. sorted:" << sorted_.size() << " marked:" << marked
			<< " not_marked:" << not_marked
			<< " now:" << nowtime);
}

//void UserInfo::MoveRecentToSorted(time_t nowtime){
//
//	StypeMergeMap stypemerge_map;
//	ScoreMap score_map;
//
//	MakeStypeMergeMap(stypemerge_map, score_map);
//
//	int marked = 0, not_marked = 0;
//	vector<time_t> scores;
//	BOOST_FOREACH(ScoreMap::value_type & vt,score_map){
//		time_t score = vt.first;
//		if(score >= nowtime){
//			PushHFeedPtr1DToSorted(*(vt.second.second),true);
//			++marked;
//		}else{
//			PushHFeedPtr1DToSorted(*(vt.second.second),false);
//			++not_marked;
//		}
//		scores.push_back(score);
//	}
//
//	recent_.clear();
//	MCE_INFO("UserInfo::MoveRecentToSorted. after get top. sorted:" << sorted_.size() << " marked:" << marked
//			<< " not_marked:" << not_marked
//			<< " line:" << __LINE__
//			<< " now:" << nowtime
////			<< " scores:" << PrintHelper::Print(scores)
//	);
//}

//只是针对recent_
void UserInfo::MakeStypeMergeMap(StypeMergeMap & stypemerge_map, ScoreMap & score_map){
	long nowtime = time(NULL);
	BOOST_FOREACH(HeavyFeed & h,recent_){
		HeavyFeedPtr hfeed = &h;

		pair<int,long> key = hfeed->GetStypeMerge();
		StypeMergeMap::iterator mit =
				stypemerge_map.find(key);
		if(mit != stypemerge_map.end()){
			mit->second.push_back(hfeed);
			stypemerge_map[key].push_back(hfeed);
		}else{
			HFeedPtr1D hfeed1d;
			hfeed1d.push_back(hfeed);
			stypemerge_map[key] = hfeed1d;
			double score = hfeed->GetScore(uid_,true);

			map<int,bool>::iterator it = marked_friends_.find(hfeed->GetActor());
			if(it != marked_friends_.end()){
				if(it->second ) //将mark
					score += nowtime;
				else
					score = hfeed->GetTime();
			} else{
				if(hfeed->IsFocusSource() or score >= 90 )
					score += nowtime;
				else
					score = hfeed->GetTime();
			}

			score_map.insert(make_pair(score,make_pair(hfeed->GetTime(),&(stypemerge_map[key]))));
		}
	}
}

void UserInfo::PushHFeedPtr1DToSorted(HFeedPtr1D & hfeedptr1d,bool mark){
//	HFeedPtr1D hfeed1d;
//	BOOST_FOREACH(HeavyFeedPtr hp, hfeedptr1d){
//		hp->Mark(mark);
//		hfeed1d.push_back(*hp);
//	}
//	sorted_.push_front(hfeed1d);
}


bool UserInfo::LoadFeedCache(){
	FeedItemSeq fitems;
	TimeStat ts;
	try{
//		fitems = FeedCacheNF55Adapter::instance().get(uid_);
		fitems = FeedCacheNAdapter::instance().get(uid_);
	}catch(Ice::Exception& e){
		MCE_WARN("UserInfo::LoadFeedCache. uid:" << uid_ << " time:" << ts.Get()<< " error:" << e );
		return false;
	}

	AddFeedItemsFromFeedCache(fitems);
//	DumpSorted();
	MCE_INFO("UserInfo::LoadFeedCache. after get." << ToString()
				<<  " fitems from FeedCache:" << fitems.size() << " cost:" << ts.Get() << " line:" << __LINE__);
	return true;
}

void UserInfo::SeperateByPreMaxFid(FeedItemSeq & fitems,HDeq & next){
	BOOST_REVERSE_FOREACH(FeedItem & fitem, fitems){
		SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);
		HeavyFeed hfeed(fitem,hdl);
		if(MergeConfig::instance().IsComment(hfeed.GetStype())){
//			comments_.push_back(hfeed);
			continue;
		}

		if(hfeed.GetFid() <= pre_max_read_fid_)
			recent_.push_back(hfeed);
		else
			next.push_back(hfeed);//最最新的

		max_fid_ = max(max_fid_,hfeed.GetFid());
	}
}
//将原来看过的作成一个块，放入sorted，
//之后将没看过的放入recent
void UserInfo::AddFeedItemsFromFeedCache(FeedItemSeq & fitems){
	HDeq next;

	SeperateByPreMaxFid(fitems,next);
	int below_pre_max = recent_.size();
	int above_pre_max = next.size();

	time_t nowtime = time(NULL);
	if(!recent_.empty())
		MoveRecentToSorted(nowtime);

	if(!next.empty())
		recent_.swap(next);

	MCE_INFO("UserInfo::AddFeedItemsFromFeedCache. uid:" << uid_ << " done. below_pre_max:" << below_pre_max
			<< " above_pre_max:" << above_pre_max
			<< sorted_.size());
}

bool UserInfo::LoadEdms(){
	//TODO只加载一条
	FeedItemSeq fitems = EDMFeedManager::instance().getEDMFeed(uid_);

	if(!fitems.empty()){
		FeedItem fitem = *(fitems.rbegin());

//		onefitem.push_back(*fitems.rbegin());//最新的一个edm在最后
//		GetSharedFeedsAndAddBuffer(onefitem,false);

		SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);
		HeavyFeed hfeed(fitem,hdl);
		edms_.push_back(hfeed);
	}
	MCE_INFO("UserInfo::LoadEdms. uid:" << uid_ << " edms:" << edms_.size() );
	return true;
}

void UserInfo::AddHeavyItem(int uid,HeavyFeed & hfeed/*,bool updatemaxfid*/){

	IceUtil::Mutex::Lock lock(mutex_);
//	Trace trace("UserInfo::AddHeavyItem");
	if(hfeed.GetTime() < 0){
		edms_.clear();
		edms_.push_back(hfeed);
		MCE_INFO("UserInfo::AddHeavyItem.is edm uid:" << uid << " fid:" << hfeed.GetFid() << " new:" << recent_.size() << " sorted:" << sorted_.size());
		return;
	}
	recent_.push_front(hfeed);
	while(Size() > kHbufSize){
		if(!sorted_.empty()){
			sorted_.pop_back();
		}else{
			recent_.pop_back();
		}
	}
	max_fid_ = max(max_fid_,hfeed.GetFid());
	MCE_INFO("UserInfo::AddHeavyItem. uid:" << uid << " fid:" << hfeed.GetFid() << " new:" << recent_.size() << " sorted:" << sorted_.size());
}
void UserInfo::SetReadAll(){
	IceUtil::Mutex::Lock lock(mutex_);
	sorted_.clear();
	edms_.clear();
//	comments_.clear();
	recent_.clear();
}
vector<long> UserInfo::SetRead(int stype,long merge ){
	IceUtil::Mutex::Lock lock(mutex_);
	bool del = false;
	vector<long> fids;
	for(HFeedBuf::iterator it = sorted_.begin();
			it != sorted_.end();){

		if(it->GetStype() == stype && it->GetMerge() == merge){
			it = sorted_.erase(it);
			fids.push_back(it->GetFid());
			//TODO 确认迭代器行为
		}else{
			++it;
		}
	}
	MCE_INFO("UserInfo::SetRead. uid:" << uid_ << " stype:" << stype << " merge:" << merge << " deleted:" << del);
	return fids;
}
//
vector<long> UserInfo::SetReadById(long fid){
	IceUtil::Mutex::Lock lock(mutex_);
	bool del = false;
	vector<long> fids;

	for(HFeedBuf::iterator it = sorted_.begin();
			it != sorted_.end();){
		if(it->GetFid() == fid){
			it = sorted_.erase(it);
			fids.push_back(it->GetFid());
			//TODO
			break;
		} else {
			++it;
		}
	}

//	for(HFeedBuf::iterator it = sorted_.begin();
//			it != sorted_.end();++it){
//		BOOST_FOREACH(HeavyFeed &hf, *it){
//			if(hf.GetFid() == fid){
//				del = true;
//				break;
//			}
//		}
//		if(del){
//			BOOST_FOREACH(HeavyFeed &hf, *it){
//				fids.push_back(hf.GetFid());
//			}
//			sorted_.erase(it);
//			break;
//		}
//	}
//	MCE_INFO("UserInfo::SetReadById. uid:" << uid_ << " fid:" << fid << " deleted:" << del);
	return fids;
}

void UserInfo::MarkFeed(long fid,long source,int stype,int actor,bool marked){
	IceUtil::Mutex::Lock lock(mutex_);

	bool set_already, old_value;
	map<int,bool>::iterator it = marked_friends_.find(actor);
	if(marked_friends_.end() != it){
		set_already = true;
		old_value = it->second;
		it->second = marked;
	}else {
		set_already = false;
		marked_friends_[actor] = marked;
	}

	bool found = false;
	vector<long> fids;
	BOOST_FOREACH(HeavyFeed & hf, sorted_){
		if(hf.GetActor() == actor){
			found = true;
			hf.Mark(marked);
			fids.push_back(fid);
		}
//		if(hf.GetFid() == fid){
//			found = true;
//			hf.Mark(marked);
//			fids.push_back(fid);
//			break;
//		}
	}

	MCE_INFO("UserInfo::MarkFeed. uid:" << uid_ << " fid:" << fid << " stype:" << stype
			<< " actor:" << actor
			<< " set_already:" << set_already
			<< " old_value:" << old_value
			<< " changed fids:" << PrintHelper::Print(fids)
			<< " found:" << found);
}

void UserInfo::SetReadByIdsExactly(const set<long> & fids,bool remove_feedcache){
	set<long> in_fids = fids;
	set<long> deleted_fids;
	{
		IceUtil::Mutex::Lock lock(mutex_);
		for(HFeedBuf::iterator it = sorted_.begin(); it != sorted_.end();/**/){
			long fid = it->GetFid();
			if(in_fids.count(it->GetFid())){
				it = sorted_.erase(it);
				in_fids.erase(fid);
				deleted_fids.insert(fid);
			} else {
				++it;
			}
			if(in_fids.empty()){
				break;
			}
		}
	}
	MCE_INFO("UserInfo::SetReadByIdsExactly. uid:" << uid_ << " fids:" << PrintHelper::Print(fids)
	<< " deleted fids:" << PrintHelper::Print(deleted_fids));

}

FeedItemSeq UserInfo::GetOrigFeedItems(){
	IceUtil::Mutex::Lock lock(mutex_);
	FeedItemSeq res;
//	BOOST_FOREACH(HFeedPtr1D& hfeed1d,sorted_){
//		BOOST_FOREACH(HeavyFeed & hfeed,hfeed1d){
//			res.push_back(hfeed.ToFeedItem());
//		}
//	}
	return res;
}

long UserInfo::GetOldestFid(){
	long fid = LONG_MAX;
//	FeedItemSeq res;
//	BOOST_FOREACH(HFeedPtr1D& hfeed1d,sorted_){
//		BOOST_FOREACH(HeavyFeed & hfeed,hfeed1d){
//			fid = min(fid,hfeed.GetFid());
//		}
//	}
	return fid;
}


int UserInfo::Size() const {
//	IceUtil::Mutex::Lock lock(mutex_);
	return sorted_.size() + recent_.size();
}

string UserInfo::ToString() const {
	ostringstream os;
//	os << " uid:" << uid_ << " bufsize:" << hbuf_.size()<<" max_fid:" << max_fid_ << " pre_max_read_fid:" << pre_max_read_fid_;
	return os.str();
}


void MaxFidUploader::handle(){
  map<int,long> map_swap;
  {
  	IceUtil::Mutex::Lock lock(mutex_);
  	map_swap.swap(map_);
  }
  typedef map<int,long>::value_type vt;
  BOOST_FOREACH(vt & p, map_swap){
  	FeedUserPropertyData data = FeedUserPropertyClient::instance().Get(p.first);
  	if(p.second <= data.GetMaxFeedId()){
  		continue;
  	}
  	data.SetMaxFeedId(p.second);
  	FeedUserPropertyClient::instance().Set(p.first,data);
  }
  MCE_INFO("MaxFidUploader::handle. uids:" << map_swap.size());
  TaskManager::instance().schedule(this);
}
