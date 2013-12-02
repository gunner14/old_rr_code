#ifndef OPERATION_INOVKE_STATISTICS_H_
#define OPERATION_INVOKE_STATISTICS_H_

#include <string>
#include <boost/utility.hpp>

namespace xce {
namespace tripod {

class StatisticsLogTask;
class OperationInvokeStatistics : private boost::noncopyable {
 public:
  OperationInvokeStatistics(const std::string& namespace_id, const std::string& business_id,
      const std::string& op, StatisticsLogTask& task);
  void IncCount(long n = 1);
 private:
  std::string Dump(long count);
  unsigned long count_;
  time_t last_invoke_time_;
  std::string namespace_id_;
  std::string business_id_;
  std::string operation_;
  boost::mutex mutex_;
  StatisticsLogTask& task_;
  static const long kTimedLogInterval = 10*60 ;//seconds
  static const long kMaxCount = 1000000;
};

typedef boost::shared_ptr<OperationInvokeStatistics> OperationInvokeStatisticsPtr;

class StatisticsLogTask :private boost::noncopyable {
 public:
  StatisticsLogTask();
  void AddMessage(const std::string& message);
  void Run();
  void Stop();
 private:
  boost::mutex mutex_;
  boost::shared_ptr<std::deque<std::string> > messages_;
  boost::condition_variable cond_;
  std::vector<std::string> log_paths_; 
};

}} // xce::tripod
#endif



