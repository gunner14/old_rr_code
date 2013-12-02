#include "UserRightTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userright {

const int UserRightTripodAdapter::expiration = 3600 * 24 * 30;

UserRightInfoPtr UserRightTripodAdapter::Get(int user_id) {
  UserRightInfoPtr result = new UserRightInfo;
  PbUserRight pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserRightUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserRightTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserRightTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserRightTripodAdapter::UserRightTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserRight>("xce_user", "user_right"));
}

UserRightTripodAdapter::~UserRightTripodAdapter() {
  client_.reset();
}

void UserRightUtil::parse(const PbUserRight &data, UserRightInfoPtr &result) {
  result->set_id(data.id());
  result->set_auth(data.auth());
  result->set_rights(data.common_rights());
}

}}} //namespace xce::adapter::userright
