#include "OceCxxAdapter/src/SocialGraphUserPhotoStatsTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

const std::string SocialGraphUserPhotoStatsTripodAdapter::kNamespaceId = "socialgraph_rcd_page";
const std::string SocialGraphUserPhotoStatsTripodAdapter::kCommonBizId = "sg_userphotostats";

SocialGraphUserPhotoStatsTripodAdapter::SocialGraphUserPhotoStatsTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kNamespaceId, kCommonBizId));
}

SocialGraphUserPhotoStatsTripodAdapter::~SocialGraphUserPhotoStatsTripodAdapter() {
  tripod_client_.reset();
}

bool SocialGraphUserPhotoStatsTripodAdapter::Get(const std::string& key, std::string* value) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphUserPhotoStatsTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->GetAndIgnoreMiss(key, value);
}

bool SocialGraphUserPhotoStatsTripodAdapter::Set(const std::string& key, const std::string& value, int expire_time) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphUserPhotoStatsTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Set(key, value, expire_time);
}

bool SocialGraphUserPhotoStatsTripodAdapter::Remove(const std::string& key) {
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphUserPhotoStatsTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  return tripod_client_->Remove(key);
}

std::map<std::string, bool> SocialGraphUserPhotoStatsTripodAdapter::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!tripod_client_) {
    MCE_ERROR("SocialGraphUserPhotoStatsTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return result;
  }
  result = tripod_client_->MultiGetAndIgnoreMiss(keys, values);
  return result;
}

}}// xce::socialgraph


