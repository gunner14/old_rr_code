#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "LogWrapper.h"

namespace xce{
namespace socialgraph {



class socialgraphTripodAdapter : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
  static void TearDownTestCase() {
  }
};


TEST_F(socialgraphTripodAdapter, GETCount) {
  SocialGraphRcdTripodAdapter::instance().Set("test1","aa bb",0);
  std::string val;
  bool re;
  re = SocialGraphRcdTripodAdapter::instance().Get("test1",&val);
  MCE_DEBUG(re);
  MCE_DEBUG(val);
  SocialGraphRcdTripodAdapter::instance().Set("test2","cc dd",0);
  StringListType keys;
  StringMapType values;
  keys.push_back("test1");
  keys.push_back("test2");
  std::map<std::string, bool> result = SocialGraphRcdTripodAdapter::instance().MultiGet(keys,&values);
  MCE_DEBUG(result["test1"]);
  MCE_DEBUG(result["test2"]);
  MCE_DEBUG(values["test1"]);
  MCE_DEBUG(values["test2"]);
  
}



}} //xce::user::adapter::userstate
