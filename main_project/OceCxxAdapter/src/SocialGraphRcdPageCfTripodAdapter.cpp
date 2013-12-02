#include "OceCxxAdapter/src/SocialGraphRcdPageCfTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphRcdPageCfTripodAdapter::kNamespaceId = "socialgraph_rcd_page";
const std::string SocialGraphRcdPageCfTripodAdapter::kCommonBizId = "sg_rcdpagecf";

SocialGraphRcdPageCfTripodAdapter::SocialGraphRcdPageCfTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kNamespaceId, kCommonBizId));
}

SocialGraphRcdPageCfTripodAdapter::~SocialGraphRcdPageCfTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphRcdPageCfTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdPageCfTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphRcdPageCfTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdPageCfTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphRcdPageCfTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdPageCfTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphRcdPageCfTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphRcdPageCfTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


