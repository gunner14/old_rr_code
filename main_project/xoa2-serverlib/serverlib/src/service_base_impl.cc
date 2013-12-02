#include "service_base_impl.h"
#include "service_arch.h"
#include <iostream>

#include "concurrency/Mutex.h"

namespace xoa2 { namespace base {

using apache::thrift::concurrency::Guard;
using namespace std;


ServiceBaseImpl::ServiceBaseImpl(std::string name, std::string version) :
  name_(name),
  version_(version) {
  setStatus(RenrenBaseStatus::STARTING);
  alive_since_ = (int64_t) time(NULL);
  service_statistics_ = boost::shared_ptr<ServiceStatistics>(new ServiceStatistics);
  service_statistics_->init();
  setStatus(RenrenBaseStatus::ALIVE);
}

inline int64_t ServiceBaseImpl::aliveSince() {
  int64_t nowTime = (int64_t) time(NULL);
  return nowTime - alive_since_;
}      

void ServiceBaseImpl::reinitialize() {
  setStatus(RenrenBaseStatus::STARTING);
  service_statistics_->initService();
  alive_since_ = (int64_t) time(NULL);
  setStatus(RenrenBaseStatus::ALIVE);
}
void ServiceBaseImpl::shutdown() {
  // service_->Stop();
  if(registry_ != NULL) {
    registry_->destroy();
  }
  std::cout << "Stop() called, to stop service... "<< std::endl;
  server_->stop();
  exit(0);
}

void ServiceBaseImpl::startMilitiaService(const std::string& serviceId) { 
  cout<<"=============="<<g_Militia<<"=========="<<serviceId<<endl;
}

void ServiceBaseImpl::setStatus(RenrenBaseStatus::type status) {
  Guard g(service_status_lock_);
  service_status_ = status;
}

void ServiceBaseImpl::getStatusDetails(std::string& _return) {
  Guard g(service_status_lock_);
  switch(service_status_) {
    case RenrenBaseStatus::STOPPING:
      _return = "status is stopping!";
      break;
    case RenrenBaseStatus::ALIVE:
      _return = "status is alive!";
      break;
    case RenrenBaseStatus::DEAD:
      _return = "status is dead!";
      break;
    case RenrenBaseStatus::STOPPED:
      _return = "status is stopped!";
      break;
    case RenrenBaseStatus::WARNING:
      _return = "status is warning!";
      break;
    case RenrenBaseStatus::STARTING:
      _return = "status is starting!";
      break;
    default:
      MCE_WARN("Get Status Details error:" << "服务状态出现异常");
      _return = "status is error!";
      break;
  }
}

int64_t ServiceBaseImpl::getCounter(const std::string& key) {
  if(key == "") {
    MCE_WARN("ServiceBaseImpl::GetCounter() error:" << "传入的参数非法，请确认参数的正确性");
    return 0;
  } 
  int64_t rv = 0;
  rv = service_statistics_->getCounter(key);
  return rv;
}  

void ServiceBaseImpl::getCounters(std::map<std::string, int64_t>& _return) {
  service_statistics_->getCounters(_return);
}

void ServiceBaseImpl::getMean(std::string& _return, const std::string& key) {
  if(key == "") {
    MCE_WARN("ServiceBaseImpl::GetMean() error:" << "传入的参数非法，请确认参数的正确性");
    _return = "";
    return ;
  }
  _return = service_statistics_->getMean(key);
}

void ServiceBaseImpl::getMax(std::string& _return, const std::string& key, const int32_t secs) {
  if(secs < 0 || key == "") {
    MCE_WARN("ServiceBaseImpl:: GetMax() error:" << "传入的参数非法，请确认参数的正确性");
    _return = "";
    return ;
  }
  _return = service_statistics_->getMax(key, secs);
}

void ServiceBaseImpl::setCallMethodInfo(const std::string& name, const int64_t time) {
  if(time < 0 || name == "") {
    MCE_WARN("ServiceBaseImp:: setCallMethodInfo() error:" << "传入的参数非法，请确认参数的正确性");
    return ;
  }
  service_statistics_->setCallMethodInfo(name, time);  
}

void ServiceBaseImpl::getCpuProfile(std::string& _return, const int32_t secs) {
  if(secs < 0) {
    MCE_WARN("ServiceBaseImp:: getCpuProfile() error:" << "传入的参数非法，请确认参数的正确性");
    _return = "";
    return ;
  }
  std::cout<<"get cpu profile ..."<<std::endl;
  _return = "get cpu profile ...";
}


}}
