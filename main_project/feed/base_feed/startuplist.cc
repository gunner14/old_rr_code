#include "feed/base_feed/startuplist.h"

#include <list>
#include <boost/foreach.hpp>

#include "feed/base_feed/common.h"
#include "feed/base_feed/logging.h"
#include "feed/base_feed/once.h"

namespace xce {

static std::list<StartupFunction>* startup_list_ = 0;
static bool called_ = false;

static void InitStartupList() {
  startup_list_ = new std::list<StartupFunction>();
}

void AddStartup(StartupFunction func, bool first) {
  static XCE_DECLARE_ONCE(once_guard_);
  XceOnceInit(&once_guard_, &InitStartupList);

  // 未加锁的，应该不需要
  ASSERT(func);
  if (first)
    startup_list_->push_front(func);
  else
    startup_list_->push_back(func);
}

void RunStartupList() {
  if (!startup_list_)
    return;

  // 一般执行多次应该是无害的
  ASSERT(!called_);
  called_ = true;

  LOG(INFO) << "StartupList count: " << startup_list_->size();
  int c = 0;
  BOOST_FOREACH(StartupFunction f, *startup_list_) {
    ++ c;
    f();
  }
  LOG(INFO) << "StartupList finished: " << c;
}

void DestoryStartupList() {
  delete startup_list_;
  startup_list_ = 0;
}

}
