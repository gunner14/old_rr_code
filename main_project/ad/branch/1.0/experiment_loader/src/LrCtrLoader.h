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
  AdCtrRealTimeMap GetAdCtrRealTimeMap(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return real_time_ctr_online_model_;
  }
  CtrOnlineModelMap GetCtrOnlineModelMap(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return ctr_online_model_;
  } 
  AdClassListMap GetAdClassListMap(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return ad_class_;
  }
  AdCtrFeatureList GetAdCtrFeatureList(){
    IceUtil::RWRecMutex::RLock lock(mutex_);  
    return ad_ctr_feature_;
  }
  void LoadPool(){
    LoadAdCtrFeaturePool();
    LoadAdClassListPool();
    LoadCtrOnlineModelPool();
    LoadAdCtrRealTimePool();
  }

private:
  void LoadAdCtrRealTimePool();
  void LoadAdCtrFeaturePool();
  void LoadAdClassListPool();
  void LoadCtrOnlineModelPool();
	void CalcWeight(AdCtrRealTimeMap&); 
	void ExpandCtrByPos(AdCtrRealTimeMap&); 
  void AddAdCtrRealTimeItem(AdCtrRealTimeMap& temp_adzone_data, string& key, AdCtrRealTimeItem& item);
  void AddAdCtrRealTimeItemAdZoneDiscount(AdCtrRealTimeMap& temp_adzone_data, string key, AdCtrRealTimeItem& item);
  double CalcRealTimeCtr(AdCtrRealTimeItem &item);
  double CalcRealTimeWeight(AdCtrRealTimeItem &item, Ice::Long pv_th);
  string GetKeyByIndex(int gender, int stage, long adzone, int pos, long group_id, int index);
  
  AdCtrRealTimeMap real_time_ctr_online_model_;
  CtrOnlineModelMap ctr_online_model_;
  AdClassListMap ad_class_;
  AdCtrFeatureList ad_ctr_feature_;
  int hour_;
  IceUtil::RWRecMutex mutex_;
};

class LrCtrLoaderTimer : public MyUtil::Timer {
public:
  LrCtrLoaderTimer(int time = 0) : Timer(time) { 
  }
  virtual void handle() {
    MCE_INFO("LrCtrLoaderTimer:begin");
    LrCtrLoader::instance().LoadPool();
    MCE_INFO("LrCtrLoaderTimer:stop");
    MyUtil::TaskManager::instance().schedule(new LrCtrLoaderTimer(5 * 60 * 1000)); //5min load 一次数据，暂时的
  }
};

}
}

#endif /* EXPERIMENTLOADERCACHE_H_ */
