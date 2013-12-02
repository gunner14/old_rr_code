#include "gtest/gtest.h"

#include "base/port.h"

#include "tripod2/cc/core/multibiz_tripod_client.h"

#include "LogWrapper.h"

namespace test {

using namespace xce::tripod;

class MultiBizTripodClientTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
    multibiz_tripod_client_ = new MultiBizTripodClient("xce_network");
  }
  static void TearDownTestCase() {
  }

  static MultiBizTripodClient* multibiz_tripod_client_;
  static int expire_time_;

};

MultiBizTripodClient* MultiBizTripodClientTest::multibiz_tripod_client_;
int MultiBizTripodClientTest::expire_time_ = 0;




TEST_F(MultiBizTripodClientTest, MultiBizGet) {
  while(1) {
  StringListType bizs;
  bizs.push_back("user_network_network");
  bizs.push_back("user_network_region");
  bizs.push_back("user_network_workplace");
  bizs.push_back("user_network_university");
  StringMapType values;
  std::map<std::string, bool> result = multibiz_tripod_client_->MultiBizGet("35178",bizs, &values,expire_time_);
  sleep(1);
  }
}



} // test



