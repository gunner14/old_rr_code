/*
 * UserInfo.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: yejingwei
 */

#include "UserInfo.h"
#include "Common.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
//#include "FeedCacheAdapter.h"
#include "cmath"
#include <algorithm>
#include "FeedMemcProxy/client/feed_user_property.h"
#include "FeedCacheNAdapter.h"
#include "FeedCacheNReplicaAdapter.h"
#include "FeedCacheLogicAdapter.h"
#include "Date.h"
#include "EDMFeed.h"
#include <boost/tuple/tuple.hpp>


using namespace boost;
using namespace xce::feed;
using namespace std;

//UserInfo::~UserInfo(){
//	MyUtil::Date date(stamp_);
//	MCE_INFO("UserInfo::~UserInfo. uid:" << uid_ << " live time:" << time(0)-stamp_ << " create at " << date.toDateTime());
//}
long UserInfo::DownloadMaxRead(){
	//zj//long old = pre_max_read_fid_;
	FeedUserPropertyData data = FeedUserPropertyClient::instance().Get(uid_);
	pre_max_read_fid_ = data.GetMaxFeedId();
	refresh_time_ = data.GetRefreshTime();

	MCE_INFO("UserInfo::DownloadMaxRead. uid:" << uid_ << " pre_max_read_fid:" << pre_max_read_fid_
			<< " refresh_time:" << refresh_time_ << " t:" << MyUtil::Date(refresh_time_).toDateTime());
	return pre_max_read_fid_;
}

bool UserInfo::SetMaxRead(long fid){
	if(pre_max_read_fid_ >= fid)
		return false;

	pre_max_read_fid_ = fid;
	return true;
}

bool UserInfo::SetRefreshTime(time_t t){
	refresh_time_ = t;
	return true;
}

//从memcahce中加载MinFeedid, 设置MinFeedid
//by jin.zhang(张劲), 2012.02.24
long UserInfo::DownloadMinRead(){
    FeedUserPropertyData data = FeedUserPropertyClient::instance().Get(uid_);
    long minfid = data.GetMinFeedId();
    if (minfid > 0)
        pre_min_read_fid_ = minfid;

    MCE_DEBUG("UserInfo::DownloadMinRead. uid: " << uid_ << "pre_min_read_fid: "
            << pre_min_read_fid_);
    return pre_min_read_fid_;
}
bool UserInfo::SetMinRead(long fid, bool update){
    if (fid <= 0 || pre_min_read_fid_ <= fid && update)
        return false;
    pre_min_read_fid_ = fid;
    return true;
}


//void UserInfo::RaiseUgc(GetRequest & req){
//}
void UserInfo::MergeAndSort(GetRequest & req){
	IceUtil::Mutex::Lock lock(mutex_);
	MergeFeed(req);

	if(req.IsSort())
		GatherFeed(req);
	else
		GatherFeedSimple(req);

	req.GetShowedFids();
}

static void PrintScore(StypemergeScoreMap & vstypemerge_score){//临时测试用
	BOOST_FOREACH(StypemergeScoreMap::value_type & p, vstypemerge_score){
		ostringstream oss;
		oss.setf(ios::fixed);
		oss << p.second;
		MCE_INFO(p.first.first << " " << p.first.second << " " << oss.str());
	}
}
void UserInfo::MergeFeed(GetRequest & req){
	StypemergeScoreMap vstypemerge_score;

	MergeByStypeMergeAndGetScore(req,vstypemerge_score);
	if(req.ShowComment()){
		ReplaceScoreByCommentScore(req,req.comment_map_,vstypemerge_score);
	}
	MakeScoreIteratorMultiMap(req,vstypemerge_score);

	MCE_INFO("UserInfo::MergeFeed. merge done." << req.ToString());
}

