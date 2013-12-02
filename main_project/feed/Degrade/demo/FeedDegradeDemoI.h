
#ifndef __FEED_DEGRADE_DEMO_H__
#define __FEED_DEGRADE_DEMO_H__

#include "ServiceI.h"
#include "Singleton.h"
#include "FeedDegradeManager.h"

// 引入降级组件
#include "feed/Degrade/src/Degrade.h"

namespace xce {
namespace feed {

using namespace std;
using namespace MyUtil;

class FeedDegradeDemoI : public FeedDegradeDemo, public MyUtil::Singleton<FeedDegradeDemoI> {
public:
    FeedDegradeDemoI();
    virtual Ice::Int run(const Ice::Current& = Ice::Current());
private:
    void play();
    int cnt_;
};

class RunTask: public Timer {
public:
    RunTask(): Timer(1000){}
    virtual ~RunTask() {}

    virtual void handle();
};

}
}
#endif

