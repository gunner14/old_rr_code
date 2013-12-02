/*
 *  ExperimentLoaderCache.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-01-05
 *  @Description: 实验数据cache 
 *
 */

#ifndef EXPERIMENTLOADERCACHE_H_
#define EXPERIMENTLOADERCACHE_H_

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

class ExperimentLoaderCache : public AdExperimentManager, public MyUtil::Singleton<ExperimentLoaderCache> {
public:
  virtual AdCtrRealTimeMap GetAdCtrRealTimePool(const ::Ice::Current& = ::Ice::Current());
  virtual AdCtrFeatureList GetAdCtrFeaturePool(const ::Ice::Current& = ::Ice::Current());
  virtual AdClassListMap GetAdClassListPool(const ::Ice::Current& = ::Ice::Current());
  virtual AdExtraDataMap GetAdExtraDataPool(const ::Ice::Current& = ::Ice::Current());
  virtual CtrOnlineModelMap GetCtrOnlineModelPoolById(int id, const ::Ice::Current& = ::Ice::Current());
  virtual AdRefreshCtrRatioMap GetAdRefreshCtrRatioPoolById(int id, const ::Ice::Current& = ::Ice::Current());
  virtual string GetModelMd5sumById(int id, const ::Ice::Current& = ::Ice::Current());
  virtual AdMemberIndustryMap GetAdMemberIndustryPool(const ::Ice::Current& = ::Ice::Current());

private:

};

}
}

#endif /* EXPERIMENTLOADERCACHE_H_ */
