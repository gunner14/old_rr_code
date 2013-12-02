#include "UserUrlTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userurl {

const int UserUrlTripodAdapter::expiration = 3600 * 24 * 30;

UserUrlInfoPtr UserUrlTripodAdapter::Get(int user_id) {
  UserUrlInfoPtr result = new UserUrlInfo;
  PbUserUrl pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserUrlUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserUrlTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserUrlTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserUrlTripodAdapter::UserUrlTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserUrl>("xce_user", "user_url"));
}

UserUrlTripodAdapter::~UserUrlTripodAdapter() {
  client_.reset();
}

void UserUrlUtil::parse(const PbUserUrl &data, UserUrlInfoPtr &result) {
  result->set_id(data.id());
  result->set_largeUrl(data.largeurl());
  result->set_mainUrl(data.mainurl());
  result->set_headUrl(data.headurl());
  result->set_tinyUrl(data.tinyurl());
  result->set_flexUrl(data.flexurl());
}

}}} //namespace xce::adapter::userurl
