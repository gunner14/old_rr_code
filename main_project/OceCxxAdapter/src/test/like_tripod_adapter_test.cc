#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/i_like_it_cache_tripod_adapter.h"
#include "LogWrapper.h"

namespace xce{
namespace ilikeit {



class LikeTripodAdapter : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
  static void TearDownTestCase() {
  }
};


TEST_F(LikeTripodAdapter, GETCount) {
  int userId = 35178;
  try {
    int info = ILikeItCacheTripodAdapter::instance().getLikeCount(2,2519243394);
    MCE_DEBUG( info );
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST_F(LikeTripodAdapter, GETCACHE) {
  int userId = 35178;
  try {
    MyUtil::IntSeq aaa = ILikeItCacheTripodAdapter::instance().getLikeCache(2,2519243394);
    vector<int>::const_iterator it = aaa.begin();
    while(it != aaa.end()) {
      MCE_DEBUG( *it++ );
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}} //xce::user::adapter::userstate
