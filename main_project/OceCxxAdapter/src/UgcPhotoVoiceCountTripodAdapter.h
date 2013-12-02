#ifndef UGC_PHOTO_VOICE_COUNT_TRIPOD_ADAPTER_H_ 
#define UGC_PHOTO_VOICE_COUNT_TRIPOD_ADAPTER_H_ 

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"

namespace xce {
namespace ugc {

using namespace xce::tripod;

class UgcPhotoVoiceCountTripodAdapter : public MyUtil::Singleton<UgcPhotoVoiceCountTripodAdapter> {
 public:
  virtual ~UgcPhotoVoiceCountTripodAdapter();
  bool Get(const std::string& key, long* value);
 private:
  friend class MyUtil::Singleton<UgcPhotoVoiceCountTripodAdapter>;
  UgcPhotoVoiceCountTripodAdapter();
  boost::shared_ptr<TripodClient> tripod_client_;
  static const std::string kNamespaceId;
  static const std::string kCommonBizId;
};

}} // xce::socialgraph

#endif

