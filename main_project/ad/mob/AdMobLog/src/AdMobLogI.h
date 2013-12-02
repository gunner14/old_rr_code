/*
 * AdMobLogI.h
 *
 *  Created on: 2012-4-17
 *      Author:yongjun.yin
 */

#ifndef __AD_MOB_LOG_I_H__
#define __AD_MOB_LOG_I_H__

#include "AdMobLog.h"
#include "ServiceI.h"
#include "Singleton.h"
#include "AdMobLogMaker.h"

namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;
using namespace com::xiaonei::xce;

class AdMobLogI: virtual public AdMobLog, public MyUtil::Singleton<AdMobLogI> {
public:
  void Initialize();
  
  virtual void Pv(const string& log, const Ice::Current& current);
  virtual void Click(const string& log, const Ice::Current& current);
  virtual void CpmCharge(const string& log, const Ice::Current& current);
  virtual void CpcCharge(const string& log, const Ice::Current& current);

  virtual void PvBatch(const StrSeq& logs, const Ice::Current& current);
  virtual void ClickBatch(const StrSeq& logs, const Ice::Current& current);
  virtual void CpmChargeBatch(const StrSeq& logs, const Ice::Current& current);
  virtual void CpcChargeBatch(const StrSeq& logs, const Ice::Current& current);
  void SetBufSize(size_t size){
    buf_size_ = size;
	}
private:
  AdMobLogMakerPtr pv_log_maker_;
  AdMobLogMakerPtr click_log_maker_;
  AdMobLogMakerPtr cpm_charge_log_maker_;
  AdMobLogMakerPtr cpc_charge_log_maker_;
  size_t buf_size_; //缓冲log条数
};

}
}
#endif
