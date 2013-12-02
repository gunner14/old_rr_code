#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserBaseTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userbase {

TEST(UserBaseTripodAdapter, GetCommon) {
  int userId = 35178;
  try {
    UserBaseCommonViewPtr info = UserBaseTripodAdapter::instance().GetCommon(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserBaseTripodAdapter, GetFull) {
  int userId = 35178;
  try {
    UserBaseFullViewPtr info = UserBaseTripodAdapter::instance().GetFull(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userright
