/*
 *  InvertedIndex.h
 *
 *   Created on: 2012-3-30
 *       Author: yongjun.yin
 *  Description: 定义系统数据监控类 AdMobMonitorI，用于实时查看引擎数据
 */

#ifndef __AD_MOB_MONITOR_H__
#define __AD_MOB_MONITOR_H__

#include <vector>
#include "AdMob.h"
#include "CachePool.h"

using namespace xce::ad;
namespace xce {
namespace ad {


class AdMobMonitorI: public AdMobMonitor, public MyUtil::Singleton<AdMobMonitorI> {
public:
  AdMobMonitorI();
  virtual string GetPoolValue(const int pool, const long id, const Ice::Current&);
  virtual int GetPoolSize(const int pool, const Ice::Current&);
  virtual string GetPoolAll(const int pool, const int size, const Ice::Current&);

  virtual string GetBrandCandidateGroups(const xce::ad::AdMobReqPara& para, const int index_flag, const Ice::Current&);
  virtual string GetSelfCandidateGroups(const xce::ad::AdMobReqPara& para, const int index_flag, const Ice::Current&);
  
  virtual string GetRotateGroups(const Ice::Long zone_id, const Ice::Current&);

  virtual AdMobTargetOutputSeq GetTargetGroups(const xce::ad::AdMobTargetInput& input, const Ice::Current&);

  virtual string GetBrandIndexInfo(const int index, const Ice::Long zone, const Ice::Current&);
  virtual string GetSelfIndexInfo(const int index, const Ice::Long zone, const Ice::Current&);

private:
  void TransToPara(const xce::ad::AdMobTargetInput& input, AdMobReqPara& para);
  map<int, CachePool*> cache_dict_;
};

}
}
#endif
