#include "gtest/gtest.h"
#include "base/port.h"
#include "tripod2/cc/client/entry_type.h"
#include "tripod2/cc/client/tripod_cache_client_factory.h"
#include "LogWrapper.h"

namespace test {

using namespace xce::tripod::client;
using namespace boost;
using xce::tripod::StringMapType;
using xce::tripod::StringListType;
using std::map;
using std::string;

class TripodCacheClientTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("TRACE");
    client_ = TripodCacheClientFactory::GetInstance().GetClient(entry_type::kBizSocialgraphCommon);
  }
  static void TearDownTestCase() {
  }
  static TripodCacheClientPtr client_;
};

TripodCacheClientPtr TripodCacheClientTest::client_;

TEST_F(TripodCacheClientTest, AAA) {
   client_->Set("key1","value1");
   std::string val;
   client_->Get("key1",&val);
   MCE_INFO(val);
}

TEST_F(TripodCacheClientTest, BasicOps) {
  std::string val;
  EXPECT_EQ(client_->Set("key2", "value23"), true);
  EXPECT_EQ(client_->Set("key3", "value33"), true);
  EXPECT_EQ(client_->Get("key2", &val), true);

  StringMapType kv_pairs;
  StringListType keys;
  keys.push_back("key2");
  keys.push_back("key3");
  keys.push_back("key_j$@#@");
  std::map<string, bool> result;
  result = client_->MultiGet(keys, &kv_pairs);
  MCE_INFO("MultiGet result:");
  for (map<string, bool>::const_iterator cit = result.begin();
      cit != result.end(); ++cit) {
    MCE_INFO(cit->first << " - " << cit->second);
  }
  MCE_INFO("MultiGet kv-pairs:");
  for (StringMapType::const_iterator cit = kv_pairs.begin();
      cit != kv_pairs.end(); ++cit) {
    MCE_INFO(cit->first << " - " << cit->second);
  }
  
  EXPECT_EQ(client_->Get("non-exist-key-$#@%$#", &val), false); 
}

TEST_F(TripodCacheClientTest, BasicOps_Abnormal) {
  std::string val;
  EXPECT_EQ(client_->Get("non-exist-key-$#@%$#", &val), false); 
  MCE_INFO(val);
  EXPECT_EQ(val.empty(), true);
}

TEST_F(TripodCacheClientTest, CounterOps) {
  long  value = 9338, step = 3, result = 0;

  EXPECT_EQ(client_->SetLong("mykey", value, 100), true);
  EXPECT_EQ(client_->GetLong("mykey", &result), true);
  EXPECT_EQ(value, result);

  EXPECT_EQ(client_->IncLong("mykey", step, &result), true);
  EXPECT_EQ(value + step, result);

  EXPECT_EQ(client_->SetLong("mykey", 200, 100), true);
  // inc with negative step
  EXPECT_EQ(client_->IncLong("mykey", -100, &result), true);
}
// 测试边界情况
TEST_F(TripodCacheClientTest, CounterOps_Abnormal) {
  long result;
  // set with negative number
  // 目前没有对输入的value做检查
  EXPECT_EQ(client_->SetLong("mykey", -100, 100), true);
  // get key with negative value
  EXPECT_EQ(client_->GetLong("mykey", &result), true);
  EXPECT_EQ(result, -100);
  MCE_INFO("GetLong result: " << result);
  EXPECT_EQ(client_->IncLong("mykey", -3, &result), true);
  EXPECT_EQ(-103, result);
  // get with non-exist key
  EXPECT_EQ(client_->GetLong("non-exist-key-blablabla..", &result), false);
  EXPECT_EQ(result, -1);
  // inc with non-exist key
  EXPECT_EQ(client_->IncLong("non-exist-key-^$#@#", 34, &result), false);
}

