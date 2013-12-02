#include "../share/TripodSimpleAdapterTest.h"

#include "TripodUserPassportAdapter.h"
#include "TripodUserNameAdapter.h"
#include "TripodUserRightAdapter.h"
#include "TripodUserStateAdapter.h"
#include "TripodUserLoginCountAdapter.h"
#include "TripodUserBornAdapter.h"
#include "TripodUserStageAdapter.h"
#include "TripodUserDoingAdapter.h"
#include "TripodUserUrlAdapter.h"

using namespace xce::adapter;
using namespace xce::tripoddata::userbase;

using testing::Types;

typedef Types< 
          TS< TripodUserPassportAdapter, PbUserPassport > ,
          TS< TripodUserNameAdapter, PbUserName > ,
          TS< TripodUserRightAdapter, PbUserRight > ,
          TS< TripodUserStateAdapter, PbUserState > ,
          TS< TripodUserLoginCountAdapter, PbUserLoginCount > ,
          TS< TripodUserBornAdapter, PbUserBorn > ,
          TS< TripodUserStageAdapter, PbUserStage > ,
          TS< TripodUserDoingAdapter, PbUserDoing > ,
          TS< TripodUserUrlAdapter, PbUserUrl >
          > AdapterImplementations;

INSTANTIATE_TYPED_TEST_CASE_P(TripodUserBaseAdapterTest, TripodSimpleAdapterTest, AdapterImplementations);

int main(int argc, char **argv) {
  MCE_DEFAULT_INIT("DEBUG");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
