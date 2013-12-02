#include "OceCxxAdapter/src/UgcPhotoVoiceCountTripodAdapter.h"
#include "LogWrapper.h"

namespace xce {
namespace ugc {

const std::string UgcPhotoVoiceCountTripodAdapter::kNamespaceId = "web_ugc";
const std::string UgcPhotoVoiceCountTripodAdapter::kCommonBizId = "photo_voice_count";

UgcPhotoVoiceCountTripodAdapter::UgcPhotoVoiceCountTripodAdapter() {
  tripod_client_.reset(new xce::tripod::TripodClient(kNamespaceId, kCommonBizId));
}

UgcPhotoVoiceCountTripodAdapter::~UgcPhotoVoiceCountTripodAdapter() {
  tripod_client_.reset();
}

bool UgcPhotoVoiceCountTripodAdapter::Get(const std::string& key, long* value) {
  if (!tripod_client_) {
    MCE_ERROR("UgcPhotoVoiceCountTripodAdapter::Get tripod_client is NULL! ns:" << kNamespaceId << "\tbiz:" << kCommonBizId);
    return false;
  }
  std::string str_value;
  bool result = tripod_client_->GetAndIgnoreMiss(key, &str_value);
  if (!result || str_value.length() == 0 ) {
    return false;
  }
  try {
    *value = boost::lexical_cast<long>(str_value);
  } catch(...) {
    MCE_ERROR("UgcPhotoVoiceCountTripodAdapter::Get lexical_cast exception"  );
    return false;
  }
  return result;
}

}}// xce::socialgraph


