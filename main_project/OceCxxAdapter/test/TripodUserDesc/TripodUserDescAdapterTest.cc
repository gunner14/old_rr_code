#include "../share/TripodSimpleAdapterTest.h"

#include "TripodUserTimeAdapter.h"
#include "TripodUserContactAdapter.h"

using namespace xce::adapter;
using namespace xce::tripoddata::userdesc;

using testing::Types;

typedef Types< 
          TS< TripodUserTimeAdapter, PbUserTime > ,
          TS< TripodUserContactAdapter, PbUserContact >
          > AdapterImplementations;

INSTANTIATE_TYPED_TEST_CASE_P(TripodUserDescAdapterTest, TripodSimpleAdapterTest, AdapterImplementations);

int main(int argc, char **argv) {
  MCE_DEFAULT_INIT("DEBUG");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
