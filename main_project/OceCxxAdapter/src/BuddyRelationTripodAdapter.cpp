#include "BuddyRelationTripodAdapter.h"
#include <sstream>
#include "LogWrapper.h"

namespace xce {
namespace buddy {
namespace adapter {


static int kFriendDesc = 0x10;
static int kApplyDesc = 0x01;
static int kAppliedDesc = 0x02;
static int kBlockDesc = 0x04;
static int kBlockedDesc = 0x08;


BuddyRelationTripodAdapter::BuddyRelationTripodAdapter() {
  using namespace com::renren::tripod;
  const std::string zk_address = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/TripodRedis";
  const std::string namespace_id = "buddyrelationcache";
  const std::string biz_id = "buddyrelationcache";
  tripod_client_.reset(new TripodClient(zk_address, namespace_id, biz_id));
}

BuddyRelationTripodAdapter::~BuddyRelationTripodAdapter() {
 tripod_client_.reset();
}

bool BuddyRelationTripodAdapter::LoadHashCache(int user_id) {
  if (user_id <= 0) {
    MCE_ERROR("the argument user_id must be a positive integer!!!");
    return false;
  }
  std::ostringstream otem;
  otem << user_id;
  if (tripod_client_) {
    return tripod_client_->LoadHashCache(otem.str());
  }
  MCE_WARN("BuddyRelationTripodAdapter::LoadHashCache user_id:" << otem.str() << "\t tripod_client_ is NULL!");
  return false;
}

BuddyDesc BuddyRelationTripodAdapter::GetRelation(int host_id, int guest_id) {
  if (host_id <= 0 || guest_id <= 0) {
    MCE_ERROR("the argument host_id or guest_id must be a positive integer!!!");
    return NoPath;
  }
  if (host_id == guest_id) {
    return Self;
  }
  bool exchange = host_id > guest_id ? true : false;
  std::string max_id, min_id;
  if (exchange) {
    max_id = boost::lexical_cast<std::string>(host_id);
    min_id = boost::lexical_cast<std::string>(guest_id);
  }
  else {
    max_id = boost::lexical_cast<std::string>(guest_id);
    min_id = boost::lexical_cast<std::string>(host_id);
  }
  StrList fields;
  fields.push_back(max_id);
  if (tripod_client_) {
    HashPair value;
    tripod_client_->GetHash(min_id, fields, value);
    HashPair::const_iterator iter = value.find(max_id);
    if (iter != value.end()) {
      return Parse(iter->second, exchange);
    }
  }
  return NoPath;
}

BuddyDesc BuddyRelationTripodAdapter::Parse(const std::string& field_value, bool exchange) {
  int v = 0;
  BuddyDesc desc = NoPath;
  try {
    v = boost::lexical_cast<int>(field_value);
  } catch (...) { }
  if ((v & kFriendDesc) > 0) {
    desc = Friend;
  }
  if ((v & kApplyDesc) > 0) {
      desc = (exchange != true ? Apply : Applied);
  }
  if ((v & kAppliedDesc) > 0) {
      desc = (exchange == true ? Apply : Applied);
  }
  if ((v & kBlockDesc) > 0) {
      desc = (exchange != true ? Block : Blocked);
  }
  if ((v & kBlockedDesc) > 0) {
      desc = (exchange == true ? Block : Blocked);
  }
  return desc;
}

}}} // xce::buddy::adapter




