/*
 * DailyRemoveOverDueEdm.h
 *
 * module role : 每天删除前一天过期的推左edm
 * Created on: 2012-09-28
 * Author: ping.zhang
 */

#ifndef __DAILY_REMOVE_OVERDUE_EDM_H__
#define __DAILY_REMOVE_OVERDUE_EDM_H__

#include "Singleton.h"
#include <ctime>
#include <map>
#include "TaskManager.h"
#include <IceUtil/RWRecMutex.h>
#include <Ice/Ice.h>

namespace xce {
namespace ad {

using namespace  MyUtil;
using namespace  std;

class DailyRemoveOverDueEdmTimer : public MyUtil::Timer {
public:
  DailyRemoveOverDueEdmTimer() :
    Timer(59 * 60 * 1000) {
    }
  virtual void handle();
  bool RemoveEdm();
private:
  set<Ice::Long> overdue_campaigns_;

};

}
}
#endif
