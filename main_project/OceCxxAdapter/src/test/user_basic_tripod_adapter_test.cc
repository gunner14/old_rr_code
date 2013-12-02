#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UserBasicTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userbasic {

TEST(UserBasicTripodAdapter, GetBasic) {
  int userId = 35178;
  try {
    UserBasicDataPtr info = UserBasicTripodAdapter::instance().GetBasic(userId);
    std::cout << info->id << endl;
    ASSERT_TRUE(info->id == 35178);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userright
