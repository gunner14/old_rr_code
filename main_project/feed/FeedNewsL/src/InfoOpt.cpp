/*********************************
* InfoOpt.cpp 
* 新排序流程 
* by jieyu.zeng
* @2012-12-03
**********************************/
#include "InfoOpt.h"

namespace xce {
namespace feed {

bool ScoreCmpBase(const InfoOptBase& a, const InfoOptBase& b) {
  return (a.score > b.score); //score高的排前面
}
bool ScoreCmp1D(const InfoOpt1D& a, const InfoOpt1D& b) {
  return (a.score > b.score); //score高的排前面
}
bool ScoreCmp2D(const InfoOpt2D a, const InfoOpt2D& b) {
  return (a.score > b.score); //score高的排前面
}
/******************************************************
* Merge 的内部排序和整体权重
******************************************************/
void InfoOpt1D::RankAndScore(double score_ratio) {
  if (Merged()) {
    sort(base_vec.begin(), base_vec.end(), ScoreCmpBase);  
    score = base_vec.front().score * score_ratio;
  } else if(!Empty()) {
    score = base_vec.front().score; 
  }
}
/******************************************************
* Fold 的内部排序和整体权重
******************************************************/
void InfoOpt2D::RankAndScore(double score_ratio) {
  if (Fold()) {
    sort(opt1d_vec.begin(), opt1d_vec.end(), ScoreCmp1D);
    score = opt1d_vec.front().score * score_ratio;
  } else if (!Empty()) {
    score = opt1d_vec.front().score; 
  }
}
/*******************************************************
* 简单的分流
*******************************************************/
bool InfoOpt::UseInfoOpt(int uid){
  return (6 == ((uid / 1000) % 10)); //千位为6
}

/*******************************************************
* 对外接口
*******************************************************/
void InfoOpt::Optimize(GetRequest& req, HBuf& hbuf, const InfoOptPara& para){
  InfoOptBaseVec base_vec; 
  InfoOpt2DVec opt2d_vec;
  Score(req, hbuf, base_vec, para);
  InfoMerge(req, base_vec, opt2d_vec);
  Rank(req, opt2d_vec, para);
  GenResult(req, opt2d_vec);
}
  
/*******************************************************
* 对每条符合条件的feed进行打分
*******************************************************/
void InfoOpt::Score(GetRequest& req, HBuf& hbuf, InfoOptBaseVec& base_vec, const InfoOptPara& para){
  int feednum = 0, filter_num = 0, scorenum = 0;
  for(HBuf::iterator it = hbuf.begin(); it != hbuf.end();){
    ++feednum;
    HeavyFeedPtr hfeed = &(*it);    
    if (ReqFilter(req, hbuf, hfeed, it)) {
      ++filter_num;
      continue;
    }
    double bayestime = 0;
    double score = 
        hfeed->GetScoreLua(req.uid_, req.IsSort(), req.GetNowTime(), para.max_read_, para.recent_cnt_, para.refresh_time_, bayestime);
    //MCE_INFO("InfoOpt::Score "<<hfeed->GetFid() <<":"<<score); 
    InfoOptBase item(hfeed, score);
    base_vec.push_back(item);
    ++it;
    ++scorenum;
  }
  MCE_DEBUG("InfoOpt::Score feednum="<< feednum << " filter_num" << filter_num <<" scorenum="<< scorenum); 
}

/*******************************************************
* 信息整合, 包括原merge和fold
*******************************************************/
void InfoOpt::InfoMerge(GetRequest& req, InfoOptBaseVec& base_vec, InfoOpt2DVec& opt1d_vec_vec){
  InfoOpt1DVec opt1d_vec;
  Merge(base_vec, opt1d_vec);
  Fold(opt1d_vec, opt1d_vec_vec);
}

/*******************************************************
* Rank : 包括原来的sort, 二次排序也要从这里
*******************************************************/
void InfoOpt::Rank(GetRequest& req, InfoOpt2DVec& opt2d_vec, const InfoOptPara& para){
  ScoreRank(opt2d_vec);

/* DEBUG Score and Rank*
  MCE_INFO("DEBUG Rank of uid="<<req.uid_);
  int rank = 0, base_num = 0;
  for (InfoOpt2DVec::iterator it2d = opt2d_vec.begin(); it2d != opt2d_vec.end(); ++it2d){
    ostringstream os;
    if (it2d->Fold()) {
      os << "[Fold]";
    }
    for (InfoOpt1DVec::iterator it1d = it2d->opt1d_vec.begin(); it1d != it2d->opt1d_vec.end(); ++it1d){
      if (it1d->Merged()) {
        os << "[Merged]";
      }
      os << "score="<< it1d->score << "(";
      base_num += it1d->Size();
      for (InfoOptBaseVec::iterator itbase = it1d->base_vec.begin(); itbase != it1d->base_vec.end(); ++itbase){
        os << " " << itbase->hfeed->GetFid() << ":" <<itbase->score;
      }
      os << ")";
    }
    MCE_INFO("rank=" << rank++ << ",score=" << it2d->score << ",feed:"<< os.str());
  }
  MCE_INFO("InfoOpt::Rank total index="<< rank <<" base_num=" << base_num);
**/
}


/*******************************************************
* 评论类新鲜事的score替换
*******************************************************/
void InfoOpt::CommentScore(GetRequest& req, InfoOpt1DVec& opt1d_vec){
  //TODO 暂时不实现 
}

/*******************************************************
* 原来的过滤及处理, CheckType, SeperateComment, Edm
*******************************************************/
bool InfoOpt::ReqFilter(GetRequest& req, HBuf& hbuf, HeavyFeedPtr& hfeed, HBuf::iterator& it) {
  if((!req.CheckType(hfeed->GetStype())) || req.SeperateComment(hfeed)) {
    ++it;
    return true;
  }
  
  bool is_edm, must_erase; //TODO 剥离EDM的这部分
  boost::tie(is_edm,must_erase) = req.SeperateEdm(hfeed);
  if(is_edm){
    if(must_erase) {
      it = hbuf.erase(it);
    } else {
      ++it;
    }
    return true;
  }
  return false; 
}

/*******************************************************
* 原Merge策略, by (stype + merge)
*******************************************************/
void InfoOpt::Merge(InfoOptBaseVec& base_vec, InfoOpt1DVec& opt1d_vec) {
  
  InfoOpt1DVecIdxMap merge_pool;
  for (InfoOptBaseVec::iterator it = base_vec.begin(); it != base_vec.end(); ++it) {
    pair<int,long> key = it->hfeed->GetVersionStypeMerge();
    InfoOpt1DVecIdxMap::iterator mit = merge_pool.find(key);
    if (mit != merge_pool.end()) { //有合并
      opt1d_vec[mit->second].AddInfoOptBase(*it);
    } else {
      InfoOpt1D item(*it);
      opt1d_vec.push_back(item);
      merge_pool[key] = opt1d_vec.size()-1;
    }
  }
  MCE_DEBUG("InfoOpt::Merge uniquenum="<< opt1d_vec.size());
/*
  int base_num = 0;
  for (InfoOpt1DVec::iterator it = opt1d_vec.begin(); it != opt1d_vec.end(); ++it) {
    base_num += it->Size(); 
  }
  MCE_INFO("InfoOpt::Merge uniquenum="<< opt1d_vec.size() <<" base_num=" << base_num); 
*/
}
/*******************************************************
* 原fold策略, by (stype) & by (stype + actor)
*******************************************************/
void InfoOpt::Fold(InfoOpt1DVec& opt1d_vec, InfoOpt2DVec& opt2d_vec) {
  //& 填充merge策略1D/不触发Fold的1D到 2D vector
  // TODO 将每个Fold策略单拆,策略插件化

  StypeactorTimeindexMap stypeactor_timeindex;
  map<int, int> stype_index;
  for (InfoOpt1DVec::iterator it = opt1d_vec.begin(); it != opt1d_vec.end(); ++it) {
    InfoOpt1D item1d = (*it);
    if (item1d.Empty()) {//包含feed数为0, 异常
      continue;
    } 
    
    HeavyFeedPtr hfeed = item1d.base_vec.front().hfeed;
    if (FoldByStype(hfeed->GetStype())) { //触发 fold by stype , Merge的结果也会进行stype折叠        
      map<int, int>::iterator stypeit = stype_index.find(hfeed->GetStype());
      if (stypeit != stype_index.end()) {
        opt2d_vec[stypeit->second].AddInfoOpt1D(item1d); 
      } else {
        InfoOpt2D item2d(item1d);
        opt2d_vec.push_back(item2d);
        stype_index[hfeed->GetStype()] = opt2d_vec.size()-1;
      }
      continue;
    } 
    if (item1d.Merged()) { //若已Merged, 则不再进行其他Fold
      InfoOpt2D item2d(item1d);
      opt2d_vec.push_back(item2d);
      continue;
    }
     
    StypeactorTimeindexMap::iterator ti;
    if (FoldByStypeActor(hfeed, stypeactor_timeindex, ti)){ //触发 fold by stype&actor
      opt2d_vec[ti->second.second].AddInfoOpt1D(item1d);
    } else {
       InfoOpt2D item2d(item1d);
      opt2d_vec.push_back(item2d);
      stypeactor_timeindex[hfeed->GetGatherKey()] = make_pair(hfeed->GetTime(), opt2d_vec.size()-1);
    }    
  }
  //对折叠的新鲜事进行内部排序及赋权
  for (InfoOpt2DVec::iterator it2d = opt2d_vec.begin(); it2d != opt2d_vec.end(); ){
    if (it2d->Empty()) {
      opt2d_vec.erase(it2d++); 
    } else {
      it2d->RankAndScore(1.0);
      ++it2d;
    }
  }
  MCE_DEBUG("InfoOpt::Fold uniquenum="<< opt2d_vec.size()); 
}
/******************************************************
* 判断是否需要按stype, 这里是stype触发池
******************************************************/
bool InfoOpt::FoldByStype(int stype){
  return UserInfoPool::instance().StypeInFoldSet(stype);
}

/******************************************************
* 判断是否按StypeActor, 这里是和Merge的类似, 但添加时间差判断
******************************************************/
bool InfoOpt::FoldByStypeActor(
    HeavyFeedPtr & hfeed,
    StypeactorTimeindexMap & stypeactor_timeindex,
    StypeactorTimeindexMap::iterator& ti) {
  return (FindSameStypeActor(hfeed, stypeactor_timeindex, ti)
      && (TimeDiffLessThanLimit(hfeed->GetTime(), ti->second.first,hfeed->GetStype()))); 
}

/*******************************************************
* 判断是否有同个GatherKey
*******************************************************/
bool InfoOpt::FindSameStypeActor(
    HeavyFeedPtr & hfeed,
    StypeactorTimeindexMap & stypeactor_timeindex,
    StypeactorTimeindexMap::iterator& ti){
  pair<int,int> gatherkey = hfeed->GetGatherKey();
  ti = stypeactor_timeindex.find(gatherkey);
  return (ti != stypeactor_timeindex.end());
}

/*******************************************************
* 时间窗判断
*******************************************************/
bool InfoOpt::TimeDiffLessThanLimit(time_t a,time_t b,int stype){
  if(stype == 8182)return true;
  time_t diff = abs(a - b);
  return diff < TimeSeqLimit(stype);;
}

/*******************************************************
* GenResult : 把排序结果数据按request的格式放入req.hfeed3d_
*******************************************************/
void InfoOpt::GenResult(GetRequest& req, InfoOpt2DVec& opt2d_vec) {
  for (InfoOpt2DVec::iterator it2d = opt2d_vec.begin(); it2d != opt2d_vec.end(); ++it2d){
    HFeed2D hfeed2d;
    for (InfoOpt1DVec::iterator it1d = it2d->opt1d_vec.begin(); it1d != it2d->opt1d_vec.end(); ++it1d){
      HFeed1D hfeed1d;
      for (InfoOptBaseVec::iterator itbase = it1d->base_vec.begin(); itbase != it1d->base_vec.end(); ++itbase){
        hfeed1d.push_back(itbase->hfeed);
      }
      hfeed2d.push_back(hfeed1d);
    }
    req.hfeed3d_.push_back(hfeed2d);
  }
}
/*******************************************************
* score rank : 按score序得出结果序
*******************************************************/
void InfoOpt::ScoreRank(InfoOpt2DVec& opt2d_vec){
  sort(opt2d_vec.begin(), opt2d_vec.end(), ScoreCmp2D);
}
  
} // end of namespace feed
} // end of namespace xce

