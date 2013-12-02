#ifndef _USER_RANK_HANDLER_H_
#define _USER_RANK_HANDLER_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <IceUtil/Shared.h>
#include "AdStruct.h"
#include "MatchHandlers.h"
#include "AdDemoQualityAdapter.h"
#include "AdDemoQuality.h"
using boost::multi_index_container;
using namespace boost::multi_index;

namespace xce {
namespace ad {

class AdMatchHandler;
class AdSelectHandler;
typedef IceUtil::Handle<AdSelectHandler> AdSelectHandlerPtr;

struct UserIndexSelectItem {
  AdGroupPtr gptr;
  int state;
  double ctr;
  double ecpm;
};

class UserRankHandler : public AdMatchHandler {
public:
  virtual bool handle(MatchHandlerParamter& para, AdResultMap& result);
private:
  void GetAds(MatchHandlerParamter &para, long zone_id, set<AdGroupPtr> &groups);
	void InitSelectPara(MatchHandlerParamter & para);
};
////////////////////////////////////////////////
class FreshAdGroupsCache : public MyUtil::Singleton<FreshAdGroupsCache> {
public:
  FreshAdQualityMap GetFreshGroups(){
    FreshAdQualityMap result;
    {
      IceUtil::RWRecMutex::RLock lock(mutex_);    
      result = fresh_groups_;
    }
    return result;
  }
  void Load() {
    MCE_INFO("[Fresh:],FreshAdGroupsCache::Load start");
    FreshAdQualityMap pool;
    try {
      pool = AdDemoQualityAdapter::instance().GetFreshAdMap(EngineConfig::instance().server_index());
    }catch (Ice::Exception& e) {
      MCE_WARN("FreshAdGroupsCache::Load --> call AdDemoQualityNAdapter ERR  line:"
          << __LINE__ << " err:" << e); 
    } catch (std::exception& e) {
      MCE_WARN("FreshAdGroupsCache::load --> call AdDemoQualityNAdapter ERR  line:"
          << __LINE__ << " err:" << e.what());
    }

    IceUtil::RWRecMutex::WLock lock(mutex_);
    fresh_groups_.swap(pool);
    MCE_INFO("[Fresh:]: Load stop, GetFreshAdMap size is:"  << fresh_groups_.size());          
  }
private:
  FreshAdQualityMap fresh_groups_;
  IceUtil::RWRecMutex mutex_;
};
class CpmLoaderCache : public MyUtil::Singleton<CpmLoaderCache> {
public:
  CpmAdgroupMap GetCpmAdGroupMap(){
  CpmAdgroupMap result;
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);    
    result = cpm_groups_;
  }
  return result;
  }
  void Load() {
    MCE_INFO("[cpm loader],CpmLoaderCache::Load start");
    CpmAdgroupMap pool;
    try {
      pool = AdDemoQualityAdapter::instance().GetCpmAdGroupMap(EngineConfig::instance().server_index());
      IceUtil::RWRecMutex::WLock lock(mutex_);
      cpm_groups_.swap(pool);
      MCE_INFO("[cpm loader]: Load stop, CpmLoaderCache size is:"  << cpm_groups_.size());          
    }catch (Ice::Exception& e) {
      MCE_WARN("CpmLoaderCache::Load --> call AdDemoQualityNAdapter CpmLoaderCache ERR  line:"
          << __LINE__ << " err:" << e); 
    } catch (std::exception& e) {
      MCE_WARN("CpmLoaderCache::load --> call AdDemoQualityNAdapter CpmLoaderCache ERR  line:"
          << __LINE__ << " err:" << e.what());
    }

  }
private:
  CpmAdgroupMap cpm_groups_;
  IceUtil::RWRecMutex mutex_;
};
/*
class TestAdImprRatioAssignCache : public MyUtil::Singleton<TestAdImprRatioAssignCache> {
public:
  TestAdImprRatioAssignCache(): yestoday_reserve_impr_(182308123), yestoday_unreserve_impr_(329373678)
  ,today_hour_reserve_impr_(80303989),today_hour_unreserve_impr_(159975961),today_predict_reserve_impr_(18230812),
  max_test_ad_impr(40000000),min_test_ad_impr(5000000),max_unreserve_test_ratio(0.03) {
  }
  double GetTestRatio(bool is_reserve){
    double ratio = 0.0;
    if(today_predict_reserve_impr_ < min_test_ad_impr) {
      if(!is_reserve) { //非余量需要测试
        ratio = (double)(min_test_ad_impr - today_predict_reserve_impr_) / (double)yestoday_unreserve_impr_;
        ratio = min(ratio, max_unreserve_test_ratio);
        MCE_INFO("TestAdImprRatioAssignCache, very Important,the reserve has no enough impr for new_ad,need unreserve test ratio :" << max_unreserve_test_ratio);
        return ratio;
      } else {//余量需要全部测试
        MCE_INFO("TestAdImprRatioAssignCache, very Important,the reserve has no enough impr for new_ad,need reserve test ratio 1.0 ");
        return 1.0;
      }
    }
    if(!is_reserve) {//不需要非余量测试
      return 0.0;
    }
    if(today_predict_reserve_impr_ < max_test_ad_impr) { //如果测试量不够，则需要全部进行测试
      return 1.0;
    } else {
      return (double)max_test_ad_impr / (double)today_predict_reserve_impr_;
    }
  }
  void Load();
private:
  Ice::Long yestoday_reserve_impr_;
  Ice::Long yestoday_unreserve_impr_;
  Ice::Long today_hour_reserve_impr_;
  Ice::Long today_hour_unreserve_impr_;
  Ice::Long today_predict_reserve_impr_;  //今天预测的余量数目
  const Ice::Long max_test_ad_impr;//最大测试广告impr不超过40000000(400);
  const Ice::Long min_test_ad_impr;//余量最小能测试50个广告(5000000)，如果余量低于50个测试广告，需要利用非余量测试
  const double max_unreserve_test_ratio;
  IceUtil::RWRecMutex mutex_;
};
*/
class TestAdImprRatioAssignCache : public MyUtil::Singleton<TestAdImprRatioAssignCache> {
public:
  TestAdImprRatioAssignCache() {}
  double GetTestRatio(Ice::Long adzone){
    double ratio, default_ratio = 0.05;
    IceUtil::RWRecMutex::RLock lock(mutex_);
    map<Ice::Long, double>::iterator it = adzone2testratio_.find(adzone);
    if (it != adzone2testratio_.end()) {
      ratio = it->second;
    } else {
      ratio = default_ratio;
    }
    return ratio;
  }
  void Load();
private:
  map<Ice::Long, double> adzone2testratio_;
  IceUtil::RWRecMutex mutex_;
};

