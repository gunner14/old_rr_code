#ifndef SOCIALGRAPH_FFUCR_TRIPOD_ADAPTER_H_ 
#define SOCIALGRAPH_FFUCR_TRIPOD_ADAPTER_H_ 

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"

namespace xce {
namespace socialgraph {

using namespace xce::tripod;

class SocialGraphFFUCRTripodAdapter : public MyUtil::Singleton<SocialGraphFFUCRTripodAdapter> {
 public:
  virtual ~SocialGraphFFUCRTripodAdapter();
  bool Get(const std::string& key, std::string* value);
  bool Set(const std::string& key, const std::string& value, int expire_time);
  bool Remove(const std::string& key);
  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values);
 private:
  friend class MyUtil::Singleton<SocialGraphFFUCRTripodAdapter>;
  SocialGraphFFUCRTripodAdapter();
  boost::shared_ptr<TripodClient> tripod_client_;
  static const std::string kSgRcdNamespaceId;
  static const std::string kSgRcdBizId;
};

}} // xce::socialgraph

#endif

