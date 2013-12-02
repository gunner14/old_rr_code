#include "socialgraph/recommendcontent/PageModelCreator/RcdPageWriter/PageCacheTask.h"
#include "socialgraph/socialgraphutil/clogging.h"

using namespace xce::socialgraph;
void SetPageToCacheTask::handle() {
  boost::mutex::scoped_lock alock(mutex_);                //lock local circle
  if (!param_.empty()) {
    RcdPageCacheHelper::setRcdPageIntoCache(userid_, param_);
  }
  return;
}

