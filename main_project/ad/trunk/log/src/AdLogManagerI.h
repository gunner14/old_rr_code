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
namespace xce{
namespace ad{
using namespace MyUtil;
using namespace std;
using namespace com::xiaonei::xce;

class AdLogManagerI: virtual public AdLogManager,virtual public MyUtil::Singleton<AdLogManagerI> {
public:
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

//  virtual void EngineClick(const string& log, const Ice::Current& = Ice::Current());
};


class PvLogHandler : public MyUtil::Singleton<PvLogHandler> {
public:
  PvLogHandler() :
    once_count_(30) {
    TaskManager::instance().execute(new PvLogHandlerTask());
  }
  void PushBack(string log) {
    IceUtil::Mutex::Lock lock(mutex_);
    queue_.push(log);
  }
  void PushBack(const MyUtil::StrSeq& logs);
  int once_count() const {
    return once_count_;
  }
  void set_once_count(int once_count) {
    this->once_count_ = once_count;
  }
private:
  bool Execute();

  queue<string> queue_;
  int once_count_;
  IceUtil::Mutex mutex_;

  class PvLogHandlerTask : virtual public MyUtil::Task {
  public:
    PvLogHandlerTask() {
    }
    virtual void handle() {
      while (true) {
        try {
          if (!PvLogHandler::instance().Execute()) {
            usleep(SLEEP_DURATION);
          }
        } catch (Ice::Exception& e) {
          MCE_WARN("PvLogHandlerTask --> handle Execute err:" << e << ", line:"
              << __LINE__);
        } catch (...) {
          MCE_WARN("PvLogHandlerTask --> handle Execute , line:" << __LINE__);
        }
      }
    }
  private:
    static const int SLEEP_DURATION = 200 * 1000; // 停顿时间
  };
};

class PvLogForChargeHandler : public MyUtil::Singleton<PvLogForChargeHandler> {
public:
  PvLogForChargeHandler() :
    once_count_(20) {
    TaskManager::instance().execute(new PvLogForChargeHandlerTask());
  }
  void PushBack(string log) {
    IceUtil::Mutex::Lock lock(mutex_);
    queue_.push(log);
  }
  void PushBack(const MyUtil::StrSeq& logs);
  int once_count() const {
    return once_count_;
  }
  void set_once_count(int once_count) {
    this->once_count_ = once_count;
  }
private:
  bool Execute();

  queue<string> queue_;
  int once_count_;
  IceUtil::Mutex mutex_;

  class PvLogForChargeHandlerTask : virtual public MyUtil::Task {
  public:
    PvLogForChargeHandlerTask() {
    }
    virtual void handle() {
      while (true) {
        try {
          if (!PvLogForChargeHandler::instance().Execute()) {
            usleep(SLEEP_DURATION);
          }
        } catch (Ice::Exception& e) {
          MCE_WARN("PvLogForChargeHandlerTask --> handle Execute err:" << e << ", line:"
              << __LINE__);
        } catch (...) {
          MCE_WARN("PvLogForChargeHandlerTask --> handle Execute , line:" << __LINE__);
        }
      }
    }
  private:
    static const int SLEEP_DURATION = 200 * 1000; // 停顿时间
  };
};


}
}
#endif
