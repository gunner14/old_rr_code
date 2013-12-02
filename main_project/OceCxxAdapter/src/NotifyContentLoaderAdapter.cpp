#include "Singleton.h"
#include "AdapterI.h"
#include "TaskManager.h"
#include "Notify.h"
#include "NotifyContentLoaderAdapter.h"

using namespace std;
using namespace MyUtil;
using namespace xce::notify;

NotifyContentLoadManagerPrx NotifyContentLoaderAdapter::getManager(
    Ice::Long id) {
  return locate<NotifyContentLoadManagerPrx>(_managers, "M", id, TWO_WAY);
}

NotifyContentLoadManagerPrx NotifyContentLoaderAdapter::getManagerOneway(
    Ice::Long id){
  return locate<NotifyContentLoadManagerPrx> (_managersOneway, "M", id, 
      ONE_WAY);
}

NotifyContentDict NotifyContentLoaderAdapter::GetNotifyContentDict(
    const MyUtil::LongSeq& nids) {
  MCE_DEBUG("NotifyContentLoaderAdapter::GetNotifyContentDict nids " 
      << nids.size());
  if(nids.empty()) {
    return NotifyContentDict();
  } else {
    return  getManager(nids[0])->GetNotifyContentDict(nids);
  }
}

void NotifyContentLoaderAdapter::AddNotifyContent(
    const NotifyContentPtr& content,
    bool compressFlag) {
  getManagerOneway(content->id)->AddNotifyContent(content, compressFlag);
}

