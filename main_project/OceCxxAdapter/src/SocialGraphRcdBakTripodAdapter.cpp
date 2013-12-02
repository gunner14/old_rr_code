#include "SocialGraphRcdBakTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphRcdBakTripodAdapter::kSgRcdNamespaceId = "ad_socialgraph_bak";
const std::string SocialGraphRcdBakTripodAdapter::kSgRcdBizId = "socialgraph_rcd";

SocialGraphRcdBakTripodAdapter::SocialGraphRcdBakTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kSgRcdNamespaceId, kSgRcdBizId));
}

SocialGraphRcdBakTripodAdapter::~SocialGraphRcdBakTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphRcdBakTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdBakTripodAdapter::Get tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphRcdBakTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdBakTripodAdapter::Set tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphRcdBakTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdBakTripodAdapter::Remove tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphRcdBakTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdBakTripodAdapter::MultiGet tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