void UserInfo::ReplaceScoreByCommentScore(GetRequest & req,StypemergeHFeed1DMap & comment_map_,
		StypemergeScoreMap & vstypemerge_score ){
	BOOST_FOREACH(StypemergeHFeed1DMap::value_type & p, comment_map_){
		StypemergeScoreMap::iterator mit = vstypemerge_score.find(p.first);
		if(mit != vstypemerge_score.end()){
			mit->second =(p.second.front())->GetScore(req.uid_,req.IsSort(),req.GetNowTime(),GetMaxRead(),recent_cnt_,refresh_time_);//TODO 判断评论是否updatetime
		}
	}
}
void UserInfo::MergeByStypeMergeAndGetScore(GetRequest & req,StypemergeScoreMap & vstypemerge_score){

//	BOOST_FOREACH(HeavyFeed & h,hbuf_){
	for(HBuf::iterator it = hbuf_.begin(); it != hbuf_.end();){
		HeavyFeedPtr hfeed = &(*it);
		if(!req.CheckType(hfeed->GetStype()) || req.SeperateComment(hfeed)) {
			++it;
			continue;
		}

		bool is_edm,must_erase;
		boost::tie(is_edm,must_erase) = req.SeperateEdm(hfeed);
		if(is_edm){
			if(must_erase) it = hbuf_.erase(it);
			else ++it;
			continue;
		}

		pair<int,long> key = hfeed->GetVersionStypeMerge();
		StypemergeHFeed1DMap::iterator mit =
				req.stypemerge_item1d_map_.find(key);
		if (mit != req.stypemerge_item1d_map_.end()) {
			mit->second.push_back(hfeed);
			if(hfeed->IsUpdateTime()){
				vstypemerge_score[key] = hfeed->GetScore(req.uid_,req.IsSort(),req.GetNowTime(),GetMaxRead(),recent_cnt_,refresh_time_);
			}
		} else {
			req.stypemerge_item1d_map_[key].push_back(hfeed);
			vstypemerge_score[key] = hfeed->GetScore(req.uid_,req.IsSort(),req.GetNowTime(),GetMaxRead(),recent_cnt_,refresh_time_);
		}
		++it;
	}
}
void UserInfo::MakeScoreIteratorMultiMap(
		GetRequest & req,StypemergeScoreMap & vstypemerge_score){
	for(StypemergeScoreMap::iterator mit = vstypemerge_score.begin();
			mit != vstypemerge_score.end();++mit){
		req.score_iterator_multimap_.insert(make_pair(mit->second,req.stypemerge_item1d_map_.find(mit->first)));
	}
}


void UserInfo::GatherFeed(GetRequest & req){
	StypeactorTimeindexMap stypeactor_timeindex;

	BOOST_REVERSE_FOREACH(ScoreIteratorMultiMap::value_type & p,req.score_iterator_multimap_){
		StypemergeHFeed1DMap::iterator & tmpitr = p.second;
		HFeed1D & item1d = tmpitr->second;
		HeavyFeedPtr hfeed = item1d.front();

		StypeactorTimeindexMap::iterator ti;
		if(FindSameStypeActor(hfeed,stypeactor_timeindex,ti)
				&& (TimeDiffLessThanLimit(hfeed->GetTime(),ti->second.first))){
			req.hfeed3d_[ti->second.second].push_back(item1d);
		}else{
			HFeed2D item2d;
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
		HFeed1D & item1d = tmpitr->second;

		HFeed2D item2d;
		item2d.push_back(item1d);

		req.hfeed3d_.push_back(item2d);
	}
//	PrintHelper::Print(req.item3d_);
}

void UserInfo::GetSharedFeedsAndAddBuffer(FeedItemSeq & fitems,bool normalfeed){
	//默认fitems按时间从大到小排序，需要从小到大放到hbuf前端
	BOOST_REVERSE_FOREACH(FeedItem & fitem, fitems){
		if(normalfeed && fitem.time < 0){//防止FeedCache中的脏数据
			MCE_WARN("UserInfo::GetSharedFeedsAndAddBuffer. uid:" << uid_
					<< " fid:" << fitem.feed
					<< " stype:" << (fitem.type & 0xFFFF)
					<< " actor:" << fitem.actor
					<< " time:" << fitem.time
					);
			continue;
		}

		SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);
		HeavyFeed hfeed(fitem,hdl);
		if(fitem.feed <= pre_max_read_fid_){
			hfeed.IncView();
		}else{
			++recent_cnt_;
			//MCE_INFO("@@@@ UserInfo::GetSharedFeedsAndAddBuffer. recent feed. uid:" << uid_
			//		<< " fid:" << fitem.feed
			//		<< " stype:" << (fitem.type & 0xFFFF)
			//		<< " actor:" << fitem.actor
			//		<< " time:" << fitem.time
			//		);
		}

		hbuf_.push_front(hfeed);
//		max_fid_ = max(max_fid_,hfeed.GetFid());
	}
}


