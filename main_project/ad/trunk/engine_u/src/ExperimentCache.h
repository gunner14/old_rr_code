/*
 *  ExperimentCache.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description:
 *
 */

#ifndef EXPERIMENTCACHE_H_
#define EXPERIMENTCACHE_H_

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

using namespace xce::ad;
using namespace std;

namespace xce {
namespace ad {

class AdSelectZoneInfo;
typedef IceUtil::Handle<AdSelectZoneInfo> AdSelectZoneInfoPtr;


class CtrOnlineModelCache : public MyUtil::Singleton<CtrOnlineModelCache> {
public:
  CtrOnlineModelCache() {
    init_key_ = "0";
    DEF_FEATURE0 = "000000000000000";
    DEF_FEATURE9 = "999999999999999";
    init_weight_ = -10.0;
    adzone_list_.push_back(100000000001);
    adzone_list_.push_back(100000000070);
    adzone_list_.push_back(100000000072);
    adzone_list_.push_back(100000000063);
  }
 
  void FastLoad() {
    LoadAdCtrRealTimePool();
    LoadCtrOnlineModelPool();
    RefreshRatio();
  }
   
  void Load(){ 
    LoadAdClassListPool(); 
    LoadAdCtrFeaturePool(); 
  }
  
  void RefreshRatio(); 
  
  bool UseLrCtr(Ice::Long zone_id); 
  bool UseRealTimeCtr(Ice::Long zone_id); 
  
  void FillCommonFeatureData(hash_map<int, string>& feature_data, const MatchHandlerParamter& para, AdSelectZoneInfoPtr& info);
  void FillAdCtrFeatureData(hash_map<int, string>& feature_data, const AdGroupPtr& gptr); 
  
	pair<double, int> GetMergedCtr(const UserProfile & user_profile, long adzone, int pos, long group_id, pair<double,int> lr_ctr); 
  pair<double, int> GetCtr(hash_map<int, string>& common_feature_data, const AdGroupPtr& gptr, const double& common_weight); 
  double GetFeatureWeightByList(hash_map<int, string>& feature_data, AdCtrFeatureList& feature_list);  
  double GetTestFeatureWeightByList(hash_map<int, string>& feature_data, AdCtrFeatureList& feature_list);  
   
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
  hash_map<string, CtrOnlineModel> ctr_online_model_;
  hash_map<string, AdCtrRealTimeItem> ad_ctr_realtime_;
  AdClassListMap ad_class_;
  hash_map<Ice::Long, int> real_time_ratio_;
  hash_map<Ice::Long, int> lr_ctr_ratio_;
  AdCtrFeatureList all_feature_;
  AdCtrFeatureList common_feature_;
  AdCtrFeatureList ad_feature_;
  AdCtrFeatureList complex_feature_;
  vector<Ice::Long> adzone_list_;
  
  void MergeCtrFeatureData(hash_map<int, string>& feature_data, hash_map<int, string>& common_feature_data);
  string GetFeatureKey(AdCtrFeature& feature, hash_map<int, string>& feature_data); //构造feature
	string GetKeyByIndex(int gender, int stage, long adzone, int pos, long group_id, int index);
  double GetWeightByKey(const string& key);  
  AdCtrRealTimeItem GetRealTimeCtrByKey(const string& key);  
  AdClass GetAdClass(Ice::Long key); //获取分类信息
  void LoadCtrOnlineModelPool(); 
  void LoadAdClassListPool(); 
  void LoadAdCtrFeaturePool();
  void LoadAdCtrRealTimePool();
};

class CtrOnlineModelCacheFastTimer : public MyUtil::Timer {
public:
  CtrOnlineModelCacheFastTimer() :
    Timer(4 * 60 * 1000) { //4分钟重新Load一次 ， 暂定
  }
  virtual void handle() { 
    CtrOnlineModelCache::instance().FastLoad();
  }
};

class CtrOnlineModelCacheTimer : public MyUtil::Timer {
public:
  CtrOnlineModelCacheTimer() :
    Timer(15 * 60 * 1000) { //15分钟重新Load一次 ， 暂定
  }
  virtual void handle() { 
    CtrOnlineModelCache::instance().Load();
  }
};

class AdSelectZoneInfo : public IceUtil::Shared {
public:
  AdSelectZoneInfo(const string &app_name, const Ice::Long& adzone_id, const int& count){
    app_name_ = app_name;
    adzone_id_ = adzone_id;
    count_= count;
  }
  AdSelectZoneInfo(const Ice::Long& adzone_id, const int& count){
    adzone_id_ = adzone_id;
    count_= count;
  }
  AdSelectZoneInfo(){
    app_name_ = "";
    adzone_id_ = 0L;
    count_ = 0;
    pos_ = 0; 
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

private:
  string app_name_;
  Ice::Long adzone_id_;
  int count_; 
  int pos_;
};

}
}
#endif /* EXPERIMENTCACHE_H_ */
