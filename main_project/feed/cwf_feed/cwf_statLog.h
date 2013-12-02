#ifndef XCE_CWF_STATLOG_H__
#define XCE_CWF_STATLOG_H__

#include "XceLoggerReplicaAdapter.h"

namespace xce {

class CwfStatLog : public MyUtil::Singleton<CwfStatLog> {
public:
  void AddLog(const xce::log::StringSeq& categories, const std::string& message);
  void initLogger();

private:
  com::xiaonei::service::XceLoggerReplicaPtr _logger;
};

}
#endif

