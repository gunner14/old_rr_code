#include "UserStateTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userstate {

const int UserStateTripodAdapter::expiration = 3600 * 24 * 30;

UserStateInfoPtr UserStateTripodAdapter::Get(int user_id) {
  UserStateInfoPtr result = new UserStateInfo;
  PbUserState pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserStateUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserStateTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserStateTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserStateTripodAdapter::UserStateTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserState>("xce_user", "user_state"));
}

UserStateTripodAdapter::~UserStateTripodAdapter() {
  client_.reset();
}

void UserStateUtil::parse(const PbUserState &data, UserStateInfoPtr &result) {
  result->set_id(data.id());
  result->set_state(data.state());
  result->set_level(data.level());
  result->set_star(data.star());
}

}}} //namespace xce::adapter::userstate
