/*
 *  ExperimentCache.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description:
 *
 */

#ifndef EXPERIMENTCACHE_H_
#define EXPERIMENTCACHE_H_

#include <boost/regex.hpp>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <Ice/Ice.h>
#include "Date.h"
#include "Singleton.h"
#include "FeedMemcProxy/client/user_profile_client.h"
#include "AdStruct.h"
#include "AdExperiment.h"
#include "MatchHandlers.h"
#include "IceLogger.h"
#include "MatchHandlers.h"

using namespace xce::ad;
using namespace std;

namespace xce {
namespace ad {

class AdSelectZoneInfo;
typedef IceUtil::Handle<AdSelectZoneInfo> AdSelectZoneInfoPtr;

class AdCtrFeatureData;
typedef IceUtil::Handle<AdCtrFeatureData> AdCtrFeatureDataPtr;
class AdCtrFeatureDataCache;

class CtrOnlineModelCache : public MyUtil::Singleton<CtrOnlineModelCache> {
public:
  CtrOnlineModelCache() {
    init_key_ = "0";
    DEF_FEATURE0 = "000000000000000";
    DEF_FEATURE9 = "999999999999999";
    model_md5sum_ = "init";
    init_weight_ = -10.0;
    InitReferPatternRegex();
    use_adgroup_member_industry_ = 0;
    need_refresh_discount_ = 0;
    adzone_list_.push_back(100000000001);
    adzone_list_.push_back(100000000060);
    adzone_list_.push_back(100000000063);
    adzone_list_.push_back(100000000065);
    adzone_list_.push_back(100000000069);
    adzone_list_.push_back(100000000070);
    adzone_list_.push_back(100000000071);
    adzone_list_.push_back(100000000072);
    adzone_list_.push_back(100000000073);
    adzone_list_.push_back(100000000074);
    adzone_list_.push_back(100000000075);
    adzone_list_.push_back(100000000093);
  }
 
  void FastLoad() {
    //LoadAdCtrRealTimePool(); 旧版实时模型目前不用
    LoadCtrOnlineModelPool();
    RefreshLrCtrParameters(); //从引擎参数中更新lr实验参数集合
  }
   
  void Load(){ 
    LoadAdClassListPool(); 
    LoadAdCtrFeaturePool(); 
    LoadAdMemberIndustryPool(); 
  }
  
  void RefreshLrCtrParameters(); 
  
  bool UseLrCtr(Ice::Long zone_id); 
  bool UseLrCtr(Ice::Long zone_id, Ice::Long uid); 
  bool UseRealTimeCtr(Ice::Long zone_id); 
  
  void FillCommonFeatureData(map<int, string>& feature_data, const MatchHandlerParamter& para, AdSelectZoneInfoPtr& info);
  void FillAdCtrFeatureData(map<int, string>& feature_data, const AdGroupPtr& gptr); 
  void ReassignPos(map<int, string>& feature_data, int pos);
  
  void AdjustCtr(pair<double, int>& ctr_state, const pair<double, int>& lr_state, const pair<double, int>& bayes_state, const UserIndexSelectPara& select_para, double lr_confidence);
	//pair<double, int> GetMergedCtr(const UserProfile & user_profile, long adzone, int pos, long group_id, pair<double,int> lr_ctr); 
  pair<double, int> GetCtr(map<int, string>& common_feature_data, const AdGroupPtr& gptr, const pair<double, double>& common_w_c, AdSelectZoneInfoPtr& info, double& confidence); 
  
  pair<double, double> GetFeatureWeightByList(map<int, string>& feature_data, AdCtrFeatureList& feature_list);  
   
  AdCtrFeatureList GetAllFeatureList(){
    IceUtil::RWRecMutex::RLock lock(mutex_);
    return all_feature_;
  }
  AdCtrFeatureList GetCommonFeatureList(){
    IceUtil::RWRecMutex::RLock lock(mutex_);
    return common_feature_;
  }
  AdCtrFeatureList GetAdFeatureList(){
    IceUtil::RWRecMutex::RLock lock(mutex_);
    return ad_feature_;
  }
  AdCtrFeatureList GetComplexFeatureList(){
    IceUtil::RWRecMutex::RLock lock(mutex_);
    return complex_feature_;
  }
  double GetInitWeight(){
    return init_weight_;
  }

private:
  IceUtil::RWRecMutex mutex_;
  double init_weight_; //0号特征权重
  string init_key_; //强制命中的特征, 0号特征
  string DEF_FEATURE0; // 0默认值
  string DEF_FEATURE9; // 9默认值
  string model_md5sum_;
  hash_map<string, CtrOnlineModel> ctr_online_model_;
  hash_map<string, AdCtrRealTimeItem> ad_ctr_realtime_;
  AdClassListMap ad_class_;
  AdMemberIndustryMap member_industry_;
  int use_adgroup_member_industry_;
  int need_refresh_discount_;
  AdRefreshCtrRatioMap refresh_ctr_ratio_;
  map<Ice::Long, int> real_time_ratio_;
  map<Ice::Long, int> lr_ctr_ratio_;
  AdCtrFeatureList all_feature_;
  AdCtrFeatureList common_feature_;
  AdCtrFeatureList ad_feature_;
  AdCtrFeatureList complex_feature_;
  vector<Ice::Long> adzone_list_;
  vector<boost::regex> refer_pattern_regex_;
  
