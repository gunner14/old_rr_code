/*
 *  Engine.h
 *
 *   Created on: 2012-3-30
 *       Author: yongjun.yin
 *  Description: 定义业务服务对外主接口 AdMobEngine
 */


#ifndef __AD_MOB_ENGINE_H__
#define __AD_MOB_ENGINE_H__

#include "AdMob.h"
#include "Handler.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "TaskManager.h"
#include "Singleton.h"

using namespace xce::ad;
namespace xce {
namespace ad {

class AdMobEngine: public AdMob, public MyUtil::Singleton<AdMobEngine> {
public:
  AdMobEngine();
  virtual AdMobResPara GetAds(const xce::ad::AdMobReqPara& para, const Ice::Current&);
  virtual AdMobResPara GetAdsForTest(const xce::ad::AdMobReqPara& para, const Ice::Current&);
  virtual void PvFeedBack(const xce::ad::AdMobFeedback& para, const Ice::Current&);

  void initOK();
private:
  bool init_ok_;
};


}
}
#endif
