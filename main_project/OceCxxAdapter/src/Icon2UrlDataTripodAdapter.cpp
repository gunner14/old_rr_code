#include "Icon2UrlDataTripodAdapter.h"

namespace xce {
namespace adapter {
namespace icon2urldata {

bool Icon2UrlDataTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, 0, is_master); 
}

bool Icon2UrlDataTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

Icon2UrlDataTripodAdapter::Icon2UrlDataTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbVipIconUrl>("xce_misc", "vip_icon_url"));
}

Icon2UrlDataTripodAdapter::~Icon2UrlDataTripodAdapter() {
  client_.reset();
}

}}} //namespace xce::adapter::icon2urldata
