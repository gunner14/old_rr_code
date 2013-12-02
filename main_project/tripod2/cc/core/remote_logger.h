#ifndef REMOTE_LOGGER_H_
#define REMOTE_LOGGER_H_

#include <string>
#include "UtilCxx/src/Singleton.h"
#include "OceCxxAdapter/src/XceLoggerReplicaAdapter.h"

namespace xce {
namespace tripod {

class RemoteLogger {
 public:
  void Log(const std::string& message,std::vector<std::string>& log_paths);
  virtual ~RemoteLogger() {}
 private:
  friend class MyUtil::Singleton<RemoteLogger>;
  RemoteLogger();
  std::string Format(const std::string& message); 
  static std::string GetTime();
  static std::string GetLocalIp(); 
  com::xiaonei::service::XceLoggerReplicaPtr xce_logger_;
  std::string ip_address_;
};

typedef MyUtil::Singleton<RemoteLogger> RemoteLoggerSingleton;
}} // xce::tripod
#endif



