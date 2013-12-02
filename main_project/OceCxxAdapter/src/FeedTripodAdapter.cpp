#include "OceCxxAdapter/src/FeedTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace feed {

const std::string FeedTripodAdapter::kNamespaceId = "feed";
const std::string FeedTripodAdapter::kCommonBizId = "fdb_test";

FeedTripodAdapter::FeedTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kNamespaceId, kCommonBizId));
}

FeedTripodAdapter::~FeedTripodAdapter() {
  tripod_client_.reset();
}

bool FeedTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("FeedTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool FeedTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("FeedTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool FeedTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("FeedTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> FeedTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("FeedTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