TEST_F(TripodCacheClientTest, SetEmptyKey) {
  std::string value;
  std::string key = "emptyStr";
  std::string result;
  ASSERT_FALSE(client_->Set(key, value, 6));
  ASSERT_FALSE(client_->Get(key, &result));
  MCE_INFO("The set item");
  value = "newkey";
  ASSERT_TRUE(client_->Set(key, value, 6));
  ASSERT_TRUE(client_->Get(key, &result));
  MCE_INFO("the get value is " << result);
}

//list测试
TEST_F(TripodCacheClientTest, GetList) {
  StringListType value;
  std::string key = "testKey";
  ASSERT_FALSE(client_->GetList(key, 0, -1, &value));
  MCE_INFO("The Size of value get from no set key size " << value.size());
  StringListType set_value;
  for(int i = 0; i < 20; i++) {
    set_value.push_back("test"+lexical_cast<string>(i));
  }
  ASSERT_TRUE(client_->SetList(key, set_value, 6));
  ASSERT_TRUE(client_->GetList(key, 0, -1, &value));
  ostringstream out;
  out << "{ ";
  for(StringListType::iterator iter = value.begin(); iter != value.end(); iter ++) {
    out << *iter << " ";
  }
  out << "}";
  MCE_INFO("value is :" << out.str());
}
TEST_F(TripodCacheClientTest, GetListFromEmptyList) {
  StringListType value;
  std::string key = "emptykey";
  StringListType result;
  MCE_INFO("value is empty() ?" << value.empty());
  ASSERT_FALSE(client_->SetList(key, value, 6));
  ASSERT_FALSE(client_->GetList(key, 0, -1, &result));
  MCE_INFO("Set a list");
  value.push_back("1");
  value.push_back("2");
  value.push_back("3");
  value.push_back("4");
  ASSERT_TRUE(client_->SetList(key, value, 6));
  ASSERT_TRUE(client_->GetList(key, 0, -1, &result));
  MCE_INFO("GetList finished!" << result.size());
  ostringstream out;
  out << "{ ";
  for(StringListType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    out << *iter << " " ;
  }
  out << " }";
  MCE_INFO("value is :" << out.str());
  MCE_INFO("After the set empty value");
  value.clear();
  result.clear();
  ASSERT_FALSE(client_->SetList(key, value, 6)); 
  ASSERT_TRUE(client_->GetList(key, 0, -1, &result));
  out.str("");
  out << "{ ";
  for(StringListType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    out << *iter << " " ;
  }
  out << " }";
  MCE_INFO("value is :" << out.str());
}


TEST_F(TripodCacheClientTest, ZSetOps) {
  StringMapType set_with_score;
  StringListType zset_value;
  set_with_score.insert(pair<string, string>("1", "one"));
  set_with_score.insert(pair<string, string>("2", "two"));
  set_with_score.insert(pair<string, string>("3", "three"));
  set_with_score.insert(pair<string, string>("4", "four"));

  std::string key = "mykey";
  EXPECT_EQ(client_->ZSet(key, set_with_score, 100), true);
  EXPECT_EQ(client_->ZRange(key, 0, 4, &zset_value), true);

  set_with_score.clear();
  set_with_score.insert(pair<string, string>("6", "hei"));
  EXPECT_EQ(client_->ZAdd(key, set_with_score), true);
  client_->ZRange(key, 0, 5, &zset_value);

  EXPECT_EQ(client_->ZRemRangeByRank(key, 1, 2), true);
  EXPECT_EQ(client_->ZRange(key, 0, 4, &zset_value), true);
  zset_value.clear();
  MCE_INFO("ZRange(key, 0, 4, &zset_value) after remove ");
  for (StringListType::const_iterator cit = zset_value.begin();
      cit != zset_value.end(); ++cit) {
    MCE_INFO(*cit);
  }
}


