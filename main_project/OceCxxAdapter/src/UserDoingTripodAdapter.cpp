#include "UserDoingTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userdoing {

const int UserDoingTripodAdapter::expiration = 3600 * 24 * 30;

UserDoingInfoPtr UserDoingTripodAdapter::Get(int user_id) {
  UserDoingInfoPtr result = new UserDoingInfo;
  PbUserDoing pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserDoingUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserDoingTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserDoingTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserDoingTripodAdapter::UserDoingTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserDoing>("xce_user", "user_doing"));
}

UserDoingTripodAdapter::~UserDoingTripodAdapter() {
  client_.reset();
}

void UserDoingUtil::parse(const PbUserDoing &data, UserDoingInfoPtr &result) {
  result->set_id(data.id());
  result->set_doing(data.doing());
  result->set_dointTime(data.doingtime());
}

}}} //namespace xce::adapter::userdoing
