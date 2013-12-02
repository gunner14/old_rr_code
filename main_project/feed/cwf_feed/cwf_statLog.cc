#include "cwf_statLog.h"

namespace xce {

void CwfStatLog::initLogger() {
  _logger = com::xiaonei::service::XceLoggerReplicaAdapter::instance().getLogger();
  _logger->initialize(); 
}

void CwfStatLog::AddLog(const xce::log::StringSeq& categories, const std::string& message) {
  if(_logger) {
    try{
      _logger->addLog(categories, message);
    } catch (Ice::Exception& e) {
      MCE_WARN("CwfStatLog::AddLog --> message:" << message << "  err:" << e);
    }
  } else {
    initLogger();
  }
}

}


