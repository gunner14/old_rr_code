
#include "UserInfo.h"
#include "cmath"
#include <algorithm>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "FeedMemcProxy/client/feed_user_property.h"
#include "FeedCacheNAdapter.h"
#include "FeedCacheNReplicaAdapter.h"
#include "FeedCacheLogicReplicaAdapter.h"
#include "Date.h"

#include "Common.h"
#include "FeedBuilder.h"


using namespace boost;
using namespace xce::feed;
using namespace std;
using namespace xce::offerfriends::adapter;


//----------  load  ---------
void UserInfo::AddHeavyItem(int uid, HeavyFeed & hfeed/*,bool updatemaxfid*/){
  IceUtil::Mutex::Lock lock(mutex_);
  hbuf_.push_front(hfeed);
}

void UserInfo::GetSharedFeedsAndAddBuffer(FeedItemSeq & fitems, bool normalfeed) {
	IceUtil::Mutex::Lock lock(mutex_);

  BOOST_REVERSE_FOREACH(FeedItem & fitem, fitems) {
    if(normalfeed && fitem.time < 0){//防止FeedCache中的脏数据
      MCE_WARN("UserInfo::GetSharedFeedsAndAddBuffer. --> uid:" << uid_
          << " fid:" << fitem.feed
          << " stype:" << (fitem.type & 0xFFFF)
          << " actor:" << fitem.actor
          << " time:" << fitem.time
          );
      continue;
    }

    SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);
    HeavyFeed hfeed(fitem, hdl);

    hbuf_.push_front(hfeed);
  }
}

void UserInfo::GetSharedFeeds(FeedItemSeq & fitems) {
  IceUtil::Mutex::Lock lock(guide_mutex_);
  guide_hbuf_.clear();

  BOOST_FOREACH(FeedItem & fitem, fitems) {
    if( fitem.time < 0){//防止FeedCache中的脏数据
      MCE_WARN("UserInfo::GetSharedFeeds. --> uid:" << uid_
          << " fid:" << fitem.feed
          << " stype:" << (fitem.type & 0xFFFF)
          << " actor:" << fitem.actor
          << " time:" << fitem.time
          );
      continue;
    }

    SharedFeedPtr hdl = SharedFeedCache::instance().Add(fitem);
    HeavyFeed hfeed(fitem, hdl);
    guide_hbuf_.push_back(hfeed);
  }
  MCE_INFO("UserInfo::GetSharedFeeds uid:" << uid_
      << " fitem_size:" << fitems.size()
      << " guide_hbuf_size:" << guide_hbuf_.size());
}

void UserInfo::AddBuffer4Guide() {
	IceUtil::Mutex::Lock lock(mutex_);
  IceUtil::Mutex::Lock lock_guide(guide_mutex_);
  int sucess = 0;
  BOOST_FOREACH(HeavyFeed & hfeed, guide_hbuf_){
    if ((int)hbuf_.size() >= kHbufSize) {
      MCE_DEBUG("UserInfo::AddBuffer4Guide --> hbuf full, "
          << "abondon the rest guide items!"
          << " uid:" << uid_
          << " guide_hbuf_size:" << guide_hbuf_.size()
          << " Sucess:" << sucess);
      break ;
    }
    ++sucess;
    hbuf_.push_back(hfeed); // should add the mutex here?
  }
  MCE_INFO("UserInfo::AddBuffer4Guide --> "
      << " uid:" << uid_
      << " hbuf_size:" << hbuf_.size()
      << " Sucess:" << sucess);
}

bool UserInfo::LoadFeedCache() {
  FeedItemSeq fitems;
  TimeStat ts;
  SetReadAllFeedCache();

  try{
    fitems = FeedCacheNReplicaAdapter::instance().get(uid_);
  }catch(Ice::Exception& e){
    MCE_WARN("UserInfo::LoadFeedCache. --> uid:" << uid_ << " time:" << ts.Get()<< " error:" << e );
    return false;
  }
  SetFriendPrefer(fitems, false);
  GetSharedFeedsAndAddBuffer(fitems, true);
  AddBuffer4Guide();
  MCE_INFO("UserInfo::LoadFeedCache.-->  after get from FeedCache.  "
      << " FeedCacheSize normal feed size:" << fitems.size()
      << " hbuf_:" << hbuf_.size()
      << " cost:" << ts.Get()
      << " line:" << __LINE__);
  return true;
}

