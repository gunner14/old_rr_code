#include "SocialGraphFoflrTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphFoflrTripodAdapter::kSgFoflrNamespaceId = "ad_socialgraph";
const std::string SocialGraphFoflrTripodAdapter::kSgFoflrBizId = "socialgraph_foflr";

SocialGraphFoflrTripodAdapter::SocialGraphFoflrTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kSgFoflrNamespaceId, kSgFoflrBizId));
}

SocialGraphFoflrTripodAdapter::~SocialGraphFoflrTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphFoflrTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrTripodAdapter::Get tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphFoflrTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrTripodAdapter::Set tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphFoflrTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrTripodAdapter::Remove tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphFoflrTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrTripodAdapter::MultiGet tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


