/*
 *  LrCtrLoader.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-02-22
 *  @Description: lr 点击率实验数据Loader 
 *
 */

#ifndef LRCTR_LOADER_H_
#define LRCTR_LOADER_H_

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
#include "IceLogger.h"
#include "ServiceI.h"
#include "AdExperiment.h"
#include "Date.h"
#include "AdConfig.h"

namespace xce{
namespace ad{
using namespace com::xiaonei::xce;
using namespace std;
using namespace MyUtil;

class LrCtrLoader : public MyUtil::Singleton<LrCtrLoader> {
public:
  LrCtrLoader(){
    db_cnt_limit_ = 10;
    load_cnt_ = db_cnt_limit_;
  }
  AdCtrRealTimeMap GetAdCtrRealTimeMap(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return real_time_ctr_online_model_;
  }
  string GetModelMd5sum(int id);
  CtrOnlineModelMap GetCtrOnlineModelMap(int id);
  AdRefreshCtrRatioMap GetAdRefreshCtrRatioPool(int id); 
  
  AdClassListMap GetAdClassListMap(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return ad_class_;
  }
  AdExtraDataMap GetAdExtraDataMap(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return ad_extra_data_;
  }
  AdCtrFeatureList GetAdCtrFeatureList(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return ad_ctr_feature_;
  }
  AdMemberIndustryMap GetAdMemberIndustryPool() {
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return member_industry_; 
  }

  void LoadPool(){
    LoadCtrOnlineModelPools();
    if (load_cnt_ >= db_cnt_limit_) { //不给db太大压力,30s * 10 = 5min
      LoadAdCtrRealTimePool();
      LoadAdCtrFeaturePool();
      LoadAdClassListPool();
      LoadAdExtraDataPool();
      LoadAdMemberIndustryPool();
      load_cnt_ = 0;
    }
    ++load_cnt_;
  }

private:
  void LoadAdCtrRealTimePool();
  void LoadAdCtrFeaturePool();
  void LoadAdClassListPool();
  void LoadAdExtraDataPool();
  void LoadCtrOnlineModelPools();
  void LoadCtrOnlineModelPool(map<int, CtrOnlineModelMap>& model_pool, int id, string& md5new);
  void LoadAdRefreshCtrRatioPool(map<int, AdRefreshCtrRatioMap>& ctr_ratio_pool, int id);
  void LoadAdMemberIndustryPool(); 
  bool CheckModelFileUpdate(int model_id, string& md5new);
	void CalcWeight(AdCtrRealTimeMap&); 
	void ExpandCtrByPos(AdCtrRealTimeMap&); 
  void AddAdCtrRealTimeItem(AdCtrRealTimeMap& temp_adzone_data, string& key, AdCtrRealTimeItem& item);
  void AddAdCtrRealTimeItemAdZoneDiscount(AdCtrRealTimeMap& temp_adzone_data, string key, AdCtrRealTimeItem& item);
  double CalcRealTimeCtr(AdCtrRealTimeItem &item);
  double CalcRealTimeWeight(AdCtrRealTimeItem &item, Ice::Long pv_th);
  string GetKeyByIndex(int gender, int stage, long adzone, int pos, long group_id, int index);
  AdCtrRealTimeMap real_time_ctr_online_model_;
  map<int, CtrOnlineModelMap> ctr_online_model_pools_;
  map<int, AdRefreshCtrRatioMap> refresh_ctr_ratio_pools_;
  map<int, string> md5sum_pool_;
  AdClassListMap ad_class_;
  AdExtraDataMap ad_extra_data_;
  AdCtrFeatureList ad_ctr_feature_;
  AdMemberIndustryMap member_industry_;
  int hour_;
  int load_cnt_;
  int db_cnt_limit_;
  IceUtil::RWRecMutex mutex_;
};

class LrCtrLoaderTimer : public MyUtil::Timer {
public:
  LrCtrLoaderTimer(int time = 3 * 60 * 1000) : Timer(time) {//服务启动3min后才开始检测模型,进行重加载 
  }
  virtual void handle() {
    MCE_INFO("LrCtrLoaderTimer:begin");
    LrCtrLoader::instance().LoadPool();
    MCE_INFO("LrCtrLoaderTimer:stop");
    MyUtil::TaskManager::instance().schedule(new LrCtrLoaderTimer(30 * 1000)); //30s 检测一次数据
  }
};

}
}

#endif /* EXPERIMENTLOADERCACHE_H_ */
