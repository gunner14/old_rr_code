#ifndef _ADLOGMANAGER_H_
#define _ADLOGMANAGER_H_

#include "AdLog.h"
#include <iostream>
#include <string>
#include <queue>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Thread.h>
#include <Ice/Ice.h>
#include "TaskManager.h"
#include <QueryRunner.h>
#include "ServiceI.h"
#include "Singleton.h"
#include "Date.h"
#include "AdLogMaker.h"

namespace xce{
namespace ad{
using namespace std;
using namespace com::xiaonei::xce;
using namespace MyUtil;

class AdLogManagerI: virtual public AdLogManager,virtual public MyUtil::Singleton<AdLogManagerI> {
public:
	void Initialize();

  virtual void ClickForBp(const string& log, const Ice::Current& = Ice::Current());
  virtual void ClickForCharge(const string& log, const Ice::Current& = Ice::Current());
  
  /*
   *should use functions below unless you know what does them mean
   *
   */
  virtual void PvBySeq(const MyUtil::StrSeq& logs, const Ice::Current& = Ice::Current());
  virtual void Pv(const string& log, const Ice::Current& = Ice::Current());
  virtual Ice::Int UpdateOnceCount(const Ice::Int count, const Ice::Current& = Ice::Current());
  
  virtual void PvBatch(const ::MyUtil::StrSeq& logs, const Ice::Current& current);
  virtual void PvForCharge(const ::MyUtil::StrSeq& logs, const Ice::Current& = Ice::Current());
  virtual void EngineLog(const string& log, const Ice::Current& = Ice::Current());
  virtual void EngineLogBatch(const ::MyUtil::StrSeq& logs, const Ice::Current& current);

	void SetBufSize(size_t size){
	  buf_size_ = size;
	}

private:
  //AdLogMakerPtr pv_log_maker_;
  //AdLogMakerPtr pv_charge_log_maker_;
  AdLogMakerPtr click_log_maker_;
  AdLogMakerPtr cpc_charge_log_maker_;
  size_t buf_size_; //缓冲log条数
};

}
}
#endif
