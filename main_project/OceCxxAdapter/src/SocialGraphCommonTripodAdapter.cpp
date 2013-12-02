#include "OceCxxAdapter/src/SocialGraphCommonTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphCommonTripodAdapter::kNamespaceId = "socialgraph_common";
const std::string SocialGraphCommonTripodAdapter::kCommonBizId = "sg_common";

SocialGraphCommonTripodAdapter::SocialGraphCommonTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kNamespaceId, kCommonBizId));
}

SocialGraphCommonTripodAdapter::~SocialGraphCommonTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphCommonTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphCommonTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphCommonTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphCommonTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphCommonTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphCommonTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphCommonTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphCommonTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