  string GetFeatureKey(AdCtrFeature& feature, map<int, string>& feature_data); //构造feature
	//string GetKeyByIndex(int gender, int stage, long adzone, int pos, long group_id, int index);
  pair<double, double> GetWeightByKey(const string& key, const AdCtrFeature& feature);  
  int GetMemberIndustry(Ice::Long member_id);
  double GetRefreshCtrRatio(AdSelectZoneInfoPtr& info);
  int ReferHash(const string& refer, Ice::Long zone_id);
  void InitReferPatternRegex();
  //AdCtrRealTimeItem GetRealTimeCtrByKey(const string& key);  
  AdClass GetAdClass(Ice::Long key); //获取分类信息
  void LoadCtrOnlineModelPool(); 
  void LoadAdClassListPool(); 
  void LoadAdCtrFeaturePool();
  void LoadAdCtrRealTimePool();
  void LoadAdMemberIndustryPool();
};

class CtrOnlineModelCacheFastTimer : public MyUtil::Timer {
public:
  CtrOnlineModelCacheFastTimer() :
    Timer(2 * 60 * 1000) { //2分钟探测一次模型更新，如果有则更新(并同步更新折算比例);更新一次实验比例参数
  }
  virtual void handle() { 
    CtrOnlineModelCache::instance().FastLoad();
  }
};

class CtrOnlineModelCacheTimer : public MyUtil::Timer {
public:
  CtrOnlineModelCacheTimer() :
    Timer(11 * 60 * 1000) { //11分钟重新Load一次特征集配置/广告分类/广告商行业
  }
  virtual void handle() { 
    CtrOnlineModelCache::instance().Load();
  }
};

class AdSelectZoneInfo : public IceUtil::Shared {
public:
  AdSelectZoneInfo(const string& refresh_idx, const Ice::Long& adzone_id, const int& count){
    if (refresh_idx.empty()) {
      refresh_idx_ = "0";
    } else {
      refresh_idx_ = refresh_idx;
    }
    adzone_id_ = adzone_id;
    count_ = count;
  }
  AdSelectZoneInfo(){
    app_name_ = "";
    adzone_id_ = 0L;
    count_ = 0;
    pos_ = 0;
    refresh_idx_ = "0";
  }

  Ice::Long AdZoneId(){
    return adzone_id_;
  }
  int Count(){
    return count_; 
  }
  void SetPos(const int& pos){
    pos_ = pos;
  }
  int Pos(){
    return pos_;
  }
  string AppName(){
    return app_name_;
  }
  string RefreshIdx(){
    return refresh_idx_;
  }

private:
  string app_name_;
  string refresh_idx_;
  Ice::Long adzone_id_;
  int count_; 
  int pos_;
};

class AdCtrFeatureData : public IceUtil::Shared {
public :
  AdCtrFeatureData(){
    sum_weight_ = 0.0;
    sum_confidence_ = 0.0;
  }
  map<int, string> FeatureData(){    
    return feature_data_;
  }
  void SetFeatureData(map<int, string>& feature_data){
    feature_data_ = feature_data;
  }
  double SumWeight(){
    return sum_weight_;
  }
  void SetSumWeight(double sum_weight){
    sum_weight_ = sum_weight;
  }
  double SumConfidence(){
    return sum_confidence_;
  }
  void SetSumConfidence(double sum_confidence){
    sum_confidence_ = sum_confidence;
  }
  
private :
  double sum_weight_;
  double sum_confidence_;
  map<int, string> feature_data_;
};

class AdCtrFeatureDataCache : public MyUtil::Singleton<AdCtrFeatureDataCache> {
public :
  void Clear();
  AdCtrFeatureDataPtr Get(const AdGroupPtr& gptr);
  
private :
  void FillOneFeatureData(AdCtrFeatureDataPtr& gdata, const AdGroupPtr& gptr);

  IceUtil::RWRecMutex mutex_;
  hash_map<Ice::Long, AdCtrFeatureDataPtr> cache_; // 广告特征数据cache  
};

class AdExtraDataCache : public MyUtil::Singleton<AdExtraDataCache> {
public :
  void Load() {
    LoadAdExtraDataPool();
  }
  AdExtraData Get(const AdGroupPtr& gptr);

private :
  void LoadAdExtraDataPool();

  IceUtil::RWRecMutex mutex_;
  AdExtraDataMap ad_extra_data_; // extra date cache 
};

class AdExtraDataCacheTimer : public MyUtil::Timer {
public:
  AdExtraDataCacheTimer() :
    Timer(11 * 60 * 1000) { //11min 更新一次
  }
  virtual void handle() {
    AdExtraDataCache::instance().Load();
  }
};
}
}
#endif /* EXPERIMENTCACHE_H_ */
