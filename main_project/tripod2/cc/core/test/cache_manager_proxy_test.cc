#include "gtest/gtest.h"

#include "base/port.h"
#include "tripod2/cc/core/cache_manager_proxy.h"
#include "LogWrapper.h"

#include "OceProto/proto/UserBase.pb.h"

namespace test {

using namespace xce::tripod;

class CacheManagerProxyTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
    cache_manager_proxy_ = new CacheManagerProxy("ljp_namespace", "test_biz");

    test_hash_value_.insert(std::make_pair("field6","CacheManagerProxyTest"));
  }
  static void TearDownTestCase() {
    cache_manager_proxy_->Remove(inc_key_);
    if (cache_manager_proxy_) {
      delete cache_manager_proxy_;
      cache_manager_proxy_ = 0;
    }
  }
  static CacheManagerProxy* cache_manager_proxy_;
  static StringMapType test_hash_value_;
  static std::string test_key;
  static std::string inc_key_;
};

CacheManagerProxy* CacheManagerProxyTest::cache_manager_proxy_;
std::string CacheManagerProxyTest::test_key = "test_hash_key";
std::string CacheManagerProxyTest::inc_key_ = "inc_key";
StringMapType CacheManagerProxyTest::test_hash_value_;

TEST_F(CacheManagerProxyTest, Constructor_And_Destructor) {
  ASSERT_TRUE(cache_manager_proxy_ != NULL);
}

TEST_F(CacheManagerProxyTest, SetPartHash) {
  ASSERT_TRUE(cache_manager_proxy_->SetPartHash(test_key, test_hash_value_));
}

TEST_F(CacheManagerProxyTest, Get) {
  std::string v;
  ASSERT_TRUE(cache_manager_proxy_->Get(test_key, &v, 100, true));
  using namespace xce::tripoddata::userbase;
  PbUserUrl url;
  ASSERT_TRUE(url.ParseFromString(v));
  MCE_DEBUG("url.id():" << url.id());
  MCE_DEBUG("url.mainurl():" << url.mainurl());
  MCE_DEBUG("url.tinyurl():" << url.tinyurl());
  MCE_DEBUG("url.headurl():" << url.headurl());
  MCE_DEBUG("url.flexurl():" << url.flexurl());
}

TEST_F(CacheManagerProxyTest, inc_normal) {
  cache_manager_proxy_->Set(inc_key_, "4", 100);
  ASSERT_TRUE(cache_manager_proxy_->Inc(inc_key_, "6") == "10");
  ASSERT_TRUE(cache_manager_proxy_->Inc(inc_key_, "-15") == "-5");
}

TEST_F(CacheManagerProxyTest, inc_new_value) {
  cache_manager_proxy_->Remove(inc_key_);
  ASSERT_TRUE(cache_manager_proxy_->Inc(inc_key_, "10") == "10");
}

TEST_F(CacheManagerProxyTest, inc_invalid_step) {
  cache_manager_proxy_->Set(inc_key_, "4", 100);
  ASSERT_TRUE(cache_manager_proxy_->Inc(inc_key_, "1.1") == "");
  ASSERT_TRUE(cache_manager_proxy_->Inc(inc_key_, "abc") == "");
  ASSERT_TRUE(cache_manager_proxy_->Inc(inc_key_, "0x1") == "");
}

TEST_F(CacheManagerProxyTest, inc_invalid_base) {
  cache_manager_proxy_->Set(inc_key_, "abc", 100);
  ASSERT_TRUE(cache_manager_proxy_->Inc(inc_key_, "1") == "");
}

TEST_F(CacheManagerProxyTest, Load) {
  ASSERT_TRUE(cache_manager_proxy_->Load(test_key, 100, true));
}

TEST_F(CacheManagerProxyTest, Remove) {
  ASSERT_TRUE(cache_manager_proxy_->Remove(test_key));
}
TEST_F(CacheManagerProxyTest, Set) {
  using namespace xce::tripoddata::userbase;
  PbUserUrl url;
  url.set_id(654321L);
  url.set_largeurl("largeurl444");
  url.set_mainurl("mainurl333");
  url.set_tinyurl("tinyurl222");
  url.set_headurl("headurl111");
  url.set_flexurl("flexurl000");
  std::string v;
  url.SerializeToString(&v);
  ASSERT_TRUE(cache_manager_proxy_->Set("654321", v, 100));
}

TEST_F(CacheManagerProxyTest, ZSet) {
using namespace std;
using namespace boost;
  String2StringMap set_with_scores;
  for (int i = 0; i < 100; i++) {
    set_with_scores.insert(make_pair(lexical_cast<string>(i), lexical_cast<string>(i)));
  }
  ASSERT_TRUE(cache_manager_proxy_->ZSet("TESTZSET", set_with_scores, 600));
}

} // test