bool UserInfo::LoadFeedCache(){
	FeedItemSeq fitems;
	TimeStat ts;
	try{
		//zj//int idx = uid_ % 10;
//		if(idx == 3){
			fitems = FeedCacheNReplicaAdapter::instance().get(uid_);
//		}else{
//			fitems = FeedCacheNAdapter::instance().get(uid_);
//		}

//		fitems = FeedCacheAdapter::instance().get(uid_);
//		fitems = FeedCacheNF55Adapter::instance().get(uid_);

	}catch(Ice::Exception& e){
		MCE_WARN("UserInfo::LoadFeedCache. uid:" << uid_ << " time:" << ts.Get()<< " error:" << e );
		return false;
	}

	GetSharedFeedsAndAddBuffer(fitems,true);

	MCE_INFO("UserInfo::LoadFeedCache. after get." << ToString()
				<< " fitems from FeedCache:" << fitems.size()
				<< " recent_cnt:" << recent_cnt_	 
				<< " cost:" << ts.Get() << " line:" << __LINE__);
	return true;
}

bool UserInfo::LoadEdms(){
	FeedItemSeq fitems = EDMFeedManager::instance().getEDMFeed(uid_);
	ostringstream os;
	MCE_INFO("UserInfo::LoadEdms. uid:" << uid_ << " edms:" << PrintHelper::Print(fitems));

	if(!fitems.empty()){
		FeedItemSeq onefitem;
		onefitem.push_back(*fitems.rbegin());//最新的一个edm在最后
		GetSharedFeedsAndAddBuffer(onefitem,false);
	}
	return true;
}

void UserInfo::AddHeavyItem(int uid,HeavyFeed & hfeed/*,bool updatemaxfid*/){
	IceUtil::Mutex::Lock lock(mutex_);
	++recent_cnt_;
	hbuf_.push_front(hfeed);
}

void UserInfo::SetReadAll(){
	IceUtil::Mutex::Lock lock(mutex_);
	hbuf_.clear();
}

vector<long> UserInfo::SetRead(int stype,long merge ){
	vector<long> res;
	{
		IceUtil::Mutex::Lock lock(mutex_);
		for(HBuf::iterator it = hbuf_.begin(); it != hbuf_.end();){
			HeavyFeed & hfeed = *it;
			if(hfeed.SameStypeMerge(stype,merge)){
				res.push_back(hfeed.GetFid());
				it = hbuf_.erase(it);
			}else{
				++it;
			}

//			HeavyFeed & hfeed = *it;
//			if(hfeed->SameStypeMerge(stype,merge)){
//				res.push_back((*it).GetFid());
//				it = hbuf_.erase(it);
//			}else{
//				++it;
//			}
		}
	}
	return res;
}

