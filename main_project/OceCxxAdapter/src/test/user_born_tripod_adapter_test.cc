#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserBornTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userborn {

TEST(UserBornTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserBornInfoPtr info = UserBornTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserBornTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserBornTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userborn
