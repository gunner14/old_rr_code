#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserRightTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userright {

TEST(UserRightTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserRightInfoPtr info = UserRightTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserRightTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserRightTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userright
