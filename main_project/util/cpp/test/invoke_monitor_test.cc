#include "gtest/gtest.h"
#include "Ice/Ice.h"
#include "util/cpp/InvokeMonitor.h"
#include "UtilCxx/src/LogWrapper.h"
#include "boost/thread.hpp"
#include "OceCxxAdapter/src/MonitorAdapter.h"

namespace {
using namespace MyUtil;

class InvokeClientTest : public testing::Test {
 public:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
};

TEST_F(InvokeClientTest, report) {
//  InvokeClient ic = InvokeClient::create(Ice::Current(), "invoke_client_test", InvokeClient::INVOKE_LEVEL_INFO);
//  boost::this_thread::sleep(boost::posix_time::milliseconds(200));
  InvokeMonitor::instance().report("invoke_client_test_report", 100);
  try {
    monitor::MonitorDispatcherAdapter::instance().report("test_type", "test_message");
  } catch (const Ice::Exception& e) {
    MCE_ERROR(e.what());
  }
}


}

