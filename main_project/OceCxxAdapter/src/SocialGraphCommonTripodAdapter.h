#ifndef SOCIALGRAPH_COMMON_TRIPOD_ADAPTER_H_ 
#define SOCIALGRAPH_COMMON_TRIPOD_ADAPTER_H_ 

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"

namespace xce {
namespace socialgraph {

using namespace xce::tripod;

class SocialGraphCommonTripodAdapter : public MyUtil::Singleton<SocialGraphCommonTripodAdapter> {
 public:
  virtual ~SocialGraphCommonTripodAdapter();
  bool Get(const std::string& key, std::string* value);
  bool Set(const std::string& key, const std::string& value, int expire_time);
  bool Remove(const std::string& key);
  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values);
 private:
  friend class MyUtil::Singleton<SocialGraphCommonTripodAdapter>;
  SocialGraphCommonTripodAdapter();
  boost::shared_ptr<TripodClient> tripod_client_;
  static const std::string kNamespaceId;
  static const std::string kCommonBizId;
};

}} // xce::socialgraph

#endif
