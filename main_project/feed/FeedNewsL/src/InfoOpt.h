/*********************************
* InfoOpt.h 
* 新排序流程 
* by jieyu.zeng
* @2012-12-03
**********************************/
#ifndef INFOOPT_H
#define INFOOPT_H
#include "Common.h"
#include "UserInfo.h"

namespace xce {
namespace feed {

/**********************************************
*  从UserInfo传过来的参数
***********************************************/
class InfoOptPara{
public:
  InfoOptPara(){}
  InfoOptPara(long max_read, int recent_cnt, time_t refresh_time){
    max_read_ = max_read; 
    recent_cnt_ = recent_cnt;
    refresh_time_ = refresh_time;
  } 
  time_t refresh_time_;
  long max_read_;  
  int recent_cnt_;
};

/**********************************************
*  Score的输出单元, Merge(merge/fold)的输入单元
***********************************************/
class InfoOptBase {
public:
  InfoOptBase(HeavyFeedPtr& _hfeed, double _score){ 
    hfeed = _hfeed;
    score = _score;
  }
  double score;
  HeavyFeedPtr hfeed;
};
typedef vector<InfoOptBase> InfoOptBaseVec;
/**********************************************
*  merge的输出单元, fold的输入单元
***********************************************/
class InfoOpt1D{
public:
  InfoOpt1D(){
    score = 0.0; 
  }
  InfoOpt1D(InfoOptBase& unit){
    base_vec.push_back(unit);
  }
  void AddInfoOptBase(InfoOptBase& unit){
    base_vec.push_back(unit);
  }
  int Size(){
    return base_vec.size();
  }
  bool Empty(){
    return 0 == base_vec.size();
  }
  bool Merged(){
    return base_vec.size() > 1;
  }
  void RankAndScore(double score_ratio);
  
  InfoOptBaseVec base_vec;
  double score;
};
typedef vector<InfoOpt1D> InfoOpt1DVec;
typedef map<pair<int, long>, int> InfoOpt1DVecIdxMap;
/**********************************************
*  Merge(merge/fold)的输出单元, Rank的输入单元
***********************************************/
class InfoOpt2D{
public:
  InfoOpt2D(){
    score = 0.0; 
  }
  InfoOpt2D(InfoOpt1D& item){
    item.RankAndScore(1.0);
    opt1d_vec.push_back(item);
  }
  void AddInfoOpt1D(InfoOpt1D& item) {
    item.RankAndScore(1.0);
    opt1d_vec.push_back(item);
  }
  int Size(){
    return opt1d_vec.size();
  }
  bool Empty(){
    return 0 == opt1d_vec.size();
  }
  bool Fold(){
    return opt1d_vec.size() > 1;
  }
  void RankAndScore(double score_ratio);
  
  InfoOpt1DVec opt1d_vec; 
  double score;
};
typedef InfoOpt2D* InfoOpt2DPtr;
typedef vector<InfoOpt2D> InfoOpt2DVec;
/*************************************************
* 新feed 排序: Score->Merge(merge/fold)->Rank
**************************************************/
class InfoOpt: public Singleton<InfoOpt> { 
public:
  InfoOpt(){
    time_seq_limit_ = 60 * 5; 
  };
  bool UseInfoOpt(int uid);
  void Optimize(GetRequest& req, HBuf& hbuf, const InfoOptPara& para);

private: 

  void Score(GetRequest& req, HBuf& hbuf, InfoOptBaseVec& base_vec, const InfoOptPara& para);
  void InfoMerge(GetRequest& req, InfoOptBaseVec& base_vec, InfoOpt2DVec& opt2d_vec);
  void Merge(InfoOptBaseVec& base_vec, InfoOpt1DVec& opt1d_vec);
  void Fold(InfoOpt1DVec& opt1d_vec, InfoOpt2DVec& opt2d_vec);
  void Rank(GetRequest& req, InfoOpt2DVec& opt2d_vec, const InfoOptPara& para);
  void ScoreRank(InfoOpt2DVec& opt2d_vec);
  void GenResult(GetRequest& req, InfoOpt2DVec& opt2d_vec);

  bool ReqFilter(GetRequest& req, HBuf& hbuf, HeavyFeedPtr& hfeed, HBuf::iterator& it);
  void CommentScore(GetRequest& req, InfoOpt1DVec& opt1d_vec);
  bool FoldByStype(int stype);
  bool FoldByStypeActor(HeavyFeedPtr& hfeed, StypeactorTimeindexMap& stypeactor_timeindex, StypeactorTimeindexMap::iterator& ti);
  bool FindSameStypeActor(HeavyFeedPtr& hfeed, StypeactorTimeindexMap& stypeactor_timeindex, StypeactorTimeindexMap::iterator& ti);
  bool TimeDiffLessThanLimit(time_t a,time_t b,int stype);
 
  time_t TimeSeqLimit(int stype) { //TODO 时间窗分stype和fold strategy 
    return time_seq_limit_;
  }
  time_t time_seq_limit_;
};

} // end of namespace feed
} // end of namespace xce

#endif /* INFOOPT_H */
