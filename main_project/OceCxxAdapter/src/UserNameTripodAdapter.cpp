#include "UserNameTripodAdapter.h"

namespace xce {
namespace adapter {
namespace username {

const int UserNameTripodAdapter::expiration = 3600 * 24 * 30;

UserNameInfoPtr UserNameTripodAdapter::Get(int user_id) {
  UserNameInfoPtr result = new UserNameInfo;
  PbUserName pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserNameUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserNameTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserNameTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserNameTripodAdapter::UserNameTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserName>("xce_user", "user_name"));
}

UserNameTripodAdapter::~UserNameTripodAdapter() {
  client_.reset();
}

void UserNameUtil::parse(const PbUserName &data, UserNameInfoPtr &result) {
  result->set_id(data.id());
  result->set_name(data.name());
  result->set_nickname(data.nickname());
}

}}} //namespace xce::adapter::username
