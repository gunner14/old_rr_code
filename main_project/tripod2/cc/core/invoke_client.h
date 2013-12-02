#ifndef INVOKE_CLIENT_H_
#define INVOKE_CLIENT_H_

#include <string>
#include <map>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include "tripod2/cc/core/operation_invoke_statistics.h"

namespace xce {
namespace tripod {

class InvokeClient {
 public:
  virtual ~InvokeClient(); 

  //
  void Destroy();
  static InvokeClient& instance();

  void Log(const std::string& op,const std::string& namespace_id,const std::string& biz_id, long count = 1);
  OperationInvokeStatisticsPtr GetOperationInvokeStatistics(const std::string& op,const std::string& namespace_id,const std::string& biz_id);
 private:
  InvokeClient();
  typedef std::map<std::string, OperationInvokeStatisticsPtr> MAP_TYPE; 
  MAP_TYPE map_; 
  boost::mutex mutex_;
  StatisticsLogTask task_; 
  boost::thread thread_;

  //
  static InvokeClient* instance_;
  static boost::mutex singleton_mutex_;
};

typedef InvokeClient InvokeClientSingleton;

// help to call InvokeClient's Destroy() when process exits.
class InvokeClientHelper {
public:
  InvokeClientHelper(InvokeClient& handle);
  ~InvokeClientHelper();

private:
  InvokeClient& handle_;
};

}} // xce::tripod
#endif



