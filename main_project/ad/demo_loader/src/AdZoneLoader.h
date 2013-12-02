/********************************
*  广告位质量数据的读取和cache
*  jieyu.zeng
*  2012-12-12
********************************/
#ifndef ADZONELOADER_H
#define ADZONELOADER_H

#include "AdDemoQuality.h"
#include "AdConfig.h"
#include "IceLogger.h"

namespace xce{
namespace ad{

class AdZoneLoader: public MyUtil::Singleton<AdZoneLoader> {
public:
  AdZoneLoader():ready_(false){}
  AdZoneQualityMap GetAdZoneQualityPool(){
    AdZoneQualityMap result;
    if (ready_) {
      IceUtil::RWRecMutex::RLock lock(mutex_);
      result = quality_pool_;
    }
    return result;
  }
  void Load(){
    LoadAdZoneQualityPool();
  }

private:
  void LoadAdZoneQualityPool();  

  bool ready_;
  AdZoneQualityMap quality_pool_;
  IceUtil::RWRecMutex mutex_;
}; 


class AdZoneLoaderTimer : public MyUtil::Timer {
public:
  AdZoneLoaderTimer(int time = 10 * 60 * 1000) :Timer(time){}
  virtual void handle() {
    MCE_INFO("AdZoneLoaderTimer handle :begin");
    MyUtil::TaskManager::instance().schedule(new AdZoneLoaderTimer(30 * 60 * 1000)); //每30读一次
    AdZoneLoader::instance().Load();
  }

};


} /*  end of namespace ad */
} /*  end of namespace xce */

#endif /* ADZONELOADER_H */
