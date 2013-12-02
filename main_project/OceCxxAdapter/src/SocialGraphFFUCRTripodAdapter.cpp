#include "SocialGraphFFUCRTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphFFUCRTripodAdapter::kSgRcdNamespaceId = "socialgraph_ffucr";
const std::string SocialGraphFFUCRTripodAdapter::kSgRcdBizId = "sg_friend_cluster";

SocialGraphFFUCRTripodAdapter::SocialGraphFFUCRTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kSgRcdNamespaceId, kSgRcdBizId));
}

SocialGraphFFUCRTripodAdapter::~SocialGraphFFUCRTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphFFUCRTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFFUCRTripodAdapter::Get tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphFFUCRTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFFUCRTripodAdapter::Set tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphFFUCRTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFFUCRTripodAdapter::Remove tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphFFUCRTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphFFUCRTripodAdapter::MultiGet tripod_client is NULL! ns:" << kSgRcdNamespaceId << "\tbiz:" << kSgRcdBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