bool UserInfo::LoadGuideFeedCache() {
  try {
    TaskManager::instance().execute(new GuideFeedLoadTask(uid_, this));
  } catch (Ice::Exception& e) {
    MCE_WARN("GuideFeedLoadTask::LoadGuideFeedCache. --> uid:" << uid_	<< " error:" << e );
  } catch (...) {
    MCE_WARN("UserInfo::LoadGuideFeedCache. failed!--> uid:" << uid_  );
    return false;
  }

  MCE_INFO("UserInfo::LoadGuideFeedCache. --> Task: get from guidefriends. "
      << " line:" << __LINE__);
  return true;
}

void GuideFeedLoadTask::handle(){
  FeedMetaSeq tmp_metas, metas;
  TimeStat ts;

  IntSeq friendids = GetCommonFriends(uid_);
  try {
    metas = FeedItemCacheByUseridReplicaAdapter::instance().getByTypes(friendids, 0, 100, fixed_types_);
    //		for (IntSeq::iterator fit = friendids.begin();
    //				fit != friendids.end(); ++fit) {
    //			tmp_metas = FeedItemCacheByUseridReplicaAdapter::instance().getByTypes(
    //					IntSeq((*fit)), 0, 5, fixed_types_);
    //			metas.insert(metas.end(), tmp_metas.begin(), tmp_metas.end());
    //		}
  } catch (Ice::Exception& e) {
    MCE_WARN("GuideFeedLoadTask::LoadFeedCache. --> uid:" << uid_	<< " error:" << e );
  }
  FeedItemSeq fitems = BuildFeedItem(metas);

  MCE_INFO("GuideFeedLoadTask::handle. get from guidefriends. --> uid:" << uinfo_->GetUid()
      << " GuideFriendsize:" << friendids.size()
      << " FeedItemize:" << fitems.size()
      << " cost:" << ts.Get()
      << " line:" << __LINE__);

  uinfo_->SetFriendPrefer(fitems, true);
  uinfo_->GetSharedFeeds(fitems);
}

FeedItemSeq GuideFeedLoadTask::BuildFeedItem(FeedMetaSeq& metas) {
  FeedItemSeq fitems;
  fitems.reserve(metas.size());
  FeedItem item;
  for (FeedMetaSeq::iterator it =  metas.begin(); 
        it != metas.end(); ++it) {
    item.feed = it->feedid;
    item.merge = it->mergeid;
    item.type = it->type;
    item.time = it->time;
    item.weight = it->weight & 0x00ffffff;//高8位清零
    item.actor = it->userid;

    fitems.push_back(item);
  }
  return fitems;
}

IntSeq GuideFeedLoadTask::GetCommonFriends(int uid) {
  int limit = 20;
  IntSeq fids;
  OfferDataNSeq cfs;
  try {
    cfs = OfferFriendsCacheAdapter::instance().getBestOffer(uid, limit);
  } catch (Ice::Exception& e) {
    MCE_WARN("GuideFeedLoadTask::GetCommonFriends. --> uid:" << uid << " error:" << e);
    return fids;
  }
  ostringstream oss, oss_count, oss_type;
  oss << "RecGuideFeed_numOfRec|" << uid ;
  for(OfferDataNSeq::iterator cit = cfs.begin(); cit != cfs.end(); ++cit) {
    fids.push_back(cit->userId);
  }
  return fids;
}

//--------------------------------------

void UserInfo::MergeAndSort(GetRequest & req){
  IceUtil::Mutex::Lock lock(mutex_);

  MergeFeed(req); //得到stype_merge的1DHfeed

  if (req.IsSort())
    GatherFeed(req);
  else
    GatherFeedSimple(req);

  req.GetShowedFids();
}

/*static void PrintScore(StypemergeScoreMap & vstypemerge_score){//临时测试用
	BOOST_FOREACH(StypemergeScoreMap::value_type & p, vstypemerge_score){
		ostringstream oss;
		oss.setf(ios::fixed);
		oss << p.second;
		MCE_INFO(p.first.first << " " << p.first.second << " " << oss.str());
	}
}
*/
void UserInfo::MergeFeed(GetRequest & req){
  StypemergeScoreMap vstypemerge_score;

  MergeByStypeMergeAndGetScore(req, vstypemerge_score);
  //	if(req.ShowComment()){
  //		ReplaceScoreByCommentScore(req, req.comment_map_, vstypemerge_score);
  //	}
  MakeScoreIteratorMultiMap(req, vstypemerge_score);

  MCE_INFO("UserInfo::MergeFeed. --> merge done. " << req.ToString());
}

