#include <boost/lexical_cast.hpp>
#include "ServiceI.h"
#include "Notify/util/Timestamp.h"
#include "Notify/loader/NotifyContentLoader.h"
#include "Notify/loader/NotifyContentLoadManager.h"

using namespace xce::notify;
using namespace com::xiaonei::xce;

using namespace MyUtil;

const int NotifyContentLoadManagerI::kDefaultTimeout = 200;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&NotifyContentLoadManagerI::instance(),
      service.createIdentity("M", ""));

  ThreadPoolConfig cfg(50, 100);
  TaskManager::instance().config(0, cfg);
}

NotifyContentLoadManagerI::NotifyContentLoadManagerI() {
}

NotifyContentLoadManagerI::~NotifyContentLoadManagerI() {
}
NotifyContentDict NotifyContentLoadManagerI::GetNotifyContentDict(
    const MyUtil::LongSeq& nids, const Ice::Current&) {
  return NotifyContentLoader::GetNotifyContentDict(nids);
}

void NotifyContentLoadManagerI::AddNotifyContent(
    const NotifyContentPtr& content, bool compressFlag, const Ice::Current&) {
  NotifyContentLoader::AddNotifyContent(content, compressFlag);
}

void NotifyContentLoadManagerI::RemoveNotifyContents(
    const LongSeq& nids, const Ice::Current&) {
  NotifyContentLoader::RemoveNotifyContents(nids);
}

/*
bool NotifyContentLoadManagerI::LoadContentFromDB(int64_t notify_id, 
    NotifyContentPtr& ntfy_cont_ptr) {
  LongSeq nids;
  nids.push_back(notify_id);
  NotifyContentDBLoader::instance().createNotifyContent(nids, 
      ntfy_cont_ptr);
}

void NotifyContentLoadManagerI::LoadContent(Ice::Long nid, const Ice::Current&) {
  MCE_INFO("NotifyContentLoadManagerI::LoadContent:nid[" << nid);
  load_task* = new LoadContentTask(nid, 
      boost::bind(&NotifyContentLoadManagerI::LoadContentFromDb2Memc, this, _1));
  TaskManager::instance().execute(load_task);
}


void LoadContentTask::handle() {
 load_func_(notify_id_);
}
*/

