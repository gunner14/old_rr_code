#ifndef _UNIT_TEST_FEEDDISPATCHNEW_
#define _UNIT_TEST_FEEDDISPATCHNEW_
#include "RFeed.h"
#include"../FeedDispatchNewI.h"
#include "gtest/gtest.h"
namespace xce {
namespace feed {
  class FeedDispatchNewTest :public ::testing::Test{
    public:
      FeedDispatchNewTest() {
        
      }
      virtual ~FeedDispatchNewTest(){
      
      }
      virtual void SetUp();
      virtual void TearDown();
      FeedSeedPtr seed;
      FeedReplyPtr reply;
      FeedIndexSeq indexSeq;
  };

}
}
#endif
