#ifndef FRESHADTESTING_H_
#define FRESHADTESTING_H_

#include "AdQualityUtil.h"
#include "AdDemoQuality.h"
static const int duration_day = 10;
static const int pv_limit_count = 100000;

namespace xce{
namespace ad{
using namespace com::xiaonei::xce;
using namespace std;
using namespace MyUtil;
class FreshCreative: public IceUtil::Shared {
  private:
    Ice::Long creative_id_;
    Ice::Long group_id_;
    Ice::Long sponsor_id_;
 
    map<Ice::Long, map<string,pair<Ice::Long,Ice::Long> > > time_zone_pos_pv_click_; //key -->time
    map<string,pair<Ice::Long,Ice::Long> > zone_pos_pv_click_;
    Ice::Long create_time_;
    map<string,double> click_discount_ratio_table;
  public:
    FreshCreative(Ice::Long creative_id, Ice::Long group_id, Ice::Long sponsor_id):creative_id_(creative_id),group_id_(group_id),
    sponsor_id_(sponsor_id),create_time_(0){
      init();
    }
    void init();
    Ice::Long sponsor_id() {
      return sponsor_id_;
    }
    void set_create_time(Ice::Long time) {
      create_time_ = time;
    }
   void print_display_count();

};
typedef IceUtil::Handle<FreshCreative> FreshCreativePtr;

class FreshCreativeCache : public MyUtil::Singleton<FreshCreativeCache> {
private:
  FreshAdQualityMap fresh_creative_pool_;
  IceUtil::RWRecMutex mutex_;

  void AssignFreshCreative(FreshAdQualityMap & fresh_pool, map<Ice::Long, FreshCreativePtr>& creative_map);
  void LoadAuditedOnlineFreshAd(map<Ice::Long, FreshCreativePtr>& creative_map);
  void LoadFreshAdData(map<Ice::Long, FreshCreativePtr>& creative_map);
  void PrintDisplayCount(map<Ice::Long, FreshCreativePtr>& creative_map);
  
	void Init();
public:
  void LoadFreshCreativePool();
  void LoadFreshCreativeMap(FreshAdQualityMap& fresh_ad_pool);
};

class FreshCreativeCacheTimer : public MyUtil::Timer {
public:
  FreshCreativeCacheTimer(int time =0 ) :Timer(time){
  }
  virtual void handle() {
    MCE_INFO("FreshCreativeCacheTimer handle :begin");
    FreshCreativeCache::instance().LoadFreshCreativePool();
    MCE_INFO("FreshCreativeCacheTimer handle :stop");
    MyUtil::TaskManager::instance().schedule(new FreshCreativeCacheTimer(3 * 60 * 1000));
  }

};
}
}
#endif /* FRESHADTESTING_H_ */