void UserInfo::ReplaceScoreByCommentScore(GetRequest & req, StypemergeHFeed1DMap & comment_map_,
    StypemergeScoreMap & vstypemerge_score ){
  BOOST_FOREACH(StypemergeHFeed1DMap::value_type & p, comment_map_){
    StypemergeScoreMap::iterator mit = vstypemerge_score.find(p.first);
    if(mit != vstypemerge_score.end()){
      mit->second =(p.second.front())->GetScore(req.uid_,req.IsSort(),req.GetNowTime());
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

    pair<int,long> key = hfeed->GetVersionStypeMerge();
    MCE_DEBUG("UserInfo::MergeByStypeMerge --> " << hfeed->ToString());
    // NO.1 安stype merge合并到同个hfeed1D中
    StypemergeHFeed1DMap::iterator mit = req.stypemerge_item1d_map_.find(key);

    if (mit != req.stypemerge_item1d_map_.end()) {//TODO
      mit->second.push_back(hfeed);
      if(hfeed->IsUpdateTime()){
        vstypemerge_score[key] = hfeed->GetScore(req.uid_,req.IsSort(),time(NULL));
      }
    } else {
      req.stypemerge_item1d_map_[key].push_back(hfeed);
      vstypemerge_score[key] = hfeed->GetScore(req.uid_,req.IsSort(),time(NULL));
    }

    ++it;
  }
}

void UserInfo::MakeScoreIteratorMultiMap( GetRequest& req, StypemergeScoreMap& vstypemerge_score){
	for(StypemergeScoreMap::iterator mit = vstypemerge_score.begin();
			mit != vstypemerge_score.end(); ++mit){
		req.score_iterator_multimap_.insert(
				make_pair(mit->second, req.stypemerge_item1d_map_.find(mit->first))
				);
	}
}

void UserInfo::GatherFeed(GetRequest & req){
  StypeactorTimeindexMap stypeactor_timeindex;

  BOOST_REVERSE_FOREACH(ScoreIteratorMultiMap::value_type & p, req.score_iterator_multimap_){
    StypemergeHFeed1DMap::iterator & tmpitr = p.second;
    HFeed1D & item1d = tmpitr->second;
    HeavyFeedPtr hfeed = item1d.front();

    //NO.2 按stype actor 时间合并
    StypeactorTimeindexMap::iterator ti;
    if( FindSameStypeActor(hfeed, stypeactor_timeindex, ti) && 
        (TimeDiffLessThanLimit(hfeed->GetTime(), ti->second.first, hfeed->GetStype())) ){
      req.hfeed3d_[ti->second.second].push_back(item1d);
    } else {
      HFeed2D item2d;
      item2d.push_back(item1d);
      req.hfeed3d_.push_back(item2d);
      stypeactor_timeindex[hfeed->GetGatherKey()]
        = make_pair(hfeed->GetTime(), req.hfeed3d_.size()-1);
    }
  }
  MCE_DEBUG("UserInfo::GatherFeed. --> Gather done. "<< req.ToString());
}

void UserInfo::GatherFeedSimple(GetRequest & req){

	BOOST_REVERSE_FOREACH(ScoreIteratorMultiMap::value_type & p, req.score_iterator_multimap_) {
		StypemergeHFeed1DMap::iterator & tmpitr = p.second;
		HFeed1D & item1d = tmpitr->second;

		HFeed2D item2d;
		item2d.push_back(item1d);

		req.hfeed3d_.push_back(item2d);
	}
}

void UserInfo::SetReadAll(){
	IceUtil::Mutex::Lock lock(mutex_);
	hbuf_.clear();
  guide_hbuf_.clear();
}

void UserInfo::SetReadAllFeedCache(){
	IceUtil::Mutex::Lock lock(mutex_);
	hbuf_.clear();
}

vector<long> UserInfo::SetRead(int stype,long merge ) {
	vector<long> res;
	{
		IceUtil::Mutex::Lock lock(mutex_);
		for(HBuf::iterator it = hbuf_.begin(); it != hbuf_.end();){
			HeavyFeed & hfeed = *it;
      if (stype == 8182 && hfeed.SameStype(stype) ) {//!!!!注意对 在玩什么 这里也有相关处理
		  	MCE_DEBUG("UserInfo::SetRead --> stype: " << stype << " merge: " << merge);
				res.push_back(hfeed.GetFid());
				it = hbuf_.erase(it);
      }
      else if(hfeed.SameStypeMerge(stype,merge)){
				res.push_back(hfeed.GetFid());
				it = hbuf_.erase(it);
			}else{
				++it;
			}
		}
	}

	try {
			BOOST_FOREACH(long fid, res) {
				FeedCacheLogicReplicaAdapter::instance().remove(uid_, fid, true);
			}
	} catch (Ice::Exception& e) {
			MCE_WARN("UserInfo::SetReadByIdsExactly. --> call cache logic fail. uid:" << uid_ 
          << " fids:" << PrintHelper::Print(res)
          << " error:" << e);
	}

	{
		IceUtil::Mutex::Lock lock(guide_mutex_);
		for(HBuf::iterator it = guide_hbuf_.begin(); it != guide_hbuf_.end();){
			HeavyFeed & hfeed = *it;
      if(hfeed.SameStypeMerge(stype,merge)){
		  	MCE_DEBUG("UserInfo::SetRead in guide_hbuf--> stype: " << stype << " merge: " << merge);
				res.push_back(hfeed.GetFid());
				it = guide_hbuf_.erase(it);
			}else{
				++it;
			}
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

void UserInfo::SetReadByIdsExactly(const set<long> & fids,bool remove_feedcache) {
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
	if (remove_feedcache) {
		try{
			BOOST_FOREACH(long fid,fids){
				FeedCacheLogicReplicaAdapter::instance().remove(uid_,fid,true);//现在暂时不调用
			}
		}catch (Ice::Exception& e) {
			MCE_WARN("UserInfo::SetReadByIdsExactly. --> call cache logic fail. uid:" << uid_ << " fids:" << PrintHelper::Print(fids)<<" error:"<< e);
		}
	}

	MCE_INFO("UserInfo::SetReadByIdsExactly . --> uid:" << uid_ << " fids size:"
			<< fids.size() << " fids:"<< PrintHelper::Print(fids) << " remove hfeeds:" << PrintHelper::Print(hfeeds));
}

FeedItemSeq UserInfo::GetOrigFeedItems() {
	IceUtil::Mutex::Lock lock(mutex_);
	FeedItemSeq res;
	BOOST_FOREACH(const HBuf::value_type & hfeed,hbuf_) {
		res.push_back(hfeed.ToFeedItem());
	}
	return res;
}

template<typename T>
void UserInfo::SetReadInFeedCache(const T & fids) {
	BOOST_FOREACH(long fid,fids) {
		FeedCacheLogicReplicaAdapter::instance().remove(uid_, fid,true);
	}
	MCE_INFO("UserInfo::SetReadInFeedCache. --> uid:" << uid_ << " fids:" << PrintHelper::Print(fids));
}

int UserInfo::Size() const {
	IceUtil::Mutex::Lock lock(mutex_);
	return hbuf_.size();
}

string UserInfo::ToString() const {
	ostringstream os;
	os << " uid:" << uid_ << " date:" << expire_date_ 
    << " bufsize:" << hbuf_.size() << " guide_buf_size:" << guide_hbuf_.size() ;
	return os.str();
}

bool UserInfo::CheckUniq() {
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
		MCE_INFO("UserInfo::CheckUniq. --> uid:" << uid_ << " success");
		return true;
	}else{
		MCE_WARN("UserInfo::CheckUniq. --> uid:" << uid_ << " fail. dup fids:" << PrintHelper::Print(dups));
		return false;
	}
}

//确认从大到小
bool UserInfo::CheckOrder(FeedItemSeq & fitems) {
	long fid = 0;
	bool ordered = true;
	BOOST_REVERSE_FOREACH(FeedItem & fitem,fitems) {
		if(fid > fitem.feed){
			MCE_WARN("UserInfo::CheckOrder. --> bad order.uid:" << uid_<<" last:" << fid << " this:" << fitem.feed);
			ordered = false;
		}
	}
	MCE_INFO("UserInfo::CheckOrder. --> uid:" << uid_ << " ordered:" << ordered);
	return ordered;
}


//-----------------------
//socialgraph log
/*

void LogStatsTask::InitLogPath() {
  using namespace com::xiaonei::service;
  log_paths_.push_back("socialgraph");
  log_paths_.push_back("reconnect");
  log_paths_.push_back("homeReconnect");
  remote_logger_ = XceLoggerAdapter::instance().getLogger();
  remote_logger_->initialize();
}

void LogStatsTask::handle() {
  if (!param_.empty()) {
    remote_logger_->addLog(log_paths_, param_);
  }
}
*/
