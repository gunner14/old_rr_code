
#ifndef __AD_MOB_LOG_MAKER_H__
#define __AD_MOB_LOG_MAKER_H__

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>

#include "client/linux/handler/exception_handler.h"
#include "../../../util/src/AdCommonUtil.h"
#include "LogWrapper.h"

#include <queue>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Monitor.h>
#include <string>

#include "TaskManager.h"
#include "Util.h"

using namespace std;
using namespace MyUtil;

namespace xce{
namespace ad{

class AdMobLogMaker : public IceUtil::Shared , public IceUtil::Monitor<IceUtil::Mutex> {
public:
  AdMobLogMaker( const string& name, const string& path, const string& loglevel, const string pattern = "%m%n", const size_t buffer_size = 1): 
	  name_(name), path_(path), loglevel_(loglevel), pattern_(pattern), buffer_size_(buffer_size) {
    Initialize();
  }

  void Initialize() {
    log4cplus::SharedAppenderPtr append_1(new log4cplus::DailyRollingFileAppender(path_, log4cplus::MINUTELY, 1, true));
    append_1->setName(name_);
    append_1->setLayout( std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern_)) );
    log4cplus::Logger::getInstance(name_).addAppender(append_1);
    log4cplus::Logger::getInstance(name_).setLogLevel(log4cplus::LogLevelManager().fromString(loglevel_));

    TaskManager::instance().execute(new FlushLogTask(this)); 
  }

  void PushBack(const string& log);
  void PushBack(const StrSeq& logs);
  void Execute(); 
  
 
private:
  string name() {
    return name_;
  }
  void set_name(string name) {
    name_ = name;
  }

  class FlushLogTask : public MyUtil::Task {
  public:
    FlushLogTask(AdMobLogMaker* parent) {
      parent_ = parent;
    }
    virtual void handle();
  private:
    AdMobLogMaker* parent_;
  };

  queue<string> queue_;

  const static int WAIT_SECONDS = 10;
  string name_;
  string path_;
  string loglevel_;
  string pattern_;
  size_t buffer_size_;
};
typedef IceUtil::Handle<AdMobLogMaker> AdMobLogMakerPtr;


}
}
#endif
