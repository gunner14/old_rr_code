#ifndef _AD_CREATIVE_CACHE_H_
#define _AD_CREATIVE_CACHE_H_

#include <ctemplate/template.h>
#include <string>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Shared.h>
//#include <netdb.h>
//#include "Util.h"
#include "LogWrapper.h"
#include "TaskManager.h"
#include "AdEngine.h"

namespace xce {
namespace ad {

using namespace  MyUtil;

class AdCreative : public IceUtil::Shared {
public:
  long id;
  long zone_id;
  long group_id;
  long widget_id;
  string widget;
  string widget_version;
  long member_id;
  int type;
  time_t update_time;
  string name;

  string title;
  string content;
  string img_url;
  string dest_url;
};

typedef IceUtil::Handle<AdCreative> AdCreativePtr;


class AdCreativeCache : public MyUtil::Singleton<AdCreativeCache> {
public:
  AdCreativeCache();
  ~AdCreativeCache();
  bool Reload();
  bool ReloadById(long id);
  bool LoadTemplate();
  AdCreativePtr GetCreativeidByGroupid(long group_id) const;
  void GetJson(const AdZone2Creatives  & ads, string * html) const;
  void GetJsonS(const AdZone2Creatives  & ads, string * html) const;
  void GetAdsByZoneInfo(const set<Ice::Long> & zoneids, AdZone2Creatives & ads) const;
private:
  mutable IceUtil::RWRecMutex mutex_;
  mutable IceUtil::RWRecMutex group_mutex_;

  typedef map<long, AdCreativePtr> AdCache; // 以广告创意id为key
  AdCache ad_cache_;
  //各广告位广告，adzone_ key 为 zone id, value 为广告列表
  map<Ice::Long, set<Ice::Long> > adzone_adgroup_map_;
  // 各个广告位对应的默认广告, key 为 zone id, value 为默认广告列表
  map<long, vector<long> > default_ads_;
  mutable IceUtil::RWRecMutex tpl_s_mutex_;
  mutable IceUtil::RWRecMutex tpl_n_mutex_;
  ctemplate::Template * json_gaten_template_;
  ctemplate::Template * json_gates_template_;
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
      Timer(60 * 60 * 1000) { //60分钟重新加载一次
      }
    virtual void handle() {
      MCE_DEBUG("Wiget2TypeCacheTimer begin");
      Widget2TypeCache::instance().Load();
      MCE_DEBUG("Wiget2TypeCacheTimer stop");
    }   
};
}
}

#endif // _AD_CREATIVE_CACHE_H_

