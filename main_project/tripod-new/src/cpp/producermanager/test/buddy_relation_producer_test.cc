#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include "boost/foreach.hpp"
#include "base/logging.h"
#include "base/once.h"
#include "QueryRunner.h"
#include "LogWrapper.h"
#include "BuddyRelationCacheProducer.h"

namespace xce {
namespace buddy {

class BuddyRelationCacheTest : public testing::Test {
 protected:
  static void SetUpTestCase() {
    using namespace com::xiaonei::xce;
    ConnectionPoolManager::instance().initialize();
    MCE_DEFAULT_INIT("DEBUG");
  }
  static void TearDownTestCase() {

  }
  void SetUp() {
    test_id_ = 700007661;
  }
 protected:
  int test_id_;
};

// fetch block
TEST_F(BuddyRelationCacheTest, fetch_block_relation) {
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  RelationEntryList block_list;
  Statement sql;
  sql << "SELECT guest FROM relation_block WHERE host = " << test_id_ << " AND host < guest";
  try {
    RelationResultHandler handler(test_id_, "guest", kBlockDesc, block_list);
    QueryRunner(kDbBuddyBlock, CDbRServer).query(sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  //EXPECT_NE(true, block_list.empty());
  BOOST_FOREACH(RelationEntry entry, block_list) {
    //MCE_DEBUG(test_id_ << "\t" << entry.to_id_ << "\t" << entry.relation_desc_);
    ASSERT_LT(test_id_, entry.to_id_);
  }
  MCE_DEBUG("block size:" << block_list.size());
}
// fetch blocked
TEST_F(BuddyRelationCacheTest, fetch_blocked_relation) {
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  RelationEntryList blocked_list;
  Statement sql;
  sql << "SELECT host FROM relation_block WHERE guest = " << test_id_ << " AND host > guest";
  try {
    RelationResultHandler handler(test_id_, "host", kBlockedDesc, blocked_list);
    QueryRunner(kDbBuddyBlock, CDbRServer).query(sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  //EXPECT_NE(true, blocked_list.empty());
  BOOST_FOREACH(RelationEntry entry, blocked_list) {
    //MCE_DEBUG(test_id_ << "\t" << entry.to_id_ << "\t" << entry.relation_desc_);
    ASSERT_LT(test_id_, entry.to_id_);
  }
  MCE_DEBUG("blocked size:" << blocked_list.size());
}
// fetch friend
TEST_F(BuddyRelationCacheTest, fetch_friend_relation) {
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  RelationEntryList friend_list;
  Statement sql;
  sql << "SELECT guest FROM relation_" << test_id_ % 100 << " WHERE host = " << test_id_ << " AND host < guest AND disc = 0";
  try {
    std::ostringstream pattern;
    pattern << "relation_" << test_id_ % 100;
    RelationResultHandler handler(test_id_, "guest", kFriendDesc, friend_list);
    QueryRunner(kDbBuddyFriend, CDbRServer, pattern.str()).query(sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  //EXPECT_NE(true, friend_list.empty());
  BOOST_FOREACH(RelationEntry entry, friend_list) {
    //MCE_DEBUG(test_id_ << "\t" << entry.to_id_ << "\t" << entry.relation_desc_);
    ASSERT_LT(test_id_, entry.to_id_);
  }
  MCE_DEBUG("friend size:" << friend_list.size());
}
// fetch applied
TEST_F(BuddyRelationCacheTest, fetch_applied_relation) {
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  RelationEntryList applied_list;
  Statement sql;
  sql << "SELECT applicant FROM buddyapply_accepter_" << test_id_ % 100 << " WHERE accepter = " << test_id_ << " AND accepter < applicant";
  try {
    std::ostringstream pattern;
    pattern << "buddyapply_accepter_" << test_id_ % 100;
    RelationResultHandler handler(test_id_, "applicant", kAppliedDesc, applied_list);
    QueryRunner(kDbBuddyAccepter, CDbRServer, pattern.str()).query(sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  //EXPECT_NE(true, applied_list.empty());
  BOOST_FOREACH(RelationEntry entry, applied_list) {
 //   MCE_DEBUG(test_id_ << "\t" << entry.to_id_ << "\t" << entry.relation_desc_);
    ASSERT_LT(test_id_, entry.to_id_);
  }
  MCE_DEBUG("applied size:" << applied_list.size());
}
// fetch apply
TEST_F(BuddyRelationCacheTest, fetch_apply_relation) {
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  RelationEntryList apply_list;
  Statement sql;
  sql << "SELECT accepter FROM buddyapply_applicant_" << test_id_ % 100 << " WHERE applicant = " << test_id_ << " AND applicant < accepter";
  try {
    std::ostringstream pattern;
    pattern << "buddyapply_applicant_" << test_id_ % 100;
    RelationResultHandler handler(test_id_, "accepter", kAppliedDesc, apply_list);
    QueryRunner(kDbBuddyApplicant, CDbRServer, pattern.str()).query(sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  //EXPECT_NE(true, apply_list.empty());
  BOOST_FOREACH(RelationEntry entry, apply_list) {
    //MCE_DEBUG(test_id_ << "\t" << entry.to_id_ << "\t" << entry.relation_desc_);
    ASSERT_LT(test_id_, entry.to_id_);
  }
  MCE_DEBUG("apply size:" << apply_list.size());
}
}} //xce::buddy

