#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/BuddyRelationTripodAdapter.h"

namespace xce {
namespace buddy {
namespace adapter {

TEST(BuddyRelationTripodAdapter, LoadHashCache) {
  int test_id = 286162347;
  try {
    bool ret = BuddyRelationTripodAdapter::instance().LoadHashCache(test_id);
    ASSERT_TRUE(ret);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::buddy::adapter



