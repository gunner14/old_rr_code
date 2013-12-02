#ifndef XCE_FEEDVIEW_STATLOG_H__
#define XCE_FEEDVIEW_STATLOG_H__

#include "XceLoggerReplicaAdapter.h"

namespace xce {

class FeedViewStatLog : public MyUtil::Singleton<FeedViewStatLog> {
public:
  void AddLog(const xce::log::StringSeq& categories, const std::string& message);
  void initLogger();

private:
  com::xiaonei::service::XceLoggerReplicaPtr _logger;
};

}
#endif

