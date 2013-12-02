#include "gtest/gtest.h"

#include "boost/foreach.hpp"

#include "tripod2/cc/core/redis_client.h"
#include "LogWrapper.h"

#include "OceProto/proto/UserBase.pb.h"
#include "boost/lexical_cast.hpp"
#include <sstream>

namespace test {

using namespace xce::tripod;

class RedisClientTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
    MCE_DEBUG("SetUpTestCase----------------------------");
    inc_key_ = "inc_key";
    test_key_ = "abc";
    map_key_ = "lin";
    redis_client_ = new RedisClient("10.3.22.21", 6600, 1000);
    redis_client2_ = new RedisClient("10.3.22.21", 6600, 1000);

    hash_value_.insert(std::make_pair("field1", "value1"));
    hash_value_.insert(std::make_pair("field2", "value2"));
    hash_value_.insert(std::make_pair("field3", "value3"));
    hash_value_.insert(std::make_pair("field4", "value4"));
    hash_value_.insert(std::make_pair("field5", "value5"));
    string_map_.insert(std::make_pair("int1", "1"));
    string_map_.insert(std::make_pair("int2", "2"));
    string_map_.insert(std::make_pair("int3", "3"));
    string_map_.insert(std::make_pair("int4", "4"));
    string_map_.insert(std::make_pair("int5", "5"));
    del_fields_.push_back("field3");
  }
  void SetUp() {
  }
  void TearDown() {
  }
  static void TearDownTestCase() {
    redis_client_->Remove(inc_key_);
    if (redis_client_) {
      delete redis_client_;
      redis_client_ = 0;
    }
    if (redis_client2_) {
      delete redis_client2_;
      redis_client2_ = 0;
    }
  }
  static std::string inc_key_;
  static StringMapType string_map_;
  static std::string map_key_;
  static StringMapType hash_value_;
  static StringListType del_fields_;
  static std::string test_key_;
  static RedisClient* redis_client_;
  static RedisClient* redis_client2_;
};

std::string RedisClientTest::inc_key_;
StringMapType RedisClientTest::string_map_;
std::string RedisClientTest::map_key_;
StringMapType RedisClientTest::hash_value_;
StringListType RedisClientTest::del_fields_;
std::string RedisClientTest::test_key_;
RedisClient* RedisClientTest::redis_client_;
RedisClient* RedisClientTest::redis_client2_;

TEST_F(RedisClientTest, HMSET) {
  using namespace std;
  StringMapType set_value;
  set_value.insert(make_pair("1", "1"));
  set_value.insert(make_pair("2", "2"));
  ASSERT_FALSE(redis_client_->HMSet("hmsetkey", set_value) );
  StringMapType result;
  ASSERT_TRUE(redis_client_->HGetAll("hmsetkey", &result) );
  for(StringMapType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    MCE_DEBUG(iter->first << " " << iter->second);
  }
  StringMapType set_valueT;
  set_valueT.insert(make_pair("3", "3") );
  ASSERT_TRUE(redis_client_->SetHash("hmsetkey", set_valueT, 20));
  sleep(1);
  ASSERT_TRUE(redis_client_->HMSet("hmsetkey", set_value) );
  result.clear();
  ASSERT_TRUE(redis_client_->HGetAll("hmsetkey", &result) );
  for(StringMapType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    MCE_DEBUG(iter->first << " " << iter->second);
  } 
}


TEST_F(RedisClientTest, zaddnoexpire) {
  using namespace boost;
  using namespace std;
  StringMapType add_value_;
  add_value_.insert(make_pair(lexical_cast<string>(1), lexical_cast<string>(1)));
  add_value_.insert(make_pair(lexical_cast<string>(2), lexical_cast<string>(2)));
  ASSERT_FALSE(redis_client_->ZAdd("zaddnoexpire", add_value_));
  StringListType result;
  ostringstream otem;
  redis_client_->ZRange("zaddnoexpire", 0, -1, &result);
  otem << "Value is :";
  for (StringListType::iterator iter = result.begin(); iter != result.end(); iter ++) {
    otem << *iter << " ";
  }
  MCE_DEBUG(otem.str());
}
TEST_F(RedisClientTest, zaddexprie) {
  using namespace boost;
  using namespace std;
  StringMapType set_value_;
  set_value_.insert(make_pair("3", "3"));
  ASSERT_TRUE(redis_client_->ZSet("zaddnoexpire", set_value_, 20));
  StringMapType add_value_;
  add_value_.insert(make_pair(lexical_cast<string>(1), lexical_cast<string>(1)));
  add_value_.insert(make_pair(lexical_cast<string>(2), lexical_cast<string>(2)));
  ASSERT_TRUE(redis_client_->ZAdd("zaddnoexpire", add_value_));
  StringListType result;
  ostringstream otem;
  redis_client_->ZRange("zaddnoexpire", 0, -1, &result);
  otem << "Value is :";
  for (StringListType::iterator iter = result.begin(); iter != result.end(); iter ++) {
    otem << *iter << " ";
  }
  MCE_DEBUG(otem.str());
   
}


