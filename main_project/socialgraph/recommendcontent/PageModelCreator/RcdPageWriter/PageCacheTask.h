#include <vector>
#include <set>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>


#include "TaskManager.h"
#include "socialgraph/recommendcontent/PageModelCreator/RcdPageDbHelper/rcdpagedbhelper.h"
#include "socialgraph/recommendcontent/PageModelCreator/RcdPageCacheHelper/pagecachehelper.h"

namespace xce {
namespace socialgraph {

const int LEVEL_LOG_STATS = 5;

class SetPageToCacheTask : public MyUtil::Task {
public:
  SetPageToCacheTask(Ice::Int userid, std::set<int>& param, int level = LEVEL_LOG_STATS):
    Task(level), userid_(userid), param_(param) {
  }
  virtual void handle();
private:
  int userid_;
  std::set<int> param_;
  boost::mutex mutex_;
};
};
};

