#ifndef _ADLOGMANAGER_H_
#define _ADLOGMANAGER_H_

#include <iostream>
#include <string>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Thread.h>
#include <Ice/Ice.h>
#include <QueryRunner.h>
#include "Util.h"
#include "AdLog.h"
#include "ServiceI.h"
#include "Singleton.h"
#include "Date.h"
#include <queue>
#include "AdLogMaker.h"

namespace xce{
namespace ad{
using namespace MyUtil;
using namespace std;
using namespace com::xiaonei::xce;

class AdLogManagerI: virtual public AdLogManager,virtual public MyUtil::Singleton<AdLogManagerI> {
public:
	void Initialize();
  virtual void Pv(const string& log, const Ice::Current& = Ice::Current());
  virtual void PvBatch(const ::MyUtil::StrSeq& logs, const Ice::Current& current);
  virtual void PvBySeq(const ::MyUtil::StrSeq& logs, const Ice::Current& current){
    return;
  }

  virtual int UpdateOnceCount(Ice::Int uid , const Ice::Current& current){
    return uid;
  }
  virtual void ClickForCharge(const string& log, const Ice::Current& = Ice::Current());
  virtual void PvForCharge(const ::MyUtil::StrSeq& logs, const Ice::Current& = Ice::Current());

  virtual void ClickForBp(const string& log, const Ice::Current& = Ice::Current());

  virtual void EngineLog(const string& log, const Ice::Current& = Ice::Current());
  virtual void EngineLogBatch(const ::MyUtil::StrSeq& logs, const Ice::Current& current);
  void SetBufSize(size_t size) {
	  buf_size_ = size;
	}
private:
	AdLogMakerPtr pv_log_maker_;
	AdLogMakerPtr click_log_maker_;
	AdLogMakerPtr ad_charge_maker_;
	AdLogMakerPtr pv_charge_log_maker_;
	AdLogMakerPtr ad_stat_log_maker_;
  size_t buf_size_;
};

}
}
#endif
