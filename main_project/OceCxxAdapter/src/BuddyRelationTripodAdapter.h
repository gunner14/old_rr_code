#ifndef BUDDY_RELATION_TRIPOD_ADAPTER_H_
#define BUDDY_RELATION_TRIPOD_ADAPTER_H_

#include <string>
#include "boost/shared_ptr.hpp"
#include "Singleton.h"
#include "TripodClient.h"
#include "BuddyCommon.h"

namespace xce {
namespace buddy {
namespace adapter {

using namespace com::renren::tripod;

class BuddyRelationTripodAdapter : public MyUtil::Singleton<BuddyRelationTripodAdapter> {
 public:
  virtual ~BuddyRelationTripodAdapter();
  bool LoadHashCache(int user_id);
  BuddyDesc GetRelation(int host_id, int guest_id);
  static BuddyDesc Parse(const std::string& field_value, bool exchange);
 private:
  friend class MyUtil::Singleton<BuddyRelationTripodAdapter>;
  BuddyRelationTripodAdapter();
  boost::shared_ptr<TripodClient> tripod_client_;
};

}}} // xce::buddy::adapter
#endif

