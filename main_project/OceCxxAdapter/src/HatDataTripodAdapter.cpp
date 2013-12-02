#include "HatDataTripodAdapter.h"

namespace xce {
namespace adapter {
namespace hatdata {

bool HatDataTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, 0, is_master); 
}

bool HatDataTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

HatDataTripodAdapter::HatDataTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbVipIconUrl>("xce_misc", "vip_hat_url"));
}

HatDataTripodAdapter::~HatDataTripodAdapter() {
  client_.reset();
}

}}} //namespace xce::adapter::hatdata
