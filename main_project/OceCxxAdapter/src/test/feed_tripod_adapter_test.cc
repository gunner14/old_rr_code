#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/FeedTripodAdapter.h"
#include "LogWrapper.h"

namespace xce{
namespace feed {

class FeedTripodAdapterTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
  static void TearDownTestCase() {
  }
};


TEST_F(FeedTripodAdapterTest, test) {
  FeedTripodAdapter::instance().Set("test1","aa bb",0);
  std::string val;
  bool re;
  re = FeedTripodAdapter::instance().Get("test1",&val);
  MCE_DEBUG(re);
  MCE_DEBUG(val);
  FeedTripodAdapter::instance().Set("test2","cc dd",0);
  StringListType keys;
  StringMapType values;
  keys.push_back("test1");
  keys.push_back("test2");
  std::map<std::string, bool> result = FeedTripodAdapter::instance().MultiGet(keys,&values);
  MCE_DEBUG(result["test1"]);
  MCE_DEBUG(result["test2"]);
  MCE_DEBUG(values["test1"]);
  MCE_DEBUG(values["test2"]);
}

}} 
