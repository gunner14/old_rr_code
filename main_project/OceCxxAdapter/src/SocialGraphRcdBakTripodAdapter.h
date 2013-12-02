#ifndef SOCIALGRAPH_RCD_BAK_TRIPOD_ADAPTER_H_ 
#define SOCIALGRAPH_RCD_BAK_TRIPOD_ADAPTER_H_ 

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"

namespace xce {
namespace socialgraph {

using namespace xce::tripod;

class SocialGraphRcdBakTripodAdapter : public MyUtil::Singleton<SocialGraphRcdBakTripodAdapter> {
 public:
  virtual ~SocialGraphRcdBakTripodAdapter();
  bool Get(const std::string& key, std::string* value);
  bool Set(const std::string& key, const std::string& value, int expire_time);
  bool Remove(const std::string& key);
  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values);
 private:
  friend class MyUtil::Singleton<SocialGraphRcdBakTripodAdapter>;
  SocialGraphRcdBakTripodAdapter();
  boost::shared_ptr<TripodClient> tripod_client_;
  static const std::string kSgRcdNamespaceId;
  static const std::string kSgRcdBizId;
};

}} // xce::socialgraph

#endif

