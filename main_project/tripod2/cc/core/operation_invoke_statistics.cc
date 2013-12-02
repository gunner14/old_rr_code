#include <boost/thread.hpp>
#include "tripod2/cc/core/common.h"
#include "tripod2/cc/core/operation_invoke_statistics.h"
#include "tripod2/cc/core/remote_logger.h"

namespace xce {
namespace tripod {
 
const long OperationInvokeStatistics::kMaxCount;
const long OperationInvokeStatistics::kTimedLogInterval;

OperationInvokeStatistics::OperationInvokeStatistics(const std::string& namespace_id, const std::string& business_id,
    const std::string& op, StatisticsLogTask& task):
        namespace_id_(namespace_id), business_id_(business_id), operation_(op),task_(task) {
  count_ =0;
  last_invoke_time_ = 0L;
}

void OperationInvokeStatistics::IncCount(long n) {
  boost::lock_guard<boost::mutex> lock(mutex_);
  count_+=n;  
  time_t now;
  time (&now);
  if( (count_>=kMaxCount) || (difftime(now,last_invoke_time_)>=kTimedLogInterval) ) {
    last_invoke_time_ = now;
    task_.AddMessage(Dump(count_));
    count_ = 0;
  }
}

std::string OperationInvokeStatistics::Dump(long count) {
  ostringstream oss;
  oss << GetVersion() << " " << namespace_id_ << " " << business_id_ << " " << operation_ << " " << count;
  return oss.str();
}

StatisticsLogTask::StatisticsLogTask() : messages_(new std::deque<std::string>) {
  log_paths_.push_back("xce");
  log_paths_.push_back("tripod");
  log_paths_.push_back("tripod_statistics");
}

void StatisticsLogTask::AddMessage(const std::string& message) {
  boost::unique_lock<boost::mutex> lock(mutex_);
  messages_->push_back(message);
  if (messages_->size()>1000) {
    MCE_ERROR("StatisticsLogTask::AddMessage statistics log message queue full!" )
    messages_->clear();
  }
  cond_.notify_all();
}

void StatisticsLogTask::Run() {
  while (true) {
    if (!messages_) {
      MCE_INFO("deque_ is null, statistics thread(" << boost::this_thread::get_id() << ") shuts down right now");
      break;
    }
    boost::unique_lock<boost::mutex> lock(mutex_);
    if (messages_->size() == 0) {
      cond_.wait(lock);
    }
    else {
      std::string msg = messages_->front();
      messages_->pop_front();
      RemoteLoggerSingleton::instance().Log(msg,log_paths_);
    }
  }
}

void StatisticsLogTask::Stop() {
  MCE_INFO("StatisticsLogTask::Stop");
  {
    boost::unique_lock<boost::mutex> lock(mutex_);
    messages_->clear();
    messages_.reset();
  }
  cond_.notify_all();
}


}} // xce::tripod



