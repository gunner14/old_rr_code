#include <iostream>
#include "OceProto/proto/user_network.pb.h"
#include "base/port.h"
#include "OceCxxAdapter/src/user_network_tripod_adapter.h"
#include "LogWrapper.h"
#include "gtest/gtest.h"


namespace test {

using namespace xce::adapter;

class UserNetworkTripodAdapterTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
  static void TearDownTestCase() {
  }
};


TEST_F(UserNetworkTripodAdapterTest, LoadNetwork) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadNetwork(35178,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetNetwork) {
  xce::tripoddata::usernetwork::PbNetworkHistory data;
  UserNetworkTripodAdapter::instance().GetNetwork(35178,&data) ;
  MCE_DEBUG( data.userid()  );
}

TEST_F(UserNetworkTripodAdapterTest, LoadCollege) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadCollege(247428422,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetCollege) {
  xce::tripoddata::usernetwork::PbCollegeInfo data;
  UserNetworkTripodAdapter::instance().GetCollege(247428422,&data) ;
  MCE_DEBUG( data.userid()  );
}

TEST_F(UserNetworkTripodAdapterTest, LoadElementarySchool) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadElementarySchool(1115,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetElementarySchool) {
  xce::tripoddata::usernetwork::PbElementarySchoolInfo data;
  UserNetworkTripodAdapter::instance().GetElementarySchool(1115,&data) ;
  MCE_DEBUG( data.userid()  );
}

TEST_F(UserNetworkTripodAdapterTest, LoadHighSchool) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadHighSchool(35178,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetHighSchool) {
  xce::tripoddata::usernetwork::PbHighSchoolInfo data;
  UserNetworkTripodAdapter::instance().GetHighSchool(35178,&data) ;
  MCE_DEBUG( data.userid()  );
}

TEST_F(UserNetworkTripodAdapterTest, LoadJuniorHighSchool) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadJuniorHighSchool(1011,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetJuniorHighSchool) {
  xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo data;
  UserNetworkTripodAdapter::instance().GetJuniorHighSchool(1011,&data) ;
  MCE_DEBUG( data.userid()  );
}

TEST_F(UserNetworkTripodAdapterTest, LoadRegion) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadRegion(35178,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetRegion) {
  xce::tripoddata::usernetwork::PbRegionInfo data;
  UserNetworkTripodAdapter::instance().GetRegion(35178,&data) ;
  MCE_DEBUG( data.userid()  );
}

TEST_F(UserNetworkTripodAdapterTest, LoadUniversity) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadUniversity(35178,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetUniversity) {
  xce::tripoddata::usernetwork::PbUniversityInfo data;
  UserNetworkTripodAdapter::instance().GetUniversity(35178,&data) ;
  MCE_DEBUG( data.userid()  );
}

TEST_F(UserNetworkTripodAdapterTest, LoadWorkplace) {
  MCE_DEBUG( UserNetworkTripodAdapter::instance().LoadWorkplace(35178,false) );
}

TEST_F(UserNetworkTripodAdapterTest, GetWorkplace) {
  xce::tripoddata::usernetwork::PbWorkplaceInfo data;
  UserNetworkTripodAdapter::instance().GetWorkplace(35178,&data) ;
  MCE_DEBUG( data.userid()  );
}




}

