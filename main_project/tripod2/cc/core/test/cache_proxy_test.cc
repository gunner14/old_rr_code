#include "gtest/gtest.h"

#include "base/port.h"
#include "tripod2/cc/core/zk_client.h"
#include "tripod2/cc/core/cache_proxy.h"
#include "LogWrapper.h"

namespace test {

using namespace xce::tripod;

class CacheProxyTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
    cache_proxy_ = new CacheProxy("ljp_namespace", "test_biz");
    cache_set_proxy_ = new CacheProxy("ljp_namespace", "zset_test_biz");
    inc_key_ = "inc_key";
    test_key = "test_key";
    test_value = "test_value_haha";

    test_list_key = "test_list";
    test_list_value.push_back("list1");
    test_list_value.push_back("list2");
    test_list_value.push_back("list3");

    test_hash_key = "test_hash_key";
    test_hash_fields.push_back("field1");
    test_hash_fields.push_back("field2");
    test_hash_fields.push_back("field3");

    test_hash_value.insert(std::make_pair("field1", "1"));
    test_hash_value.insert(std::make_pair("field2", "2"));
    test_hash_value.insert(std::make_pair("field3", "3"));
    
    test_hmset_fields.push_back("field4");
    test_hmset_fields.push_back("field5");
    del_list_.push_back("field4");
     
    test_hmset_value.insert(std::make_pair("field4", "4"));
    test_hmset_value.insert(std::make_pair("field5", "5"));

  }
  static void TearDownTestCase() {
    cache_proxy_->Remove(inc_key_);
    if (cache_proxy_) {
      delete cache_proxy_;
      cache_proxy_ = 0;
    }
  }

  static std::string inc_key_;
  static std::string test_key;
  static std::string test_value;

  static std::string test_list_key;
  static StringListType test_list_value;
  static StringListType del_list_;
  static std::string test_hash_key;
  static StringListType test_hash_fields;
  static StringMapType test_hash_value;
  static StringListType test_hmset_fields;
  static StringMapType test_hmset_value;

  static CacheProxy* cache_proxy_;
  static CacheProxy* cache_set_proxy_;
};

CacheProxy* CacheProxyTest::cache_proxy_;
CacheProxy* CacheProxyTest::cache_set_proxy_;
std::string CacheProxyTest::inc_key_; 
std::string CacheProxyTest::test_key; 
std::string CacheProxyTest::test_value;
std::string CacheProxyTest::test_list_key;
StringListType CacheProxyTest::test_list_value;
std::string CacheProxyTest::test_hash_key;
StringListType CacheProxyTest::test_hash_fields;
StringMapType CacheProxyTest::test_hash_value;
StringListType CacheProxyTest::test_hmset_fields;
StringMapType CacheProxyTest::test_hmset_value;
StringListType CacheProxyTest::del_list_;

TEST_F(CacheProxyTest, Constructor_And_Destructor) {
  ASSERT_TRUE(cache_proxy_->IsValid());
}

TEST_F(CacheProxyTest, Remove) {
  ASSERT_TRUE(cache_proxy_->Remove(test_key));
}

TEST_F(CacheProxyTest, Set) {
  ASSERT_TRUE(cache_proxy_->Set(test_key, test_value));
}

TEST_F(CacheProxyTest, Get) {
  std::string v;
  ASSERT_TRUE(cache_proxy_->Get(test_key, &v));
  ASSERT_EQ(v, test_value);
}

TEST_F(CacheProxyTest, inc_normal) {
  cache_proxy_->Set(inc_key_, "4", 600);
  ASSERT_TRUE(cache_proxy_->Inc(inc_key_, "6") == "10");
  ASSERT_TRUE(cache_proxy_->Inc(inc_key_, "-15") == "-5");
}

TEST_F(CacheProxyTest, inc_new_value) {
  cache_proxy_->Remove(inc_key_);
  ASSERT_TRUE(cache_proxy_->Inc(inc_key_, "10") == "10");
}

TEST_F(CacheProxyTest, inc_invalid_step) {
  cache_proxy_->Set(inc_key_, "4");
  ASSERT_TRUE(cache_proxy_->Inc(inc_key_, "1.1") == "");
  ASSERT_TRUE(cache_proxy_->Inc(inc_key_, "abc") == "");
  ASSERT_TRUE(cache_proxy_->Inc(inc_key_, "0x1") == "");
}

TEST_F(CacheProxyTest, inc_invalid_base) {
  cache_proxy_->Set(inc_key_, "abc");
  ASSERT_TRUE(cache_proxy_->Inc(inc_key_, "1") == "");
}

TEST_F(CacheProxyTest, SetList) {
  ASSERT_TRUE(cache_proxy_->SetList(test_list_key, test_list_value));
}