class TestAdImprRatioAssignCacheTimer : public MyUtil::Timer {
  public:
  TestAdImprRatioAssignCacheTimer(int time =0 ) :Timer(time){
  }
  virtual void handle() {
    MCE_INFO("TestAdImprRatioAssignCacheTimer begin");
    TestAdImprRatioAssignCache::instance().Load();
    MCE_INFO("TestAdImprRatioAssignCacheTimer stop");
    MyUtil::TaskManager::instance().schedule(new TestAdImprRatioAssignCacheTimer(30 * 60 * 1000));//半小时计算一次
  }
};

class FreshAdGroupsCacheTimer : public MyUtil::Timer {
public:
  FreshAdGroupsCacheTimer() :
    Timer(5 * 60 * 1000) { //5分钟重新计算一次
  }
  virtual void handle() {
    FreshAdGroupsCache::instance().Load();
  }
};
class CpmLoaderCacheTimer : public MyUtil::Timer {
public:
  CpmLoaderCacheTimer() :
    Timer(2 * 60 * 1000) { //2分钟重新计算一次
  }
  virtual void handle() {
    CpmLoaderCache::instance().Load();
  }
};

class AdSortPosCache : public MyUtil::Singleton<AdSortPosCache> {
public:
  PosSeq GetAppSortPos(string app_name){
    PosSeq sort_pos;
    {
      IceUtil::RWRecMutex::RLock lock(mutex_);
      if (app_sort_pos.find(app_name) != app_sort_pos.end()) {
        sort_pos = app_sort_pos[app_name];
        MCE_DEBUG("[AdSortPosCache:]sort app: find app " << app_name << ",sort is " << sort_pos[0] << " " << sort_pos[1] << " " <<sort_pos[2]);
      }else{
        sort_pos.push_back(1);
        sort_pos.push_back(2);
        sort_pos.push_back(3);
        MCE_DEBUG("[AdSortPosCache:]sort app:find no app, app is " << app_name);
      }
    }
    return sort_pos;
  }
  void Load() {
    MCE_INFO("[AdSortPosCache:]App sort pos::Load start");
    AdSortPosMap local_app_sort_pos;
    try {
      local_app_sort_pos = AdDemoQualityAdapter::instance().GetAppSortPos(EngineConfig::instance().server_index());
    }catch (Ice::Exception& e) {
      MCE_WARN("AdSortPosCache::Load --> call AdDemoQualityNAdapter ERR  line:"
          << __LINE__ << " err:" << e);
    } catch (std::exception& e) {
      MCE_WARN("AdSortPosCache::load --> call AdDemoQualityNAdapter ERR  line:"
          << __LINE__ << " err:" << e.what());
    }

    IceUtil::RWRecMutex::WLock lock(mutex_);
    app_sort_pos.swap(local_app_sort_pos);
    MCE_INFO("[App sort pos:]: Load stop, GetAppSortPosMap size is:"  << app_sort_pos.size());
  }
private:
  AdSortPosMap app_sort_pos;
  IceUtil::RWRecMutex mutex_;
};

