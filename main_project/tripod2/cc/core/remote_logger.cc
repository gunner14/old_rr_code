#include "tripod2/cc/core/remote_logger.h"
#include <time.h>
#include "base/network.h"

namespace xce {
namespace tripod {

RemoteLogger::RemoteLogger() {
  using namespace com::xiaonei::service;
  xce_logger_ = XceLoggerReplicaAdapter::instance().getLogger();
  xce_logger_->initialize();
  ip_address_ = GetLocalIp();
}
 
void RemoteLogger::Log(const std::string& message,std::vector<std::string>& log_paths) {
  if (!xce_logger_) {
    MCE_ERROR("XceLogger is not initialized!!");
  }
  else {
    xce_logger_->addLog(log_paths, Format(message));
  }
}

std::string RemoteLogger::Format(const std::string& message) {
  return GetTime() + " " + ip_address_ +" "+ message;
}

std::string RemoteLogger::GetTime() {
  time_t now;
  time (&now);
  struct tm * time_info = localtime ( &now );
  char time_char[9];
  strftime(time_char, sizeof(time_char), "%H:%M:%S", time_info);
  return std::string(time_char);
}

std::string RemoteLogger::GetLocalIp() {
  return xce::Network::local_ip();
}

}} // xce::tripod



