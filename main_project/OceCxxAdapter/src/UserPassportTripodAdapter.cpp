#include "UserPassportTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userpassport {

const int UserPassportTripodAdapter::expiration = 3600 * 24 * 30;

UserPassportInfoPtr UserPassportTripodAdapter::Get(int user_id) {
  UserPassportInfoPtr result = new UserPassportInfo;
  PbUserPassport pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserPassportUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserPassportTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserPassportTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserPassportTripodAdapter::UserPassportTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserPassport>("xce_user", "user_passport"));
}

UserPassportTripodAdapter::~UserPassportTripodAdapter() {
  client_.reset();
}

void UserPassportUtil::parse(const PbUserPassport &data, UserPassportInfoPtr &result) {
  result->set_id(data.id());
  result->set_status(data.status());
  result->set_safestatus(data.safe_status());
  result->set_account(data.account());
  result->set_domain(data.domain());
  result->set_passwordmd5(data.password_md5());
}

}}} //namespace xce::adapter::userpassport
