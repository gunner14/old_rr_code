#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserLoginCountTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userlogincount {

TEST(UserLoginCountTripodAdapter, GET) {
  int userId = 35178;
  int count = 0;
  try {
    count = UserLoginCountTripodAdapter::instance().Get(userId);
    std::cout << "login_count:" << count << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  ASSERT_TRUE(count > 0);
}

TEST(UserLoginCountTripodAdapter, LOAD) {
  int userId = 35178;
  bool result = false;
  try {
    result = UserLoginCountTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  ASSERT_TRUE(result);
}

}}} //xce::user::adapter::userlogincount
