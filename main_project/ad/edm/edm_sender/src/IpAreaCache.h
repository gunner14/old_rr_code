/*
 * IpAreaCache.h
 *
 * module role :通过ip获取地域
 * Created on: 2012-2-26
 * Author: shuangshuang
 *
 */
 
#ifndef _AD_EDM_IP_AREA_CACHE_H_
#define _AD_EDM_IP_AREA_CACHE_H_

#include <string>
#include <map>
#include <IceUtil/RWRecMutex.h>
#include "Singleton.h"
#include "TaskManager.h"
#include "RangeKey.h"

namespace xce {
namespace ad {

using namespace  MyUtil;

class IpAreaCache : public MyUtil::Singleton<IpAreaCache> {
public:
  IpAreaCache();
  ~IpAreaCache();
  bool Reload();
  string GetIpArea(unsigned int ip) const;
  string GetIpArea(const string & ip) const;
private:
  mutable IceUtil::RWRecMutex mutex_;
  typedef map<RangeKey<unsigned int>, string> CacheMap;
  CacheMap cache_;
  string default_area_;
  bool ready_;
};

class IpAreaCacheReloadTask : public Task {
public:
  IpAreaCacheReloadTask () {}
  virtual void handle();
};

}
}

#endif 

