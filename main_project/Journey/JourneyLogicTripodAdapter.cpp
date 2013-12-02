#include "Journey/JourneyLogicTripodAdapter.h"
#include "LogWrapper.h"

namespace xce{
namespace journey{

const std::string JourneyLogicCountTripodAdapter::kNamespaceId = "web_ugc";
const std::string JourneyLogicCountTripodAdapter::kBusinessId = "journey_count";
const std::string JourneyLogicListTripodAdapter::kNamespaceId = "web_ugc";
const std::string JourneyLogicListTripodAdapter::kBusinessId = "journey_list";

JourneyLogicCountTripodAdapter::JourneyLogicCountTripodAdapter() {
  count_client_.reset(new xce::tripod::TripodClient(kNamespaceId, kBusinessId)); 
}

JourneyLogicCountTripodAdapter::~JourneyLogicCountTripodAdapter() {
  count_client_.reset();
}

bool JourneyLogicCountTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!count_client_) {
    MCE_ERROR("JourneyLogicCountTripodAdapter::Set count_client_ is null ns: " << kNamespaceId << "\tbiz: "<< kBusinessId);
    return false;
  }
  return count_client_->Set(key, value, expire_time);
}

bool JourneyLogicCountTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!count_client_) {
    MCE_ERROR("JourneyLogicCountTripodAdapter::Get count_client_ is null ns: " << kNamespaceId << "\tbiz: "<< kBusinessId);
    return false;
  }
  return count_client_->GetAndIgnoreMiss(key, value);
}

bool JourneyLogicCountTripodAdapter::Remove(const std::string& key) {
  if (!count_client_) {
    MCE_ERROR("JourneyLogicCountTripodAdapter::Remove count_client_ is null ns: " << kNamespaceId << "\tbiz: "<< kBusinessId);
    return false;
  }
  return count_client_->Remove(key);
}

std::string JourneyLogicCountTripodAdapter::Inc(const std::string& key, const std::string& step) {
  std::string result;
  if (!count_client_) {
    MCE_ERROR("JourneyLogicCountTripodAdapter::Remove count_client_ is null ns: " << kNamespaceId << "\tbiz: "<< kBusinessId);
    return result;
  }
  result = count_client_->Inc(key, step);
  return result;
}

JourneyLogicListTripodAdapter::JourneyLogicListTripodAdapter () {
  list_client_.reset(new xce::tripod::TripodClient(kNamespaceId, kBusinessId));
}

JourneyLogicListTripodAdapter::~JourneyLogicListTripodAdapter() {
  list_client_.reset();
}

bool JourneyLogicListTripodAdapter::GetSortedSet(const std::string& key, StringListType* zset_value, int begin, int limit) {
  if (!list_client_) {
    MCE_ERROR("JourneyLogicListTripodAdapter::GetSortedSet list_client_ is null ns: " << kNamespaceId << "\tbiz: " << kBusinessId);
    return false;
  }
  return list_client_->ZRange(key, zset_value, begin, limit);
}

bool JourneyLogicListTripodAdapter::AddSortedSet(const std::string& key, const StringMapType& set_with_scores, int expire_time) {
  if (!list_client_) {
    MCE_ERROR("JourneyLogicListTripodAdapter::AddSortedSet list_client_ is null ns: " << kNamespaceId << "\tbiz: " << kBusinessId);
    return false;
  }
  if (list_client_->ZAdd(key, set_with_scores)) {
    return true;
  }
  return list_client_->ZSet(key, set_with_scores, expire_time); 
}

bool JourneyLogicListTripodAdapter::TrimSortedSet(const std::string& key, int begin, int end) {
  if (!list_client_) {
    MCE_ERROR("JourneyLogicListTripodAdapter::TrimSortedSet list_client_ is null ns: " << kNamespaceId << "\tbiz: " << kBusinessId);
    return false;
  }
  return list_client_->ZRemRangeByRank(key, begin, end);
}

bool JourneyLogicListTripodAdapter::Remove(const std::string& key) {
  if (!list_client_) {
    MCE_ERROR("JourneyLogicListTripodAdapter::Remove list_client_ is null ns: " << kNamespaceId << "\tbiz: " << kBusinessId);
    return false;
  }
  return list_client_->Remove(key); 
}

}} //xce::journey
