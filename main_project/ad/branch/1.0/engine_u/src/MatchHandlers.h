#ifndef AD_MACHER_HANDLERS_H_
#define AD_MACHER_HANDLERS_H_

#include <IceUtil/Shared.h>
#include <boost/algorithm/string/regex.hpp>
#include "AdEngine.h"
#include "AdStruct.h"
#include "AdConfig.h"
#include "../../util/src/AdCommonUtil.h"
//#include "AdEffect.h"

namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;
using namespace boost;


struct MatchHandlerParamter {
  MatchHandlerParamter(const UserProfile& userProfile,
                       const string& append_para):
                       userProfile_(userProfile), rotate_index_(0), has_re_(0), tag_(""), photo_(false), count_(0), is_selected_vip_(false) {
    vector < string > para_strs;
    boost::algorithm::split_regex(para_strs, append_para, regex("\\^C"));
    if (para_strs.size() >= 3) {
      referer_ = para_strs[0];
      next_load_time_ = para_strs[1];
      refresh_idx_ = para_strs[2];       
    } else if (para_strs.size() >= 1) {
      referer_ = para_strs[0];
    }
    vector < string > referer_strs;
    boost::algorithm::split_regex(referer_strs, referer_, regex("refresh_idx="));
    if (referer_strs.size() > 1) {
      vector < string > refresh_strs;
      boost::algorithm::split_regex(refresh_strs , referer_strs[1], regex("&"));
      if (refresh_strs.size() > 0) {
        refresh_idx_ = refresh_strs[0];
      }
    }
    //MakeSessionId
    struct timeval t;
    gettimeofday(&t, NULL);
    int rand_len = 20;//20bit放随机数 
    Ice::Long time_field = (Ice::Long)(t.tv_sec) * 1000L + t.tv_usec / 1000L;//毫秒数 
    session_id_ =  ((time_field << rand_len) | (rand() & ((1 << rand_len) - 1)));   
  };

  int count() const {
    if (EngineConfig::instance().IsForBrand()) {
      return adzone_->brand_count();
    } else if(photo_ && adzone_->id() == 100000000072) {
      return EngineConfig::instance().photo_wb1_count();
    } else if(count_ && adzone_->id() == 100000000063) {
      return count_;
    } else {
      int cnt = adzone_->bp_count();
      if( style_ ){
        cnt = style_->ad_cnt();
      }
      return cnt;
    }
  }

  unsigned int PickCount() const {  // 返回还需要选取的广告数
    return count() - zone_selected_groups_.size();
    /*if (EngineConfig::instance().IsForBrand()) {
      return adzone_->brand_count() - zone_selected_groups_.size();
    } else if(photo_ && adzone_->id() == 100000000072) {
      return EngineConfig::instance().photo_wb1_count() - zone_selected_groups_.size();
    } else if(count_ && adzone_->id() == 100000000063) {
      return count_ - zone_selected_groups_.size();
    } else {
      int cnt = adzone_->bp_count();
      if( style_ ){
        cnt = style_->ad_cnt();
      }
      return cnt - zone_selected_groups_.size();
    }*/
  }

  UserProfile userProfile_;
  string referer_;
  string next_load_time_;
  string refresh_idx_;
  AdStylePtr style_;
  AdZonePtr adzone_;
  int rotate_index_;
  set<SelectedAdGroupPtr> total_selected_groups_;
  set<SelectedAdGroupPtr> zone_selected_groups_;
  set<AdGroupPtr> all_selected_groups_;
  vector<SelectedAdGroupPtr> final_selected_groups_;
  bool has_re_;
  string tag_;
  bool photo_;
  int count_;
  bool is_selected_vip_;
  Ice::Long session_id_;
};

class AdMatchHandler : public IceUtil::Shared {
public:
  virtual bool handle(MatchHandlerParamter& para, AdResultMap& result) = 0;
};

class RotationHandler : public AdMatchHandler {
public:
  virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
};

class DefaultHandler : public AdMatchHandler {
public:
  virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
};

class PosAdjustHandler : public AdMatchHandler {
public:
  virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
};

class StyleHandler : public AdMatchHandler {
public:
  virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
public :
  bool isBig(int widget_id);
};

class LogHandler : public AdMatchHandler {
public:
  virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
};

}
}

#endif // AD_MACHER_HANDLERS_H_