/*TEST_F(RedisClientTest, zset) {
using namespace boost;
using namespace std;
  StringMapType set_value_;
  for (int i = 1; i <= 5000; i ++) {
    set_value_.insert(make_pair(lexical_cast<string>(i), lexical_cast<string>(i)));
  }
  ASSERT_TRUE(redis_client_->ZSet("zsetTest", set_value_, 6000));
}

TEST_F(RedisClientTest, zrange) {
  using namespace std;
  using namespace boost;
  StringListType result;
  ASSERT_TRUE(redis_client_->ZRange("zsetTest", 0, 20, &result));
  ostringstream otem;
  otem << "Value is { ";
  for (StringListType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    otem << *iter << " ";
  }
  otem << "}";
  MCE_DEBUG(otem.str());
}

TEST_F(RedisClientTest, zrem) {
  using namespace std;
  using namespace boost;
  ASSERT_TRUE(redis_client_->ZRemRangeByRank("zsetTest", 10, 8));
}

TEST_F(RedisClientTest, zrange2) {
  using namespace std;
  using namespace boost;
  StringListType result;
  ASSERT_TRUE(redis_client_->ZRange("zsetTest", 0, 20, &result));
  ostringstream otem;
  otem << "Value is { ";
  for (StringListType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    otem << *iter << " ";
  }
  otem << "}";
  MCE_DEBUG(otem.str());
}

TEST_F(RedisClientTest, zsetInvalid) {
  using namespace std;
  using namespace boost;
  StringMapType set_value;
  set_value.insert(make_pair("invalid", "test"));
  ASSERT_FALSE(redis_client_->ZSet("zsetTest2", set_value, 600));
}

TEST_F(RedisClientTest, zAddValid) {
  using namespace std;
  using namespace boost;
  StringMapType set_value;
  set_value.insert(make_pair("1", "test1"));
  set_value.insert(make_pair("2", "test2"));
  set_value.insert(make_pair("3", "test3"));
  set_value.insert(make_pair("4", "test4"));
  set_value.insert(make_pair("5", "test5"));
  ASSERT_TRUE(redis_client_->ZAdd("zsetTest3", set_value)); 
}

TEST_F(RedisClientTest, zaddInvalid) {
  using namespace std;
  using namespace boost;
  StringMapType set_value;
  set_value.insert(make_pair("invalid", "test"));
  ASSERT_FALSE(redis_client_->ZAdd("zsetTest3", set_value)); 
}

TEST_F(RedisClientTest, zRangeAll) {
  using namespace std;
  using namespace boost;
  {
    StringListType result;
    ASSERT_FALSE(redis_client_->ZRange("zsetTest2", 0, 20, &result));
    ostringstream otem;
    otem << "Value is { ";
    for (StringListType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
      otem << *iter << " ";
    }
    otem << "}";
    MCE_DEBUG(otem.str());
  }
  {
    StringListType result;
    ASSERT_TRUE(redis_client_->ZRange("zsetTest3", 0, 20, &result));
    ostringstream otem;
    otem << "Value is { ";
    for (StringListType::const_iterator iter = result.begin(); iter != result.end(); iter++) {
      otem << *iter << " ";
    }
    otem << "}";
    MCE_DEBUG(otem.str());
  }
}
TEST_F(RedisClientTest, inc_normal) {
  redis_client_->Set(inc_key_, "4", 10);
  ASSERT_TRUE(redis_client_->Inc(inc_key_, "6") == "10");
  ASSERT_TRUE(redis_client_->Inc(inc_key_, "-15") == "-5");
}

TEST_F(RedisClientTest, inc_new_value) {
  redis_client_->Remove(inc_key_);
  //不支持不过期时间
  //ASSERT_TRUE(redis_client_->Inc(inc_key_, "10") == "10");
}

TEST_F(RedisClientTest, inc_invalid_step) {
  redis_client_->Set(inc_key_, "4", 10);
  ASSERT_TRUE(redis_client_->Inc(inc_key_, "1.1") == "");
  ASSERT_TRUE(redis_client_->Inc(inc_key_, "abc") == "");
  ASSERT_TRUE(redis_client_->Inc(inc_key_, "0x1") == "");
}

TEST_F(RedisClientTest, inc_invalid_base) {
  redis_client_->Set(inc_key_, "abc", 10);
  ASSERT_TRUE(redis_client_->Inc(inc_key_, "1") == "");
}

TEST_F(RedisClientTest, inc_expire) {
  redis_client_->Set(inc_key_, "4", 1);
  sleep(2);
  ASSERT_TRUE(redis_client_->Inc(inc_key_, "1") == "");
  string value;
  redis_client_->Get(inc_key_, &value);
  ASSERT_TRUE(value == "");
}

TEST_F(RedisClientTest, Inc) {
  ASSERT_TRUE(redis_client2_->SetHash(map_key_, string_map_));
  ASSERT_TRUE(redis_client2_->HIncrBy(map_key_, "int3", "-220"));
  StringMapType hv;
  ASSERT_TRUE(redis_client2_->HGetAll(map_key_, &hv));
  MCE_DEBUG("After the Inc of 20 the map is:");
  StringMapType::const_iterator iter = hv.begin();
  while(iter != hv.end()) {
    MCE_DEBUG("key:" << test_key_ << " field:"<< iter->first << " value:"<< iter->second);
    iter++;
  }
  
  
}

TEST_F(RedisClientTest, SetHash2) {
  ASSERT_TRUE(redis_client_->SetHash(test_key_, hash_value_));
}

TEST_F(RedisClientTest, HDel) {
  ASSERT_TRUE(redis_client_->HDel(test_key_, del_fields_));
}

TEST_F(RedisClientTest, HGetAll) {
  StringMapType hv;
  ASSERT_TRUE(redis_client_->HGetAll(test_key_, &hv));
  StringMapType::const_iterator iter = hv.begin();
  while(iter != hv.end()) {
    MCE_DEBUG("key:" << test_key_ << " field:"<< iter->first << " value:"<< iter->second);
    iter++;
  }
}

TEST_F(RedisClientTest, UserUrl) {
  using namespace xce::tripoddata::userbase; 
  PbUserUrl url;
  url.set_id(35201L);
  url.set_largeurl("largeurl...");
  url.set_mainurl("mainurl...");
  url.set_tinyurl("tinyurl...");
  url.set_headurl("headurl...");
  url.set_flexurl("flexurl...");
  std::string v;
  ASSERT_TRUE(url.SerializeToString(&v));
  ASSERT_TRUE(redis_client_->Set("liyong", v));
  MCE_DEBUG("url.id():" << url.id());
  MCE_DEBUG("url.mainurl():" << url.mainurl());
  MCE_DEBUG("url.tinyurl():" << url.tinyurl());
  MCE_DEBUG("url.headurl():" << url.headurl());
  MCE_DEBUG("url.flexurl():" << url.flexurl());

  std::string v2;
  ASSERT_TRUE(redis_client_->Get("liyong", &v2));
  PbUserUrl url2;
  ASSERT_TRUE(url2.ParseFromString(v2));
  MCE_DEBUG("url2.id():" << url2.id());
  MCE_DEBUG("url2.mainurl():" << url2.mainurl());
  MCE_DEBUG("url2.tinyurl():" << url2.tinyurl());
  MCE_DEBUG("url2.headurl():" << url2.headurl());
  MCE_DEBUG("url2.flexurl():" << url2.flexurl());
}


TEST_F(RedisClientTest, Type) {
  ASSERT_TRUE(redis_client_ != NULL)<<"it is the type failed in the first test!";
  ASSERT_TRUE(redis_client_->IsValid());
  redis_client_->Type(test_key_);
}

TEST_F(RedisClientTest, Size) {
  ASSERT_TRUE(redis_client_ != NULL)<<"it is the size failed in the second test!";
  ASSERT_TRUE(redis_client_->IsValid());
  redis_client_->Size(test_key_);
}

TEST_F(RedisClientTest, Expire) {
  ASSERT_TRUE(redis_client_ != NULL)<<"it is the expire failed in the thrid test!";
  ASSERT_TRUE(redis_client_->IsValid());
  redis_client_->Expire(test_key_, 8); // 8 secs
}

TEST_F(RedisClientTest, Set) {
  ASSERT_TRUE(redis_client_ != NULL)<<"it is the set failed in the four test!";
  ASSERT_TRUE(redis_client_->IsValid());
  std::string value = "hello_world";
  ASSERT_TRUE(redis_client_->Set(test_key_, value));
}

TEST_F(RedisClientTest, Get) {
  ASSERT_TRUE(redis_client_ != NULL)<<"it is the get failed in the fifth test!";
  ASSERT_TRUE(redis_client_->IsValid());
  std::string value;
  ASSERT_TRUE(redis_client_->Get(test_key_, &value));
  MCE_DEBUG("value:" << value);
}

TEST_F(RedisClientTest, SetList) {
  ASSERT_TRUE(redis_client_ != NULL);
  ASSERT_TRUE(redis_client_->IsValid());
  StringListType list;
  list.push_back("China");
  list.push_back("USA");
  list.push_back("UK");
  ASSERT_TRUE(redis_client_->SetList("mylist", list));
}

TEST_F(RedisClientTest, GetList) {
  ASSERT_TRUE(redis_client_ != NULL);
  ASSERT_TRUE(redis_client_->IsValid());
  StringListType list;
  ASSERT_TRUE(redis_client_->GetList("mylist", &list, 0, 100));
  BOOST_FOREACH(const std::string& item, list) {
    MCE_DEBUG("list item:" << item);
  }
}

TEST_F(RedisClientTest, SetHash) {
  ASSERT_TRUE(redis_client_ != NULL);
  ASSERT_TRUE(redis_client_->IsValid());
  StringMapType hash_value;
  hash_value.insert(std::make_pair("name", "Michael"));
  hash_value.insert(std::make_pair("age", "50"));
  ASSERT_TRUE(redis_client_->SetHash("myhash", hash_value));
}

TEST_F(RedisClientTest, GetHash) {
  ASSERT_TRUE(redis_client_ != NULL);
  ASSERT_TRUE(redis_client_->IsValid());
  StringListType fields;
  fields.push_back("name");
  fields.push_back("sex");
  fields.push_back("age");
  StringMapType hash_value;
  ASSERT_TRUE(redis_client_->GetHash("myhash", fields, &hash_value));
  StringMapType::const_iterator iter = hash_value.begin();
  for (; iter != hash_value.end(); ++iter) {
    MCE_DEBUG("field:" << iter->first << "\t" << "value:" << iter->second);
  }
}

TEST_F(RedisClientTest, Ping) {
  ASSERT_TRUE(redis_client_ != NULL);
  ASSERT_TRUE(redis_client_->IsValid());
  std::string pong = redis_client_->Ping();
  ASSERT_EQ(pong, "PONG");
}



TEST_F(RedisClientTest, MultiGet) {
  ASSERT_TRUE(redis_client_ != NULL);
  ASSERT_TRUE(redis_client_->IsValid());
  StringListType keys;
  keys.push_back("abc");
  keys.push_back("444");
  keys.push_back("555");
  keys.push_back("666");
  keys.push_back("777");
  keys.push_back("888");
  StringMapType values;
  std::map<std::string, bool> result = redis_client_->MultiGet(keys, &values);
  std::map<std::string, bool>::const_iterator iter1 = result.begin();
  StringMapType::const_iterator iter = values.begin();
  while(iter!=values.end() ) {
    MCE_DEBUG("key"<< iter->first << " value:" << iter->second);
    ++iter;
  }
  while(iter1!=result.end() ) {
    MCE_DEBUG("key"<< iter1->first << ":" << iter1->second);
    ++iter1;
  }
}*/

TEST_F(RedisClientTest, GetMissList) {
  StringListType value;
  ASSERT_FALSE(redis_client_->GetList("LJP", &value, 0, 2));
  MCE_DEBUG("redis_client_->GetList('LJP', &value, 0, 2).size="<<value.size());
  for(StringListType::const_iterator iter = value.begin(); iter != value.end(); iter++) {
    MCE_DEBUG(*iter);
  }
}

} // test



