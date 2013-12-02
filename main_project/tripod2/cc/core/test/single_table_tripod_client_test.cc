#include <iostream>
#include "OceProto/proto/UserBase.pb.h"
#include "base/port.h"
#include "tripod2/cc/client/single_table_tripod_client.h"
#include "LogWrapper.h"
#include "gtest/gtest.h"



namespace test {

using namespace xce::tripod;

class SingleTableClientTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
    client_ = new SingleTableTripodClient<xce::tripoddata::userbase::PbUserUrl,long>("test_namespace","user_url");
  }
  static void TearDownTestCase() {
    if (client_) {
      delete client_;
      client_ = 0;
    }
  }

  static SingleTableTripodClient<xce::tripoddata::userbase::PbUserUrl,long>* client_;

};

SingleTableTripodClient<xce::tripoddata::userbase::PbUserUrl,long>* SingleTableClientTest::client_;


TEST_F(SingleTableClientTest, Load) {
 // MCE_DEBUG( client_->Load((long)30050, 0) );
}


TEST_F(SingleTableClientTest, Get) {
  xce::tripoddata::userbase::PbUserUrl data;
  MCE_DEBUG( client_->Get((long)30050, &data) );
  MCE_DEBUG( data.tinyurl()  );
}

TEST_F(SingleTableClientTest, Remove) {
  xce::tripoddata::userbase::PbUserUrl data;
  MCE_DEBUG( client_->Remove((long)30050) );
  MCE_DEBUG( client_->Get((long)30050, &data) );
}


TEST_F(SingleTableClientTest, MultGet) {
  std::list<long> keys;
  keys.push_back(30021);
  keys.push_back(30022);
  keys.push_back(30023);
  keys.push_back(30024);
  keys.push_back(30025);
  keys.push_back(30006);
  keys.push_back(30017);
  keys.push_back(30018);
  keys.push_back(30019);

  std::map<long, xce::tripoddata::userbase::PbUserUrl> values;
  std::map<long, bool> result = client_->MultiGet(keys, &values);

  MCE_DEBUG("values size"<< values.size() << " " << result.size());
 
  std::map<long, bool>::const_iterator iter1 = result.begin();
  std::map<long, xce::tripoddata::userbase::PbUserUrl>::iterator iter = values.begin();
  while(iter!=values.end() ) {
    MCE_DEBUG("key"<< iter->first << " value:" << (iter->second).tinyurl() );
    ++iter;
  }
  while(iter1!=result.end() ) {
  MCE_DEBUG("key"<< iter1->first << ":" << iter1->second);
  ++iter1;
  }



}




}
