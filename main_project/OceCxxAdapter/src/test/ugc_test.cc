#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/UgcPhotoVoiceCountTripodAdapter.h"
#include "OceCxxAdapter/src/UgcVoiceCountTripodAdapter.h"
#include "LogWrapper.h"

namespace xce{
namespace ugc {



class socialgraphTripodAdapter : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
  static void TearDownTestCase() {
  }
};


TEST_F(socialgraphTripodAdapter, GETCount) {
  //SocialGraphRcdPageCfTripodAdapter::instance().Set("test1","aa bb",0);
  std::string val;
  bool re;
  re = UgcPhotoVoiceCountTripodAdapter::instance().Get("hellokey",&val);
  //re = SocialGraphRcdPageCfTripodAdapter::instance().Get("test1",&val);
  MCE_DEBUG(re);
  MCE_DEBUG(val);
  re = UgcVoiceCountTripodAdapter::instance().Get("hellokey",&val);
  //re = SocialGraphRcdPageCfTripodAdapter::instance().Get("test1",&val);
  MCE_DEBUG(re);
  MCE_DEBUG(val);
 
}



}} //xce::user::adapter::userstate
