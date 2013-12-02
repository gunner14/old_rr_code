/*
 * EdmDiscountCache.h
 *
 * module role :通过member_id获取折扣
 * Created on: 2012-8-8
 * Author: 
 *
 */
#ifndef _AD_EDM_DISCOUNT_CACHE_H_
#define _AD_EDM_DISCOUNT_CACHE_H_

#include <string>
#include <map>
#include <IceUtil/RWRecMutex.h>
#include "Singleton.h"
#include "TaskManager.h"
#include "IceLogger.h"
#include "LogWrapper.h"

namespace xce {
namespace ad {

using namespace MyUtil;

class EdmDiscountCache : public MyUtil::Singleton<EdmDiscountCache> {
public:
  void LoadEdmDiscount();

  double GetDiscountRatio(Ice::Long member_id){
    IceUtil::RWRecMutex::RLock lock(mutex_);
    map<Ice::Long, double>::iterator itdis = discount_map_.find(member_id);
    if(itdis != discount_map_.end()) {
	  return itdis->second;
	} 
    return 1.0;
  }

private:
  map<Ice::Long,double> discount_map_;
  IceUtil::RWRecMutex mutex_;
};

//timer
class EdmDiscountCacheTimer : public MyUtil::Timer {
  public:
  EdmDiscountCacheTimer() : Timer(1 * 60 * 1000) { //5分钟重新计算一次
  }   
  virtual void handle() {
    MCE_DEBUG("DiscountCacheTimer begin");
	EdmDiscountCache::instance().LoadEdmDiscount();
	MCE_DEBUG("DiscountCacheTimer stop");
  }
};

}
}
#endif
