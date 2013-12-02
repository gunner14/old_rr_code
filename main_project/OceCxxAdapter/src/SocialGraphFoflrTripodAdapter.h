#ifndef SOCIALGRAPH_FOFLR_TRIPOD_ADAPTER_H_ 
#define SOCIALGRAPH_FOFLR_TRIPOD_ADAPTER_H_ 

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"

namespace xce {
namespace socialgraph {

using namespace xce::tripod;

class SocialGraphFoflrTripodAdapter : public MyUtil::Singleton<SocialGraphFoflrTripodAdapter> {
 public:
  virtual ~SocialGraphFoflrTripodAdapter();
  bool Get(const std::string& key, std::string* value);
  bool Set(const std::string& key, const std::string& value, int expire_time);
  bool Remove(const std::string& key);
  std::map<std::string, bool> MultiGet(const StringListType& keys, StringMapType* values);
 private:
  friend class MyUtil::Singleton<SocialGraphFoflrTripodAdapter>;
  SocialGraphFoflrTripodAdapter();
  boost::shared_ptr<TripodClient> tripod_client_;
  static const std::string kSgFoflrNamespaceId;
  static const std::string kSgFoflrBizId;
};

}} // xce::socialgraph

#endif

