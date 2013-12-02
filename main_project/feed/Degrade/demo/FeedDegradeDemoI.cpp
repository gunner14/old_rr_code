#include "FeedDegradeDemoI.h"

using namespace MyUtil;
using namespace xce::feed;

void MyUtil::initialize() {
    ServiceI& service = ServiceI::instance();
    service.getAdapter()->add(&FeedDegradeDemoI::instance(), service.createIdentity("M", ""));
    TaskManager::instance().scheduleRepeated(new RunTask());
}

FeedDegradeDemoI::FeedDegradeDemoI() : cnt_(0) {
    // 设置db源
    DegradeManager::instance().SetDBSource("feed_degrade");

    // 注册降级模块 
    DegradeManager::instance().Register("pull_mode");
    DegradeManager::instance().Register("play");

    // 然后初始化
    DegradeManager::instance().Init();
}

Ice::Int FeedDegradeDemoI::run(const Ice::Current&) {

    MCE_INFO("===========================================================");
    // 判断拉模式是否可用
    if (DegradeManager::instance().IsEnable("pull_mode")) {
        // 可用，执行拉模式
        MCE_INFO("this is pull mode...~_~!!");
    } else {
        // 不可用
        MCE_INFO("pull_mode disabled.");
    }

    // 判断可以play否
    if (DegradeManager::instance().IsEnable("play")) {
        // 可以
        play();
    } else {
        // 不可以play，好好学习
        MCE_INFO("good good study, day day up!");
    }

    return cnt_++;
}

void FeedDegradeDemoI::play() {
    MCE_INFO("[play start.......................................play end]");
}


void RunTask::handle() {
    FeedDegradeDemoI::instance().run();
}