class AdSortPosCacheTimer : public MyUtil::Timer {
public:
  AdSortPosCacheTimer(int time =0) :
    Timer(time) { //30分钟重新计算一次
  }
  virtual void handle() {
    AdSortPosCache::instance().Load();
    MyUtil::TaskManager::instance().schedule(new AdSortPosCacheTimer(30 * 60 * 1000));
  }
};

class AdsQualityCache : public MyUtil::Singleton<AdsQualityCache> {
public:
  double GetAdQuScore(Ice::Long group_id,Ice::Long adzone){
    double score = 1.0;
    stringstream gidzid;
    gidzid<<group_id<<adzone;
    string score_name= gidzid.str();
    
    AdQualityScoreMap::iterator it;
    {
      IceUtil::RWRecMutex::RLock lock(mutex_score);
      it = quality_score.find(score_name);
      if (it != quality_score.end()) {
        score = it->second;
        MCE_DEBUG("groupid zoneid : "<<gidzid<<" ,score is "<<score);
      }
    }
    return score;
  }
  void Load() {
    MCE_INFO("[AdsQualityCache:]App sort pos::Load Begin");
    AdQualityScoreMap quality_score_pool;
    try {
      quality_score_pool = AdDemoQualityAdapter::instance().GetAdsQuality(EngineConfig::instance().server_index());
      if(!quality_score_pool.empty()){
        IceUtil::RWRecMutex::WLock lock(mutex_score);
        quality_score.swap(quality_score_pool);
      } else {  
        MCE_WARN("AdQualityScoreCache:: Load QualityScore empty");
      }
      MCE_INFO("GetAdsQuality size is:"  <<  quality_score.size());
     }catch (Ice::Exception& e) {
      MCE_WARN("AdQualityScore::Load --> call  AdQualityScoreNAdapter ERR  line:"
          << __LINE__ << " err:" << e);
    } catch (std::exception& e) {
      MCE_WARN("AdQualityScoreCache::load --> call  AdQualityScoreNAdapter ERR  line:"
          << __LINE__ << " err:" << e.what());
    }
    MCE_INFO("[AdsQualityCache:]App sort pos::Load End");
  }

private:
  AdQualityScoreMap quality_score;
  IceUtil::RWRecMutex mutex_score;
};

class AdsQualityTimer : public MyUtil::Timer {
public:
  AdsQualityTimer(int time = 0) :Timer(time) { 
  }
  virtual void handle() {
    AdsQualityCache::instance().Load();
    MyUtil::TaskManager::instance().schedule(new AdsQualityTimer(6 * 60 * 60 * 1000)); // 6 小时 更新一次
  }
};

//部分二跳计价的用户ecpm打折
//cache
class DiscountCache : public MyUtil::Singleton<DiscountCache> {
public:
  double GetDiscountRatio(Ice::Long member_id){
    IceUtil::RWRecMutex::RLock lock(mutex_);
    map<Ice::Long, double>::iterator itdis = discount_map_.find(member_id);
    if(itdis != discount_map_.end()) {
      return itdis->second;
    }
    return 1.0;
  }
  void Load();
private:
  map<Ice::Long,double> discount_map_;
  IceUtil::RWRecMutex mutex_;
};

//timer
class DiscountCacheTimer : public MyUtil::Timer {
public:
  DiscountCacheTimer() :
    Timer(5 * 60 * 1000) { //5分钟重新计算一次
  }
  virtual void handle() {
    MCE_DEBUG("DiscountCacheTimer begin");
    DiscountCache::instance().Load();
    MCE_DEBUG("DiscountCacheTimer stop");
  }
};

