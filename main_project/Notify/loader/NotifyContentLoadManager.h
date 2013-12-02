#ifndef _XCE_NOTIFY_CONTENT_LOAD_MANAGER_
#define _XCE_NOTIFY_CONTENT_LOAD_MANAGER_

#include <iostream>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include "Notify.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "QueryRunner.h"

namespace xce {
namespace notify {

class NotifyContentLoadManagerI : public NotifyContentLoadManager,
    public MyUtil::Singleton<NotifyContentLoadManagerI> {
 public:
  NotifyContentLoadManagerI();
  virtual ~NotifyContentLoadManagerI();

  virtual NotifyContentDict GetNotifyContentDict(const MyUtil::LongSeq& nids,
      const Ice::Current& = Ice::Current());
  virtual void AddNotifyContent(const NotifyContentPtr& content, 
      bool compressFlag, const Ice::Current& = Ice::Current());
  virtual void RemoveNotifyContents(const MyUtil::LongSeq& nids,
      const Ice::Current& = Ice::Current());
/*
  virtual MyUtil::Str2StrMap GetContentSeqByKey(const MyUtil::StrSeq& memc_keys,
      const Ice::Current& = Ice::Current());
  virtual NotifyContentDict GetContentDictTimeout(int uid, MyUtil::LongSeq nids,
      const Ice::Current& = Ice::Current(), int timeout);
  virtual void LoadContent(Ice::Long id, const Ice::Current& = Ice::Current());
  virtual void LoadContentSeq(const Ice::LongSeq& nids, 
      const Ice::Current& = Ice::Current());
  virtual void LoadContentByKey(const std::string& memc_key, 
      const Ice::Current& = Ice::Current());
  virtual void LoadContentSeqByKey(const MyUtil::StrSeq& memc_keys,
      const Ice::Current& = Ice::Current());
  // return  true if load succeed, return false otherwise
  bool LoadContentFromDB(int64_t notify_id, NotifyContentPtr);
  void LoadContentFromDb2Memc(int64_t notify_id);
  */
 private:
  static const int kDefaultTimeout;
  IceUtil::Mutex mutex_;
};
/*
class LoadNotifyContentTask : public MyUtil::Task {
 public:
  typedef boost::function<void (int64_t)> LoadFunc;
  LoadTask(int64_t nid, LoadFunc& load_func) : notify_id_(nid), 
      load_func_(load_func) {}
  virtual void handle();
 private:
  int64_t notify_id_;
  LoadFunc& load_func_;
};
*/
}
}
#endif // _XCE_NOTIFY_CONTENT_LOAD_MANAGER_

