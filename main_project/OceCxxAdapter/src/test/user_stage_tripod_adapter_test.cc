#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserStageTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userstage {

TEST(UserStageTripodAdapter, GET) {
  int userId = 35178;
  try {
    UserStageInfoPtr info = UserStageTripodAdapter::instance().Get(userId);
    std::cout << info->to_string() << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserStageTripodAdapter, LOAD) {
  int userId = 35178;
  try {
    UserStageTripodAdapter::instance().Load(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userstage
