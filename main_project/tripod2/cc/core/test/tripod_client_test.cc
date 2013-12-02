#include "gtest/gtest.h"

#include "base/port.h"

#include "tripod2/cc/core/tripod_client.h"
#include "tripod2/cc/core/cache_proxy.h"

#include "LogWrapper.h"
#include "util/cpp/TimeCost.h"

namespace test {

using namespace xce::tripod;
using namespace boost;

class TripodClientTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("INFO");
    //test_namespace_ = "xce_test";
    test_namespace_ = "ljp_namespace";
    test_biz_ = "test_biz";
    tripod_client_ = new TripodClient(test_namespace_, test_biz_);
    tripod_settest_client_ = new TripodClient("ljp_namespace", "testzset");
  }
  static void TearDownTestCase() {
    if (tripod_client_) {
      delete tripod_client_;
      tripod_client_ = 0;
    }
    if (tripod_settest_client_) {
      delete tripod_settest_client_;
      tripod_settest_client_ = 0;
    }
  }
  static TripodClient* tripod_client_;
  static TripodClient* tripod_settest_client_;
  static std::string test_namespace_;
  static std::string test_biz_;
};

TripodClient* TripodClientTest::tripod_client_;
TripodClient* TripodClientTest::tripod_settest_client_;
std::string TripodClientTest::test_namespace_;
std::string TripodClientTest::test_biz_;

/*TEST_F(TripodClientTest, Zset) {
using namespace std;
using namespace boost;
  StringMapType set_with_scores;
  for (int i = 0; i < 100; i ++) {
    set_with_scores.insert(make_pair(lexical_cast<string>(i), lexical_cast<string>(i)));
  }
  {
    tripod_settest_client_->ZSet("testzsetkey", set_with_scores, 600);
    MyUtil::Clock clock;
    tripod_settest_client_->ZSet("testzsetkey", set_with_scores, 600);
    MCE_WARN("zset time cost " << clock.total() << " us"); 
  }
  {
    StringListType zsetValue;
    tripod_settest_client_->ZRange("testzsetkey", &zsetValue, 0, -1);
    MyUtil::Clock clock;
    tripod_settest_client_->ZRange("testzsetkey", &zsetValue, 0, -1);
    MCE_WARN("zrange time cost " << clock.total() << " us");
    ostringstream otem;
    otem << "Value is : { ";
    for (StringListType::const_iterator iter= zsetValue.begin(); iter != zsetValue.end(); iter++) {
      otem << *iter << " ";
    }  
    otem << "}";
    MCE_INFO(otem.str());
  }
}*/

TEST_F(TripodClientTest, LinSet) {
  ASSERT_TRUE(tripod_client_->Set("1357986", "value1", 6000));
  ASSERT_TRUE(tripod_client_->Set("1357987", "value1", 6000));
  ASSERT_TRUE(tripod_client_->Set("1357988", "value1", 6000));
  ASSERT_TRUE(tripod_client_->Set("1357989", "value1", 6000));
}

/*TEST_F(TripodClientTest, AAA) {
 {
   tripod_client_->Set("key1","value1");
   MyUtil::Clock clock;
   tripod_client_->Set("key1","value1");
   MCE_WARN("set time cost  "  << clock.total() << " us");
 }
 {
   std::string val;
   tripod_client_->GetAndIgnoreMiss("key1",&val);
   MyUtil::Clock clock;
   tripod_client_->GetAndIgnoreMiss("key1",&val);
   MCE_WARN("get time cost  "  << clock.total() << " us");
   MCE_INFO(val);
 }
 {
   const int count = 300;
   for(int i=1;i < count; ++i ) {
     std::string key = "key" + boost::lexical_cast<std::string>(i);
     std::string value = "valueaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"+boost::lexical_cast<std::string>(i) +
    "valueaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"+boost::lexical_cast<std::string>(i) +
    "valueaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"+boost::lexical_cast<std::string>(i);
     tripod_client_->Set(key,value);
   }
   StringListType keys;
   StringMapType values;
   for(int i=1;i < count; ++i ) {
     std::string key = "key" + boost::lexical_cast<std::string>(i);
     keys.push_back(key);
   }
   std::map<std::string, bool> result;
   result = tripod_client_->MultiGetAndIgnoreMiss(keys,&values);
   MyUtil::Clock clock;
   result = tripod_client_->MultiGetAndIgnoreMiss(keys,&values);
   MCE_WARN("multiget " << count <<" time cost  "  << clock.total() << " us");
   for(int i=1;i < count; ++i ) {
     std::string key = "key" + boost::lexical_cast<std::string>(i);
     MCE_DEBUG(result[key]);
     MCE_DEBUG(values[key]);
   }
 }
}*/


} // test


