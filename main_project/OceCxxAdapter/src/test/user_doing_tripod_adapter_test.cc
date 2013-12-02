#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserDoingTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userdoing {

TEST(UserDoingTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserDoingInfoPtr info = UserDoingTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserDoingTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserDoingTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userdoing