vector<long> UserInfo::SetReadById(long fid){
	pair<int,long> stypemerge = make_pair<int,long>(0,0);
	{
		IceUtil::Mutex::Lock lock(mutex_);
		BOOST_FOREACH(HBuf::value_type & hfeed , hbuf_){
			if(hfeed.GetFid() == fid)
				stypemerge = hfeed.GetStypeMerge() ;
		}
	}
	if(stypemerge.first) return SetRead(stypemerge.first,stypemerge.second);
	else return vector<long>();
}

void UserInfo::SetReadByIdsExactly(const set<long> & fids,bool remove_feedcache){
	if(fids.empty())
		return;
	vector<HeavyFeed> hfeeds;
	int removed = 0;
	{
		IceUtil::Mutex::Lock lock(mutex_);
		for(HBuf::iterator it = hbuf_.begin(); it != hbuf_.end();){
			HeavyFeed & hfeed = *it;
			if(fids.count(hfeed.GetFid())){
				hfeeds.push_back(hfeed);
				it = hbuf_.erase(it);

				if(++removed == (int)fids.size()) break;
			}else{
				++it;
			}
		}
	}
	if(remove_feedcache){
		try{
			BOOST_FOREACH(long fid,fids){
				FeedCacheLogicAdapter::instance().remove(uid_,fid,true);//现在暂时不调用
			}
		}catch (Ice::Exception& e) {
			MCE_WARN("UserInfo::SetReadByIdsExactly. call cache logic fail. uid:" << uid_ << " fids:" << PrintHelper::Print(fids)<<" error:"<< e);
		}
	}

	MCE_INFO("UserInfo::SetReadByIdsExactly . uid:" << uid_ << " fids size:"
			<< fids.size() << " fids:"<< PrintHelper::Print(fids) << " remove hfeeds:" << PrintHelper::Print(hfeeds));
}

FeedItemSeq UserInfo::GetOrigFeedItems(){
	IceUtil::Mutex::Lock lock(mutex_);
	FeedItemSeq res;
	BOOST_FOREACH(const HBuf::value_type & hfeed,hbuf_){
		res.push_back(hfeed.ToFeedItem());
	}
	return res;
}

int UserInfo::GetUnviewCount(const set<int> & types){
	IceUtil::Mutex::Lock lock(mutex_);
	int res = 0;
	BOOST_FOREACH(HBuf::value_type & hfeed,hbuf_){
		if(hfeed.GetView() == 0) ++res;
	}
	return res;
}

map<int,int> UserInfo::GetCountByStype(const set<int> & stypes,bool unviewed){
	map<int,int> res;
	IceUtil::Mutex::Lock lock(mutex_);
	BOOST_FOREACH(HBuf::value_type & hfeed,hbuf_){
		if(!unviewed || (unviewed && hfeed.GetFid() > pre_max_read_fid_)){
			int stype = hfeed.GetStype();
			if(stypes.count(stype)){
				++res[stype];
			}
		}
	}
	return res;
}

map<int,int> UserInfo::GetCountByStypeMerged(const set<int> & stypes,bool unviewed){
	vector<int> stypes_v(stypes.begin(),stypes.end());
	GetRequest req(uid_,stypes_v,true,0,250,false);

	map<int,int> res;
	StypemergeScoreMap vstypemerge_score;

	{
		IceUtil::Mutex::Lock lock(mutex_);
		MergeByStypeMergeAndGetScore(req, vstypemerge_score );
		BOOST_FOREACH(StypemergeHFeed1DMap::value_type & vt,req.stypemerge_item1d_map_){
			int stype = vt.first.first & 0xFFFF;
			if(vt.second.empty())
				continue;
			long fid = vt.second.front()->GetFid();
			if(stypes.count(stype) && ((!unviewed)|| fid < pre_max_read_fid_) ){
				++res[stype];
			}
		}
	}
	MCE_INFO("UserInfo::GetCountByStypeMerged. @@@@ uid:" << uid_ << " stypemerge_item1d_map_ size:" << req.stypemerge_item1d_map_.size()
			<< " stypes:" << PrintHelper::Print(stypes_v));
	return res;
}


