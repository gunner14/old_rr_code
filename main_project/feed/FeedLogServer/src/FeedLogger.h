/*
 * FeedLogger.h
 *
 *  Created on: 2012-11-13
 *      Author:rui.qiu
 */
#ifndef __FEED_LOGGER_H__
#define __FEED_LOGGER_H__

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>

#include "client/linux/handler/exception_handler.h"
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
namespace feed{

class FeedLogger : public IceUtil::Shared , public IceUtil::Monitor<IceUtil::Mutex> {
public:
  FeedLogger( const string& name, const string& path, const string& loglevel, const string pattern = "%m%n", size_t buffer_size = 10, size_t wait_seconds = 10, int task_level = 0): 
	  name_(name), path_(path), loglevel_(loglevel), pattern_(pattern), buffer_size_(buffer_size), wait_seconds_(wait_seconds), task_level_(task_level) {
    Initialize();
  }

  void Initialize() {
    log4cplus::SharedAppenderPtr append_1(new log4cplus::DailyRollingFileAppender(path_, log4cplus::MINUTELY, 1, true));
    append_1->setName(name_);
    append_1->setLayout( std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern_)) );
    log4cplus::Logger::getInstance(name_).addAppender(append_1);
    log4cplus::Logger::getInstance(name_).setLogLevel(log4cplus::LogLevelManager().fromString(loglevel_));
    TaskManager::instance().execute(new FlushLogTask(this, task_level_)); 
  }

  //add a single log to buffer
  void PushBack(const string& log);
  //add several logs to buffer
  void PushBack(const StrSeq& logs);
  //write logs in buffer to local file
  void Execute(); 
  //flush buffer to local file
  void Flush();
  
  string name() {
    return name_;
  }
  void set_name(const string& name) {
    name_ = name;
  }
 
  string path() {
    return path_;
  }
  void set_path(const string& path) {
    path_ = path;
  }

  size_t wait_seconds() {
    return wait_seconds_;
  }
  void set_wait_seconds(size_t wait_seconds) {
    wait_seconds_ = wait_seconds;
  }
  
  size_t buffer_size() {
    return buffer_size_;
  }
  void set_buffer_size(size_t buffer_size) {
    buffer_size_ = buffer_size;
  }
 
  int task_level() {
    return task_level_;
  }
  void set_task_level(int task_level) {
    task_level_ = task_level;
  }

private:

  class FlushLogTask : public MyUtil::Task {
  public:
    FlushLogTask(FeedLogger* parent, int task_level): MyUtil::Task(task_level) {
      parent_ = parent;
    }
    virtual void handle();
  private:
    FeedLogger* parent_;
  };

  
  queue<string> queue_; // local log buffer

  string name_; // log name
  string path_; // local log file path
  string loglevel_; //log level, INFO as default
  string pattern_; //log pattern
  size_t buffer_size_; // local buffer size
  size_t wait_seconds_; //interval write to local file
  int task_level_; //logger task level in TaskManager
};

typedef IceUtil::Handle<FeedLogger> FeedLoggerPtr;

}
}
#endif
