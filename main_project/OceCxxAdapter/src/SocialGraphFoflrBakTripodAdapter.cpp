#include "SocialGraphFoflrBakTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphFoflrBakTripodAdapter::kSgFoflrNamespaceId = "ad_socialgraph_bak";
const std::string SocialGraphFoflrBakTripodAdapter::kSgFoflrBizId = "socialgraph_foflr";

SocialGraphFoflrBakTripodAdapter::SocialGraphFoflrBakTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kSgFoflrNamespaceId, kSgFoflrBizId));
}

SocialGraphFoflrBakTripodAdapter::~SocialGraphFoflrBakTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphFoflrBakTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrBakTripodAdapter::Get tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphFoflrBakTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrBakTripodAdapter::Set tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphFoflrBakTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrBakTripodAdapter::Remove tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphFoflrBakTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFoflrBakTripodAdapter::MultiGet tripod_client is NULL! ns:" << kSgFoflrNamespaceId << "\tbiz:" << kSgFoflrBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


