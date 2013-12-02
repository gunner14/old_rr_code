
#ifndef _BASE_LOGGING_H_
#define _BASE_LOGGING_H_

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>
namespace base {

void InitLogger(const std::string& name, 
                const std::string& path, 
                const std::string& loglevel = "WARN",
                const std::string& pattern = "%-10D{%H:%M:%S.%q} : [%p] : %m%n");

#define LOGGER_NAME "default_logger"

#define MCE_INIT(path, level)  \
  base::InitLogger(LOGGER_NAME, path, level);

#define MCE_LEVEL(loglevel)  \
  do { \
    Logger::getInstance(LOGGER_NAME).setLogLevel(log4cplus::LogLevelManager().fromString(loglevel)); \
  } while(0);

#define MCE_TRACE(msg)  \
  do { \
    LOG4CPLUS_TRACE(log4cplus::Logger::getInstance(LOGGER_NAME), msg); \
  } while(0);

#define MCE_DEBUG(msg)  \
  do { \
    LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance(LOGGER_NAME), msg); \
  } while(0);

#define MCE_INFO(msg) \
  do { \
    LOG4CPLUS_INFO(log4cplus::Logger::getInstance(LOGGER_NAME), msg); \
  } while(0);

#define MCE_WARN(msg) \
  do { \
    LOG4CPLUS_WARN(log4cplus::Logger::getInstance(LOGGER_NAME), msg); \
  } while(0);

#define MCE_ERROR(msg) \
  do { \
    LOG4CPLUS_ERROR(log4cplus::Logger::getInstance(LOGGER_NAME), msg) ; \
  } while(0);

#define MCE_FATAL(msg) \
  do { \
    LOG4CPLUS_FATAL(log4cplus::Logger::getInstance(LOGGER_NAME), msg); \
  } while(0);
}

#endif // _BASE_LOGGING_H_

