#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userurl {

TEST(UserUrlTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserUrlInfoPtr info = UserUrlTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserUrlTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserUrlTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userurl
