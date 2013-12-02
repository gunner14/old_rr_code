#include "VipMemberDataTripodAdapter.h"

namespace xce {
namespace adapter {
namespace vipmemberdata {

bool VipMemberDataTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, 0, is_master); 
}

bool VipMemberDataTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

VipMemberDataTripodAdapter::VipMemberDataTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbVipMemberInfo>("xce_misc", "vip_member_info"));
}

VipMemberDataTripodAdapter::~VipMemberDataTripodAdapter() {
  client_.reset();
}

}}} //namespace xce::adapter::vipmemberdata
