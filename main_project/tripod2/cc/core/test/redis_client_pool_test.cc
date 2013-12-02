#include "gtest/gtest.h"

#include "boost/foreach.hpp"
#include "tripod2/cc/core/redis_client.h"
#include "LogWrapper.h"

namespace test {

using namespace xce::tripod;

class RedisClientPoolTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
  void SetUp() {
    pool_ = new RedisClientPool("cmt0.xce.d.xiaonei.com", 6600, 1000, 5, 10);
  }
  void TearDown() {
    if (pool_) {
      delete pool_;
      pool_ = 0;
    }
  }

  CacheClientPool* pool_;
};

TEST_F(RedisClientPoolTest, BorrowItemExtremity) {
  std::vector<CacheClient*> borrowed;
  for (int i = 0; i < 15; ++i) {
    CacheClient* cache_client = pool_->BorrowItem();
    MCE_DEBUG("loop:" << i << "\tcache_client:" << cache_client << "\t" << pool_->Dump());
    if (cache_client) {
      borrowed.push_back(cache_client);
    }
  }
  BOOST_FOREACH(CacheClient* cc, borrowed) {
    pool_->ReturnItem(cc);
    MCE_DEBUG("ReturnItem:" << cc << "\t" << pool_->Dump());
  }
  MCE_DEBUG("After Return borrowed dump:" << pool_->Dump());
}
TEST_F(RedisClientPoolTest, BorrowItem) {
  CacheClient* client1 = pool_->BorrowItem();
  ASSERT_TRUE(client1 != NULL);
  ASSERT_TRUE(client1->IsValid());
  CacheClient* client2 = pool_->BorrowItem();
  ASSERT_TRUE(client2 != NULL);
  ASSERT_TRUE(client2->IsValid());
  pool_->ReturnItem(client1);
  pool_->ReturnItem(client2);
} 

TEST_F(RedisClientPoolTest, Set) {
  CacheClient* client = pool_->BorrowItem();
  MCE_DEBUG(pool_->Dump());
  ASSERT_TRUE(client != NULL);
  ASSERT_TRUE(client->IsValid());
  std::string value = "Michael";
  ASSERT_TRUE(client->Set("abc", value));
  pool_->ReturnItem(client);
}


TEST_F(RedisClientPoolTest, Get) {
  CacheClient* client = pool_->BorrowItem();
  MCE_DEBUG(pool_->Dump());
  ASSERT_TRUE(client != NULL);
  ASSERT_TRUE(client->IsValid());
  std::string value;
  ASSERT_TRUE(client->Get("abc", &value));
  MCE_DEBUG("value:" << value);
  pool_->ReturnItem(client);
}

}  // test


