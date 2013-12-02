#ifndef BUDDY_RELATION_CACHE_PRODUCER_H_
#define BUDDY_RELATION_CACHE_PRODUCER_H_

#include "TripodProducer.h"
#include <string>
#include <list>
#include "QueryRunner.h"
#include "listorhashvalue.pb.h"

namespace xce {
namespace buddy {


const static std::string kFriendDesc = "16";
const static std::string kApplyDesc = "1";
const static std::string kAppliedDesc = "2";
const static std::string kBlockDesc = "4";
const static std::string kBlockedDesc = "8";

const static std::string kDbBuddyFriend = "buddycore_friend";
const static std::string kDbBuddyApplicant = "buddyrelation_applicant_preloader";
const static std::string kDbBuddyAccepter = "buddyrelation_accepter_preloader";
const static std::string kDbBuddyBlock = "buddycore_block";

struct RelationEntry {
  int to_id_;
  std::string relation_desc_;
  RelationEntry(int to, const std::string& desc);
  virtual ~RelationEntry();
};

typedef std::list<RelationEntry> RelationEntryList;

class RelationResultHandler : public com::xiaonei::xce::ResultHandler {
 public:
  RelationResultHandler(int user_id, const std::string& table_field,
                        const std::string& relation_desc, RelationEntryList& relation_entry_list);
  virtual bool handle(mysqlpp::Row& row) const;
 private:
  int user_id_;
  std::string table_field_;
  std::string relation_desc_;
  RelationEntryList& relation_entry_list_;
};

class BuddyRelationCacheProducer : virtual public com::renren::tripod::TripodProducer {
 public:
  BuddyRelationCacheProducer();
  virtual ~BuddyRelationCacheProducer();
  com::renren::tripod::DataMap create(const com::renren::tripod::KeySeq& keys, bool is_master);
  com::renren::tripod::KeySeq createHotKeys(const std::string& begin_key, int limit);
  com::renren::tripod::KeySeq createKeys(const std::string& begin_key, int limit);

 private:
  void Convert(com::renren::tripod::ListOrHashValue& result, RelationEntryList& list);
  // key is userid string
  bool CreateById(const std::string& key, com::renren::tripod::Data& data, bool is_master);

};

}} // xce::buddy
#endif