TEST_F(TripodCacheClientTest, ZremByScore) {
  StringMapType set_with_score;
  StringListType zset_value;
  set_with_score.insert(pair<string, string>("0", "zero"));
  set_with_score.insert(pair<string, string>("1", "one"));
  set_with_score.insert(pair<string, string>("2", "two"));
  set_with_score.insert(pair<string, string>("3", "three"));
  set_with_score.insert(pair<string, string>("4", "four"));
  std::string key = "testscoreKey";
  EXPECT_EQ(client_->ZSet(key, set_with_score, 100), true);
  client_->ZRange(key, 0, 5, &zset_value);
  for (StringListType::const_iterator cit = zset_value.begin();
      cit != zset_value.end(); ++cit) {
    MCE_INFO(*cit);
  }
  ASSERT_TRUE(client_->ZRemRangeByScore(key, "2", "4"));
  zset_value.clear(); 
  MCE_INFO("After the zrem by score");
  EXPECT_EQ(client_->ZRange(key, 0, 4, &zset_value), true);
  for (StringListType::const_iterator cit = zset_value.begin();
      cit != zset_value.end(); ++cit) {
    MCE_INFO(*cit);
  }
  zset_value.clear(); 
  ASSERT_FALSE(client_->ZRemRangeByScore(key, "abnormal", "3"));
  ASSERT_TRUE(client_->ZRange(key, 0, -1, &zset_value));
  zset_value.clear(); 
  ASSERT_TRUE(client_->ZRemRangeByScore(key, "-inf", "+inf"));
  ASSERT_FALSE(client_->ZRange(key, 0, -1, &zset_value)); 
}

// test abnormal zset ops
TEST_F(TripodCacheClientTest, ZSetOps_Abnormal) {
  using xce::tripod::StringMapType;
  using xce::tripod::StringListType;

  StringMapType set_with_score;
  StringListType zset_value;
  set_with_score.insert(pair<string, string>("1", "one"));

  std::string key = "my-new-key";
  client_->Remove(key);
  // add before set
  EXPECT_EQ(client_->ZAdd(key, set_with_score), false);

  set_with_score.insert(pair<string, string>("2", "two"));
  set_with_score.insert(pair<string, string>("3", "three"));
  set_with_score.insert(pair<string, string>("4", "four"));
  EXPECT_EQ(client_->ZSet(key, set_with_score, 10), true);

  EXPECT_EQ(client_->ZRange(key, 0, 0, &zset_value), true);
  MCE_INFO("ZRange(key, 0, 0, &zset_value)");
  for (StringListType::const_iterator cit = zset_value.begin();
      cit != zset_value.end(); ++cit) {
    MCE_INFO(*cit);
  }
  // begin < 0
  EXPECT_EQ(client_->ZRange(key, -3, 6, &zset_value), false);
  MCE_INFO("ZRange(key, -3, 6, &zset_value)");
  for (StringListType::const_iterator cit = zset_value.begin();
      cit != zset_value.end(); ++cit) {
    MCE_INFO(*cit);
  }
  // large limit
  EXPECT_EQ(client_->ZRange(key, 0, 600, &zset_value), true);
  MCE_INFO("ZRange(key, 0, 600, &zset_value)");
  for (StringListType::const_iterator cit = zset_value.begin();
      cit != zset_value.end(); ++cit) {
    MCE_INFO(*cit);
  }
  // remove with negative begin
  EXPECT_EQ(client_->ZRemRangeByRank(key, -1, 2), false);
  // remove with 0 limit 
  EXPECT_EQ(client_->ZRemRangeByRank(key, 0, 0), true);
  // zrange with wrong range large neagtive begin
  EXPECT_EQ(client_->ZRange(key, -100, 10, &zset_value), false);
  // ZRemRangeByRank with large neagtive, but still return true.
  EXPECT_EQ(client_->ZRemRangeByRank(key, 0, -100), true);
  // set with illegal score
  set_with_score.clear();
  set_with_score.insert(pair<string, string>("3jk", "five"));
  EXPECT_EQ(client_->ZSet(key, set_with_score, 10), false);
}

} // test


