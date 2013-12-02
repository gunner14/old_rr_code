#include "BuddyRelationCacheProducer.h"
#include "LogWrapper.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "util/cpp/TimeCost.h"

namespace xce {
namespace buddy {



RelationEntry::RelationEntry(int to, const std::string& desc) : to_id_(to), relation_desc_(desc) {

}

RelationEntry::~RelationEntry() {

}

RelationResultHandler::RelationResultHandler(int user_id, const std::string& table_field, const std::string& relation_desc, RelationEntryList& relation_entry_list): user_id_(user_id), table_field_(table_field), relation_desc_(relation_desc), relation_entry_list_(relation_entry_list) {

}

bool RelationResultHandler::handle(mysqlpp::Row& row) const {
  int to_id = (int) row[table_field_.c_str()];
  relation_entry_list_.push_back(RelationEntry(to_id, relation_desc_)); 
  return true;
}

BuddyRelationCacheProducer::BuddyRelationCacheProducer() {

}

BuddyRelationCacheProducer::~BuddyRelationCacheProducer() {

}


com::renren::tripod::DataMap BuddyRelationCacheProducer::create(const com::renren::tripod::KeySeq& keys, bool is_master) {
  com::renren::tripod::DataMap result;
  BOOST_FOREACH(const std::string& key, keys) {
    com::renren::tripod::Data data;
    bool ret = CreateById(key, data, is_master);
    if (ret) {
      result.insert(std::make_pair(key, data));
    }
  }
  return result;
}

com::renren::tripod::KeySeq BuddyRelationCacheProducer::createHotKeys(const std::string& begin_key, int limit) {
  return com::renren::tripod::KeySeq();
}

com::renren::tripod::KeySeq BuddyRelationCacheProducer::createKeys(const std::string& begin_key, int limit) {
  return com::renren::tripod::KeySeq();
}

bool BuddyRelationCacheProducer::CreateById(const std::string& key, com::renren::tripod::Data& data, bool is_master) {
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  std::ostringstream otem;
  otem << "BuddyRelationCacheProducer::CreateById() key:" << key << "\tis_master:" << is_master;
  TimeCost tc = TimeCost::create(otem.str(), LOG_LEVEL_INFO);
  //MCE_INFO("BuddyRelationCacheProducer::CreateById() key:" << key << "\tis_master:" << is_master);
  int user_id = atoi(key.c_str());
  if (user_id <= 0) {
    return false;
  }
  com::renren::tripod::ListOrHashValue hash_value;
  // fetch block
  RelationEntryList block_list;
  Statement block_sql;
  block_sql << "SELECT guest FROM relation_block WHERE host = " << user_id << " AND host < guest";
  try {
    RelationResultHandler handler(user_id, "guest", kBlockDesc, block_list);
    QueryRunner(kDbBuddyBlock, CDbRServer).query(block_sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  Convert(hash_value, block_list);
  // fetch blocked
  RelationEntryList blocked_list;
  Statement blocked_sql;
  blocked_sql << "SELECT host FROM relation_block WHERE guest = " << user_id << " AND host > guest";
  try {
    RelationResultHandler handler(user_id, "host", kBlockedDesc, blocked_list);
    QueryRunner(kDbBuddyBlock, CDbRServer).query(blocked_sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  Convert(hash_value, blocked_list);
  // fetch friend
  RelationEntryList friend_list;
  Statement friend_sql;
  friend_sql << "SELECT guest FROM relation_" << user_id % 100 << " WHERE host = " << user_id << " AND host < guest AND disc = 0";
  try {
    std::ostringstream pattern;
    pattern << "relation_" << user_id % 100;
    RelationResultHandler handler(user_id, "guest", kFriendDesc, friend_list);
    QueryRunner(kDbBuddyFriend, CDbRServer, pattern.str()).query(friend_sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  Convert(hash_value, friend_list);
  // fetch applied
  RelationEntryList applied_list;
  Statement applied_sql;
  applied_sql << "SELECT applicant FROM buddyapply_accepter_" << user_id % 100 << " WHERE accepter = " << user_id << " AND accepter < applicant";
  try {
    std::ostringstream pattern;
    pattern << "buddyapply_accepter_" << user_id % 100;
    RelationResultHandler handler(user_id, "applicant", kAppliedDesc, applied_list);
    QueryRunner(kDbBuddyAccepter, CDbRServer, pattern.str()).query(applied_sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  Convert(hash_value, applied_list);
  // fetch apply
  RelationEntryList apply_list;
  Statement apply_sql;
  apply_sql << "SELECT accepter FROM buddyapply_applicant_" << user_id % 100 << " WHERE applicant = " << user_id << " AND applicant < accepter";
  try {
    std::ostringstream pattern;
    pattern << "buddyapply_applicant_" << user_id % 100;
    RelationResultHandler handler(user_id, "accepter", kApplyDesc, apply_list);
    QueryRunner(kDbBuddyApplicant, CDbRServer, pattern.str()).query(apply_sql, handler);
  } catch (std::exception& e) {
    MCE_ERROR(e.what());
  }
  Convert(hash_value, apply_list);
  int size = hash_value.ByteSize();
  MCE_DEBUG("key:" << key << "\thash_value.Bytesize():" << hash_value.ByteSize());
  std::string s;
  hash_value.SerializeToString(&s);
  data.reserve(s.size());
  data.assign(s.data(), s.data() + s.size());
  return true;
}

void BuddyRelationCacheProducer::Convert(com::renren::tripod::ListOrHashValue& result, RelationEntryList& list) {
  if (list.empty()) {
    return;
  }
  BOOST_FOREACH(const RelationEntry& entry, list) {
    std::string to_id_string;
    try {
      to_id_string = boost::lexical_cast<std::string>(entry.to_id_);
      result.add_values(to_id_string);
      result.add_values(entry.relation_desc_);
    } catch (...) {
      continue;
    }
  }
}

}} // xce::buddy

