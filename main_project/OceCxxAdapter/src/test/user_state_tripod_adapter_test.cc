#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserStateTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userstate {

TEST(UserStateTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserStateInfoPtr info = UserStateTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserStateTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserStateTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userstate
