#ifndef _NOTIFY_CONTENT_LOADER_ADAPTER_H_
#define _NOTIFY_CONTENT_LOADER_ADAPTER_H_

#include "AdapterI.h"
#include "Notify.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "TaskManager.h"

namespace xce {
namespace notify {

class NotifyContentLoaderAdapter: public MyUtil::AdapterI,
    public MyUtil::AdapterISingleton<MyUtil::NotifyChannel, NotifyContentLoaderAdapter> {
 public:
  NotifyContentLoaderAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }
  NotifyContentLoadManagerPrx getManager(Ice::Long id);
  NotifyContentLoadManagerPrx getManagerOneway(Ice::Long id);

  NotifyContentDict GetNotifyContentDict(const MyUtil::LongSeq& nids);
  void AddNotifyContent(const NotifyContentPtr& notify, bool compressFlag);

protected:
  virtual string name() {
    return "NotifyContentLoader";
  }
  virtual string endpoints() {
    return "@NotifyContentLoader";
  }
  virtual size_t cluster() {
    return 10;
  }

  vector<NotifyContentLoadManagerPrx> _managersOneway;
  vector<NotifyContentLoadManagerPrx> _managers;
};


}
}

#endif // _NOTIFY_CONTENT_LOADER_ADAPTER_H_
