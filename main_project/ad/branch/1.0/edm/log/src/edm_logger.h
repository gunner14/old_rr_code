/*
 * edm_logger.h
 *
 *  Created on: 2012-2-23
 *      Author: ping.zhang
 */

#ifndef EDM_LOGGER_H_
#define EDM_LOGGER_H_

#include <Ice/Ice.h>
#include <IceUtil/Mutex.h>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include "LogWrapper.h"
#include "TaskManager.h"

namespace xce {
namespace ad {

inline void InitLogger(const std::string& name,
                      const std::string& path,
                      const std::string& loglevel = "WARN",
  //const std::string& pattern = "%L, %-10D{%H:%M:%S.%q} : [%p] : %m%n")
                      const std::string& pattern = "%m%n") {
  log4cplus::SharedAppenderPtr append_1(new log4cplus::DailyRollingFileAppender(path, log4cplus::MINUTELY, 1, true));
  append_1->setName(name);
  append_1->setLayout( std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)) );
  log4cplus::Logger::getInstance(name).addAppender(append_1);
  log4cplus::Logger::getInstance(name).setLogLevel(log4cplus::LogLevelManager().fromString(loglevel));
}

#define EDMSD_INFO(msg) LOG_INFO("edm_send", msg);
#define EDMCK_INFO(msg) LOG_INFO("edm_click", msg);
#define EDMEX_INFO(msg) LOG_INFO("edm_expose", msg);

class EdmLogTimer : public MyUtil::Timer {
public:
  EdmLogTimer() : Timer(43 * 1000) {}
  virtual void handle() {
    EDMSD_INFO("empty");
    EDMCK_INFO("empty");
    EDMEX_INFO("empty");
  }
};
}
}


#endif /* EDM_LOGGER_H_ */
