#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserNameTripodAdapter.h"

namespace xce {
namespace adapter {
namespace username {

TEST(UserNameTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserNameInfoPtr info = UserNameTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserNameTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserNameTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::username