template<typename T>
void UserInfo::SetReadInFeedCache(const T & fids){
	BOOST_FOREACH(long fid,fids){
		FeedCacheLogicAdapter::instance().remove(uid_, fid,true);
	}
	MCE_INFO("UserInfo::SetReadInFeedCache. uid:" << uid_ << " fids:" << PrintHelper::Print(fids));
}

int UserInfo::Size() const {
	IceUtil::Mutex::Lock lock(mutex_);
	return hbuf_.size();
}

string UserInfo::ToString() const {
	ostringstream os;
	os << " uid:" << uid_ << " bufsize:" << hbuf_.size() << " pre_max_read_fid:" << pre_max_read_fid_;

    //log输出MinFeedid, by jin.zhang(张劲), 2012.02.24
    os << " pre_min_read_fid:" << pre_min_read_fid_;
	return os.str();
}

void UserInfo::IncViewBelowFid(long fid){
	IceUtil::Mutex::Lock lock(mutex_);
	BOOST_FOREACH(HBuf::value_type & hfeed, hbuf_){
		if(hfeed.GetFid() < fid and hfeed.GetView() == 0)
			hfeed.IncView();
	}
}

bool UserInfo::CheckUniq(){
	set<long> uniqids;
	set<long> dups;
	IceUtil::Mutex::Lock lock(mutex_);
	BOOST_FOREACH(HBuf::value_type & hfeed, hbuf_){
		pair<set<long>::iterator ,bool> p = uniqids.insert(hfeed.GetFid());
		if(!(p.second)){
			dups.insert(hfeed.GetFid());
		}
	}
	if(dups.empty()){
		MCE_INFO("UserInfo::CheckUniq. uid:" << uid_ << " success");
		return true;
	}else{
		MCE_WARN("UserInfo::CheckUniq. uid:" << uid_ << " fail. dup fids:" << PrintHelper::Print(dups));
		return false;
	}
}

//确认从大到小
bool UserInfo::CheckOrder(FeedItemSeq & fitems){
	long fid = 0;
	bool ordered = true;
	BOOST_REVERSE_FOREACH(FeedItem & fitem,fitems){
		if(fid > fitem.feed){
			MCE_WARN("UserInfo::CheckOrder. bad order.uid:" << uid_<<" last:" << fid << " this:" << fitem.feed);
			ordered = false;
		}
	}
	MCE_INFO("UserInfo::CheckOrder. uid:" << uid_ << " ordered:" << ordered);
	return ordered;
}



void MaxFidUploader::handle(){
	TimeStat ts;
	map<int,RefreshInfo> map_swap;
  {
  	IceUtil::Mutex::Lock lock(mutex_);
  	map_swap.swap(map_);
  }
  typedef map<int,RefreshInfo>::value_type vt;
  int cnt = 0;
  BOOST_FOREACH(vt & p, map_swap){
  	FeedUserPropertyData data = FeedUserPropertyClient::instance().Get(p.first);
  	RefreshInfo & r = p.second;
//  	MCE_INFO("MaxFidUploader::handle. uid:" << p.first
//  			<< " oldfid:" << data.GetMaxFeedId() << " oldrtime:" << data.GetRefreshTime()
//  			<< " fid:" << r.fid_ << " rtime:" << r.refresh_time_);
  	if(r.fid_ == data.GetMaxFeedId() and r.refresh_time_ == data.GetRefreshTime() )
  		continue;
  	data.SetMaxFeedId(r.fid_);
  	data.SetRefreshTime(r.refresh_time_);
  	FeedUserPropertyClient::instance().Set(p.first,data);
  	++cnt;
  }
  MCE_INFO("MaxFidUploader::handle. uids:" << map_swap.size() << " upload_cnt:" << cnt << " cost:" << ts.Get());
  TaskManager::instance().schedule(this);
}
