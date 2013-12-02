#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <string>
#include "boost/thread.hpp"
#include "UtilCxx/src/LogWrapper.h"
#include "OceCxxAdapter/src/XceLoggerAdapter.h"
#include "OceCxxAdapter/src/ActiveTrackAdapter.h"

namespace xce {
namespace log {

using namespace com::xiaonei::service;

class ActiveTrackTest : public testing::Test {
 protected:
  void SetUp() {
    MCE_DEFAULT_INIT("DEBUG");
    log_paths_.push_back("xce");
    log_paths_.push_back("buddy");
    std::string endpoint = "M@XceLogger";
    remote_logger_ = XceLoggerAdapter::instance().getLogger(endpoint);
    remote_logger_->initialize();
    ActiveTrack::instance().initialize();
  }
 
 protected:
  XceLoggerPtr remote_logger_;
  std::vector<std::string> log_paths_;
};


TEST_F(ActiveTrackTest, action_log) {
  std::string action_message = "LoadHashCache:286162347";
  for (int i = 0; i < 1500; ++i) {
   std::ostringstream otem;
   otem << action_message << ":" << i;
   remote_logger_->addLog(log_paths, otem.str());
  }
  sleep(10);
};

}} // xce::log

