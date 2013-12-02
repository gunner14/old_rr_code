#include "gtest/gtest.h"
#include <string>
#include <list>
#include "boost/foreach.hpp"
#include "base/logging.h"
#include "LogWrapper.h"

#include "TripodClient.h"

namespace com {
namespace renren {
namespace tripod {

const static std::string kZkAddress ="UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/TripodRedis";

static std::string test_id_string = "286162347"; // test_id
static std::string test_biz_id = "buddyrelationcache";
static std::string test_namespace_id = "buddyrelationcache";

class TripodClientTest : public testing::Test {
 protected:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
};

/*
TEST_F(TripodClientTest, hash_reload) {
  TripodClient tripod_client(kZkAddress, test_namespace_id, test_biz_id);
  std::list<std::string> test_id_list;
  test_id_list.push_back("286162347");
  test_id_list.push_back("27640369");
  test_id_list.push_back("72139581");
  test_id_list.push_back("259282972");
  test_id_list.push_back("288690501");
  test_id_list.push_back("87869320");
  test_id_list.push_back("268396330");
  BOOST_FOREACH(const std::string& key, test_id_list) {
    bool ret = tripod_client.LoadHashCache(key);
    MCE_DEBUG("ret:" << ret);
    ASSERT_TRUE(ret);
  }
}

TEST_F(TripodClientTest, hash_get) {
  TripodClient tripod_client(kZkAddress, test_namespace_id, test_biz_id);
  StrList fields;
  fields.push_back("268396330");
  HashPair hp;
  std::string key = "27640369";
  tripod_client.GetHash(key, fields, hp);
  HashPair::const_iterator iter = hp.begin();
  for (;iter != hp.end(); ++iter) {
    MCE_DEBUG("key:" << key << "field:" << iter->first << "\tvalue:" << iter->second);
  }

}
*/

TEST_F(TripodClientTest, Exists) {
  TripodClient tripod_client(kZkAddress, test_namespace_id, test_biz_id);
  std::string key = "162248114";
  bool exists = tripod_client.Exists(key);
  MCE_DEBUG("key:" << key << "\texists:" << exists);
}


}}} // com::renren::tripod

