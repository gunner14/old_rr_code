#include "tripod2/cc/core/common.h"
#include "tripod2/cc/core/invoke_client.h"
#include "LogWrapper.h"

namespace xce {
namespace tripod {
 
InvokeClient::InvokeClient(): thread_(boost::bind(&StatisticsLogTask::Run, &task_)) {
}

 
InvokeClient::~InvokeClient() {
  MCE_INFO("InvokeClient::~InvokeClient()");
  Destroy();
  thread_.join();
}

void InvokeClient::Log(const std::string& op,const std::string& namespace_id,const std::string& biz_id, long count) {
  GetOperationInvokeStatistics(op,namespace_id,biz_id)->IncCount(count);
}

OperationInvokeStatisticsPtr InvokeClient::GetOperationInvokeStatistics(const std::string& op,const std::string& namespace_id,const std::string& biz_id) {
  std::string key = namespace_id + biz_id + op;
  MAP_TYPE::iterator iter; 
  if ( (iter=map_.find(key))  == map_.end() ) {
    boost::unique_lock<boost::mutex> lock(mutex_);
    if ( (iter=map_.find(key)) == map_.end() ) {
      map_.insert(make_pair(key,OperationInvokeStatisticsPtr(new OperationInvokeStatistics(namespace_id,biz_id,op,task_))));
    }
  }
  return map_[key];
}

InvokeClient* InvokeClient::instance_;
boost::mutex InvokeClient::singleton_mutex_;

void InvokeClient::Destroy() {
  MCE_INFO("InvokeClient::Destroy()");
  task_.Stop();
}

InvokeClient& InvokeClient::instance() {
  if (instance_ == NULL) {
    boost::unique_lock<boost::mutex> lock(singleton_mutex_);
    if (instance_ == NULL) {
      instance_ = new InvokeClient();
      // when helper destroys, instance_.Destroy will be invoked.
      static InvokeClientHelper helper(*instance_);
    }
  }
  return *instance_;
}

InvokeClientHelper::InvokeClientHelper(InvokeClient& handle) : handle_(handle) {
}

InvokeClientHelper::~InvokeClientHelper() {
  handle_.Destroy();
}

}} // xce::tripod



