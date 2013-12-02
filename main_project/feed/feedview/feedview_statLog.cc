#include "feedview_statLog.h"

namespace xce {

void FeedViewStatLog::initLogger() {
  _logger = com::xiaonei::service::XceLoggerReplicaAdapter::instance().getLogger();
  _logger->initialize(); 
}

void FeedViewStatLog::AddLog(const xce::log::StringSeq& categories, const std::string& message) {
  if(_logger) {
    try{
      _logger->addLog(categories, message);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedViewStatLog::AddLog --> message:" << message << "  err:" << e);
    }
  } else {
    initLogger();
  }
}

}


