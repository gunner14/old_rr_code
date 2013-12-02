#include <iostream>
#include "OceProto/proto/UserConfig.pb.h"
#include "base/port.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "LogWrapper.h"
#include "gtest/gtest.h"

namespace test {

using namespace xce::tripod;

class UserConfigTest: public ::testing::Test {
protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
    client_ = new SingleTableTripodClient<xce::tripod::UserConfigData, long> ("xin.huang_namespace", "user_config");
  }
  static void TearDownTestCase() {
    if (client_) {
      delete client_;
      client_ = 0;
    }
  }

static SingleTableTripodClient<xce::tripod::UserConfigData, long>* client_;


public:
  static void printData(const xce::tripod::UserConfigData& data) {
    MCE_DEBUG("DATA---------------------");
    MCE_DEBUG("[id]:" << data.id());
    MCE_DEBUG("[email_config]:" << data.email_config());
    MCE_DEBUG("[basic_config]:" << data.basic_config());
    MCE_DEBUG("[poke_config]:" << data.poke_config());
    MCE_DEBUG("[request_friend_config]:" << data.request_friend_config());
    MCE_DEBUG("[photo_config]:" << data.photo_config());
    MCE_DEBUG("[message_config]:" << data.message_config());
    MCE_DEBUG("[browse_config]:" << data.browse_config());
    MCE_DEBUG("[display_menu_list]:" << data.display_menu_list());
    MCE_DEBUG("[more_menu_list]:" << data.more_menu_list());
    MCE_DEBUG("[want_see_css]:" << data.want_see_css());
    MCE_DEBUG("[send_feed_config]:" << data.send_feed_config());
    MCE_DEBUG("[recv_feed_config]:" << data.recv_feed_config());
    MCE_DEBUG("[profile_privacy]:" << data.profile_privacy());
    MCE_DEBUG("[status_config]:" << data.status_config());
  }


};

SingleTableTripodClient<xce::tripod::UserConfigData, long>* UserConfigTest::client_;
long testKey = 10;

TEST_F(UserConfigTest, Load) {
  // MCE_DEBUG( client_->Load((long)30050, 0) );
}

TEST_F(UserConfigTest, Get) {
  xce::tripod::UserConfigData data;
  MCE_DEBUG( client_->Get(testKey, &data, 60) );
  UserConfigTest::printData(data);
}

TEST_F(UserConfigTest, Remove) {
  xce::tripod::UserConfigData data;
  MCE_DEBUG( client_->Remove(testKey) );
//  MCE_DEBUG( client_->Get(testKey, &data) );
}

TEST_F(UserConfigTest, MultGet) {
  std::list<long> keys;
  //  keys.push_back(testKey);
  keys.push_back(11);
  keys.push_back(12);
  keys.push_back(13);
  keys.push_back(14);
  keys.push_back(15);
  keys.push_back(16);
  keys.push_back(17);
  keys.push_back(18);

  std::map<long, xce::tripod::UserConfigData> values;
  std::map<long, bool> result = client_->MultiGet(keys, &values);

  MCE_DEBUG("values size"<< values.size() << " " << result.size());

  std::map<long, xce::tripod::UserConfigData>::iterator iter = values.begin();
  while(iter!=values.end() ) {
    UserConfigTest::printData(iter->second);
    ++iter;
  }
  std::map<long, bool>::const_iterator iter1 = result.begin();
  while(iter1!=result.end() ) {
    MCE_DEBUG("key"<< iter1->first << ":" << iter1->second);
    ++iter1;
  }
}

}
