#include "SocialGraphRcdTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphRcdTripodAdapter::kSgRcdNamespaceId = "ad_socialgraph";
const std::string SocialGraphRcdTripodAdapter::kSgRcdBizId = "socialgraph_rcd";

SocialGraphRcdTripodAdapter::SocialGraphRcdTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kSgRcdNamespaceId, kSgRcdBizId));
}

SocialGraphRcdTripodAdapter::~SocialGraphRcdTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphRcdTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdTripodAdapter::Get tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphRcdTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdTripodAdapter::Set tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphRcdTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdTripodAdapter::Remove tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphRcdTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdTripodAdapter::MultiGet tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


