#include "gtest/gtest.h"

#include "base/port.h"
#include "tripod2/cc/core/producer_manager_proxy.h"
#include "LogWrapper.h"

namespace test {

using namespace  xce::tripod;

class ProducerManagerProxyTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
    producer_manager_proxy_ = new ProducerManagerProxy("test_namespace", "test_biz");
    url_proxy_ = new ProducerManagerProxy("test_namespace", "user_url");
    user_count_proxy_ = new ProducerManagerProxy("ljp_namespace", "user_count");
    notify_count_proxy_ = new ProducerManagerProxy("ljp_namespace", "notify_count");
    recent_menu_proxy_ = new ProducerManagerProxy("ljp_namespace", "user_recent_menu");
    favorite_menu_proxy_ = new ProducerManagerProxy("ljp_namespace", "user_favorite_menu");
    buddy_by_id_proxy_ = new ProducerManagerProxy("ljp_namespace", "buddy_by_id_cache");
    buddy_by_name_proxy_ = new ProducerManagerProxy("ljp_namespace", "buddy_by_name_cache");
    friend_count_proxy_ = new ProducerManagerProxy("ljp_namespace", "friend_count_cache");
  }
  static void TearDownTestCase() {
    if (producer_manager_proxy_) {
      delete producer_manager_proxy_;
      producer_manager_proxy_ = 0;
      delete url_proxy_;
      url_proxy_ = 0;

      delete user_count_proxy_;
      user_count_proxy_ = 0;
      delete notify_count_proxy_;
      notify_count_proxy_ = 0;
      delete recent_menu_proxy_;
      recent_menu_proxy_ = 0;
      delete favorite_menu_proxy_;
      favorite_menu_proxy_ = 0;
      delete buddy_by_id_proxy_;
      buddy_by_id_proxy_ = 0;
      delete buddy_by_name_proxy_;
      buddy_by_name_proxy_ = 0;
      delete friend_count_proxy_;
      friend_count_proxy_ = 0;
    }
  }
  static ProducerManagerProxy* producer_manager_proxy_;
  static ProducerManagerProxy* buddy_by_name_proxy_;
  static ProducerManagerProxy* url_proxy_;
  static ProducerManagerProxy* user_count_proxy_;
  static ProducerManagerProxy* notify_count_proxy_;
  static ProducerManagerProxy* recent_menu_proxy_;
  static ProducerManagerProxy* favorite_menu_proxy_;
  static ProducerManagerProxy* buddy_by_id_proxy_;
  static ProducerManagerProxy* friend_count_proxy_;
  static std::string test_key;
};

ProducerManagerProxy* ProducerManagerProxyTest::producer_manager_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::url_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::user_count_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::notify_count_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::recent_menu_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::favorite_menu_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::buddy_by_id_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::buddy_by_name_proxy_;
ProducerManagerProxy* ProducerManagerProxyTest::friend_count_proxy_;
std::string ProducerManagerProxyTest::test_key = "test_key";

/*TEST_F(ProducerManagerProxyTest, Constructor_And_Destructor) {
  ASSERT_TRUE(producer_manager_proxy_ != NULL);
  Sleep(1000); 
}

TEST_F(ProducerManagerProxyTest, Produce) {
  std::string v;
  ASSERT_TRUE(producer_manager_proxy_->Produce(test_key, &v, true));
}


TEST_F(ProducerManagerProxyTest, SingleTableProduce) {
  std::string v;
  ASSERT_TRUE(user_count_proxy_->Produce("30082", &v, true));

  //std::string v2;
  //ASSERT_TRUE(user_count_proxy_->Produce("30082", &v, true));
}

TEST_F(ProducerManagerProxyTest, ProduceMenu){
  std::string result;
  ASSERT_TRUE(favorite_menu_proxy_->Produce("1", &result, true));
  std::string result2;
  ASSERT_TRUE(recent_menu_proxy_->Produce("1", &result2, true));
  MCE_DEBUG("the recent result is "<< result2 << " the favorite result is " <<result);
}

TEST_F(ProducerManagerProxyTest, BuddyByIdCache) {
  std::string result;
  ASSERT_TRUE(buddy_by_id_proxy_->Produce("121664576", &result, false));
  MCE_DEBUG("the buddy_by_id_cache_ has create the value is: " << result << " the size of result is:" << result.size());

}

TEST_F(ProducerManagerProxyTest, BuddyByNameCache) {
  std::string result;
  ASSERT_TRUE(buddy_by_name_proxy_->Produce("121664576", &result, false));
  MCE_DEBUG("the buddy_by_name_cache_ has create the value is: " << result << " the size of result is:" << result.size());
}*/

TEST_F(ProducerManagerProxyTest, FriendCacheCount) {
  std::string result;
  ASSERT_TRUE(friend_count_proxy_->Produce("1", &result, false));
  MCE_DEBUG("The id of 1 has friends is:" << result);
}

} // test


