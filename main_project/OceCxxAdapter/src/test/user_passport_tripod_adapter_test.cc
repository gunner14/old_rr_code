#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserPassportTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userpassport {

TEST(UserPassportTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserPassportInfoPtr info = UserPassportTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserPassportTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserPassportTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userpassport