TEST_F(CacheProxyTest, GetList) {
  StringListType lv;
  ASSERT_TRUE(cache_proxy_->GetList(test_list_key, &lv, 0, 1000));
  ASSERT_EQ(lv, test_list_value);
}

TEST_F(CacheProxyTest, SetHash) {
  ASSERT_TRUE(cache_proxy_->SetHash(test_hash_key, test_hash_value));
}
TEST_F(CacheProxyTest, GetHash) {
  StringMapType hv;
  ASSERT_TRUE(cache_proxy_->GetHash(test_hash_key, test_hash_fields, &hv));
  ASSERT_EQ(hv, test_hash_value);
}

TEST_F(CacheProxyTest, HMSet) {
  ASSERT_TRUE(cache_proxy_->HMSet(test_hash_key, test_hmset_value));
}

TEST_F(CacheProxyTest, GetHash2) {
  StringMapType hv;
  ASSERT_TRUE(cache_proxy_->GetHash(test_hash_key, test_hmset_fields, &hv));
  MCE_INFO("The value length is :" << hv.size());
  ASSERT_EQ(hv, test_hmset_value);
}

TEST_F(CacheProxyTest, HDel) {
  ASSERT_TRUE(cache_proxy_->HDel(test_hash_key, del_list_));
}

TEST_F(CacheProxyTest, HGetAll) {
  StringMapType hv;
  ASSERT_TRUE(cache_proxy_->HGetAll(test_hash_key, &hv));
  StringMapType::iterator iter = hv.begin();
  MCE_INFO("The value length is :" << hv.size());
  while(iter != hv.end()) {
    MCE_INFO("field :" << iter->first << " value :" << iter->second);
    ++iter;
  }
}

TEST_F(CacheProxyTest, HIncrBy) {
  ASSERT_TRUE(cache_proxy_->HIncrBy(test_hash_key,"field1", "-888"));
  StringMapType hv;
  ASSERT_TRUE(cache_proxy_->HGetAll(test_hash_key, &hv));
  StringMapType::iterator iter = hv.begin();
  MCE_INFO("After the inc operation!!!!!!!!!!!!!1");
  while(iter != hv.end()) {
    MCE_INFO("field :" << iter->first << " value :" << iter->second);
    ++iter;
  }
}


TEST_F(CacheProxyTest, Exists) {
  ASSERT_TRUE(cache_proxy_->Exists(test_key));
}

TEST_F(CacheProxyTest, Size) {
  int size = cache_proxy_->Size(test_key);
  MCE_DEBUG("size:" << size);
}


TEST_F(CacheProxyTest, MultiGet) {
  ASSERT_TRUE(cache_proxy_->Set("zzz", "aaa"));
  ASSERT_TRUE(cache_proxy_->Set("yyy", "bbb"));
  ASSERT_TRUE(cache_proxy_->Set("xxx", "ccc"));
  ASSERT_TRUE(cache_proxy_->Set("uuu", "ddd"));
  ASSERT_TRUE(cache_proxy_->Set("vvv", "eee"));
  ASSERT_TRUE(cache_proxy_->Set("www", "fff"));
  ASSERT_TRUE(cache_proxy_->Set("rrr", "ggg"));
  ASSERT_TRUE(cache_proxy_->Set("sss", "hhh"));
  ASSERT_TRUE(cache_proxy_->Set("ttt", "iii"));
  StringListType keys;
  keys.push_back("zzz");
  keys.push_back("yyy");
  keys.push_back("xxx");
  keys.push_back("uuu");
  keys.push_back("vvv");
  keys.push_back("www");
  keys.push_back("rrr");
  keys.push_back("sss");
  keys.push_back("ttt");
  keys.push_back("tmn");
  keys.push_back("mnt");
  StringMapType values;
  std::map<std::string, bool> result =  cache_proxy_->MultiGet(keys, &values);
  MCE_DEBUG("values size"<< values.size() << " " << result.size());
  StringListType::iterator it = keys.begin();
  while(it!=keys.end() ) {
    MCE_DEBUG("key"<<*it);
    ++it;
  }
}

TEST_F(CacheProxyTest, ZSet) {
  using namespace std;
  using namespace boost;
  StringMapType set_with_scores;
  for (int i = 0; i < 100; i++) {
    set_with_scores.insert(make_pair(lexical_cast<string>(i), lexical_cast<string>(i)));
  }
  ASSERT_TRUE(cache_set_proxy_->ZSet("ZSET_TEST", set_with_scores, 600) );
}

TEST_F(CacheProxyTest, ZRange) {
  using namespace std;
  using namespace boost;
  StringListType result;
  ASSERT_TRUE(cache_set_proxy_->ZRange("ZSET_TEST", &result, 0, 20) );
  ostringstream otem; 
  otem << "Value is:{ ";
  for (StringListType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    otem << *iter <<" ";
  }
  otem << "}";
  MCE_DEBUG(otem.str());
}

} // test


