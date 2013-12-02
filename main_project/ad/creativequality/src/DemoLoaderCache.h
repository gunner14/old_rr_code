/*
 * DemoLoader.h
 *
 *  Created on:May 10, 2011
 *      Author: kingsfield
 */

#ifndef DEMOLOADERCACHE_H_
#define DEMOLOADERCACHE_H_

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
//#include "FeedMemcProxy/client/user_profile_client.h"
#include "AdCreativeQuality.h"
#include "IceLogger.h"
#include "ServiceI.h"
#include "Date.h"
#include "client/linux/handler/exception_handler.h"
#include "AdConfig.h"
#include "Util.h"

namespace xce{
namespace ad{
using namespace com::xiaonei::xce;
using namespace std;
using namespace MyUtil;


const int no_gender = 1;
const int no_stage = 2;
const int spid_type = 3;
const int all_type = 4;
const int two_feature = 5;
 

class DemoLoaderCache :public AdCreativeQualityManager, public MyUtil::Singleton<DemoLoaderCache> {
public:
  virtual AdQualityMap GetDemoPool(const ::Ice::Current& = ::Ice::Current());
  virtual FreshAdQualityMap GetFreshAdMap(const ::Ice::Current& = ::Ice::Current());
  
  void LoadPool();
   
private:
  void DataValidityCheck(AdQualityMap& res_pool);
	void ShowAllCtr();
 
  
  string GetKey(Ice::Long zone_id, Ice::Long sp_id ,Ice::Long group_id,int pos , Ice::Long creative_id,  int gender,int stage){
    sp_id = sp_id * 10 + pos;
    group_id = group_id * 10 + pos;
    ostringstream sin;
    sin << zone_id <<"_"<< sp_id <<"_"<< group_id <<"_"  << creative_id << "_" <<gender<<"_"<<stage;
    return sin.str();
  };
  string GetKey(Ice::Long sp_id ,Ice::Long group_id,int gender,int stage){
    ostringstream sin;
    sin << sp_id <<"_"<< group_id <<"_" <<gender<<"_"<<stage;
    return sin.str();
  };
  
  string GetKey(string zone_id, Ice::Long sp_id ,Ice::Long group_id,int pos ,int gender,int stage){
    sp_id = sp_id * 10 + pos;
    group_id = group_id * 10 + pos;
    ostringstream sin;
    sin << zone_id <<"_"<< sp_id <<"_"<< group_id <<"_" <<gender<<"_"<<stage;
    return sin.str();
  };

  string GetKey(string zone_id, Ice::Long sp_id ,Ice::Long group_id,int pos, Ice::Long creative_id, int gender,int stage){
    sp_id = sp_id * 10 + pos;
    group_id = group_id * 10 + pos;
		creative_id = creative_id * 10 + pos;
    ostringstream sin;
    sin << zone_id <<"_"<< sp_id <<"_"<< group_id <<"_" << creative_id << "_" <<gender<<"_"<<stage;
    return sin.str();
  };

  string GetKey(Ice::Long zone_id, int pos) {
    Ice::Long key = zone_id * 10 + pos;
    ostringstream sin;
    sin << key;
    return sin.str();
  }
       
  bool IsCredible(int pv, int click, int type) {
    int pv_th = 100000;
    int click_th = 20;
    switch(type) {
       case no_gender:
         pv_th = AdDemoConfig::instance().GetNoGenderPvTh();
         click_th = AdDemoConfig::instance().GetNoGenderClickTh();
         break;
       case no_stage:
         pv_th = AdDemoConfig::instance().GetNoStagePvTh();
         click_th = AdDemoConfig::instance().GetNoStageClickTh();
         break;
       case spid_type:
         pv_th = AdDemoConfig::instance().GetSpidPvTh();
         click_th = AdDemoConfig::instance().GetSpidClickTh();
         break;
       case all_type:
         pv_th = AdDemoConfig::instance().GetAllPvTh();
         click_th = AdDemoConfig::instance().GetAllClickTh();
         break;
			 case two_feature:
         pv_th = AdDemoConfig::instance().GetTwoFeaturePvTh();
         click_th = AdDemoConfig::instance().GetTwoFeatureClickTh();
				 break;
        default:
          MCE_WARN("IsCredblie,no type:" << type);
          pv_th = 100000;
          click_th = 20;
    }
    return  (pv > pv_th) || ( pv > 0 && click > click_th);
  }

  AdQualityMap demo_pool;
  //Ice::Long hour_impr;
  static const size_t DAYCOUNT = 10;
  static const size_t HOURCOUNT = 24;
  static const size_t TIMEINTERVAL = 1 * 60 * 60;
  static const int NO_USE = -1;
  IceUtil::RWRecMutex mutex_;
};

class DemoLoaderCacheTimer : public MyUtil::Timer {
public:
  DemoLoaderCacheTimer(int time = 1 * 60 * 1000) :
    Timer(time) { 
  }
  virtual void handle() {
    MCE_INFO("DemoLoaderCacheTimer:begin");
    DemoLoaderCache::instance().LoadPool();
    MCE_INFO("DemoLoaderCacheTimer:stop");
    MyUtil::TaskManager::instance().schedule(new DemoLoaderCacheTimer(1 * 60 * 1000));
  }
};
}
}

#endif /* DEMOLOADERCACHE_H_ */