class Adzone2ChooseLogicPool : public MyUtil::Singleton<Adzone2ChooseLogicPool> {
public:
  bool GetHandleListByAdzoneID(const Ice::Long & adzone, vector<AdSelectHandlerPtr> & ad_select_handler_list, MatchHandlerParamter & para);
  void Load();
  void FillHandleListByConfig(const vector<int> & vechandle, vector<AdSelectHandlerPtr> &ad_select_handler_list);
private:
  map<Ice::Long,vector<AdSelectHandlerPtr> > adzone2handlelists_;
  IceUtil::RWRecMutex mutex_;
};

class Widget2TypeCache : public MyUtil::Singleton<Widget2TypeCache> {
public:
  int GetWidgetType(Ice::Long widget_id) {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    map<Ice::Long, int>::iterator itdis = widget2Type_map_.find(widget_id);
    if(itdis != widget2Type_map_.end()) {
       return itdis->second;
    }
    return 0;
  }
  void Load();
private:
  map<Ice::Long, int> widget2Type_map_;
  IceUtil::RWRecMutex mutex_;
};

class Widget2TypeCacheTimer : public MyUtil::Timer {
public:
  Widget2TypeCacheTimer() :
    Timer(5 * 60 * 1000) { //5分钟重新加载一次
  }
  virtual void handle() {
   MCE_DEBUG("Wiget2TypeCacheTimer begin");
   Widget2TypeCache::instance().Load();
     MCE_DEBUG("Wiget2TypeCacheTimer stop");
   }
};

////////////////////////////////////////////////////////////////////////////////////////
//EngineU 代码重构部分
///////////////////////////////////////////////////////////////////////////////////////

class AdSelectHandler : public IceUtil::Shared {
public:
  virtual void handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para) = 0 ;
public:
  //自助广告位竞价流程、包括70/72全竞价
  bool Bidding(SelectedAdGroupSeq& result,
		  set<AdGroupPtr>& groups, set<AdGroupPtr>& all_selected_groups_local, 
      MatchHandlerParamter& para, UserIndexSelectPara& select_para);
  //随机选择广告
  vector<AdGroupPtr> RandomSelectAdGroup(const UserProfile & user_profile,set<AdGroupPtr>& bid_gids,	
					                        set<AdGroupPtr>& all_selected_gids, const Ice::Long zone_id ,const int need_count); 
  //获取新广告
  AdGroupPtr GetZonePosFreshGroup(int userid, FreshAdQualityMap & fresh_groups, set<AdGroupPtr>& bid_ids, 
					                        set<AdGroupPtr>& selected_ids, long adzone);
public:
	//考虑是否与已选广告组构成竞争关系
  bool IsCompetedGroup(AdGroupPtr candidate_group, set<AdGroupPtr> & selected_ids);
	//比较两个double，
  int CompareDouble(double x, double y, double equal_ratio);
	//获取全竞价vip广告的加权
  double GetAllPkLambada(Ice::Long adzone);
	//过滤vip广告
  set<AdGroupPtr> FilterVipGroups(set<AdGroupPtr> & bid_gids,MatchHandlerParamter & para);
	//根据参数判断是否需要继续选择
	bool CheckNeedSelectMore(MatchHandlerParamter & para);
	//过滤并获取保量广告
	set<AdGroupPtr> FilterAndGetCpmGroups(set<AdGroupPtr>& candidate_group,MatchHandlerParamter & para);
};

class BiddingSelectHandle : public AdSelectHandler {
public:
  void handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para);
};

class RandomSelectHandle: public AdSelectHandler {
public:
  void handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para);
};

class AllPkSelectHandle : public AdSelectHandler {
public:
  void handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para);
};

class FreshAdSelectHandle : public AdSelectHandler {
public:
  void handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para);
};

class CpmMemberAdSelectHandle : public AdSelectHandler {
public:
  void handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para);
};

class RefreshSelectHandle: public AdSelectHandler {
public:
  void handle(set<AdGroupPtr>& bid_gids,MatchHandlerParamter& para);
};

/////////////////////////////////////////////////////////////////////////////////////
//重构部分结束
////////////////////////////////////////////////////////////////////////////////////
}
}

#endif // _USER_RANK_HANDLER_H_
