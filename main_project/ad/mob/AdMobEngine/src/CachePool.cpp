/*
 * AdBaseCache.cpp
 *
 *  Created on: 2012-4-1
 *      Author: yongjun.yin
 */

#include "CachePool.h"

using namespace com::xiaonei::xce;
//using namespace xce::ad;
using namespace MyUtil;

namespace xce{
namespace ad{

void CachePool::Initialize(const string& DbInstance, const Ice::Long updateInterval) {
  this->init_ok_ = false;
  this->DbInstance_ = DbInstance;
  this->updateInterval_ = updateInterval;
  TotalLoad();
  init_ok_ = true;
  TaskManager::instance().execute(new ReloadTask(this, updateInterval_));
}

void CachePool::ReloadTask::handle() {
  try {
    while(true) {
      usleep(updateInterval_);
      parent_->IncLoad();
    }
  } catch (std::exception& e) {
    MCE_WARN("ReloadTask--> handle ERR  line:" << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("ReloadTask--> handle ERR  line:" << __LINE__);
  }
}


}
}
