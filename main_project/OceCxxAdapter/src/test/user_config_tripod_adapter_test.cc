#include "gtest/gtest.h"
#include <iostream>
#include "../UserConfigTripodAdapter.h"

namespace xce {
namespace adapter {

TEST(UserConfigTripodAdapter, getUserConfig) {
  int userId =349440824;
  try {
    UserConfigInfoPtr info = UserConfigTripodAdapter::instance().getUserConfig(userId);
    info->print();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserConfigTripodAdapter, reload) {
  int userId =349440824;
  try {
    UserConfigTripodAdapter::instance().reload(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}} //xce::user::adapter



