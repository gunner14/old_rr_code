#include "UserBornTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userborn {

const int UserBornTripodAdapter::expiration = 3600 * 24 * 30;

UserBornInfoPtr UserBornTripodAdapter::Get(int user_id) {
  UserBornInfoPtr result = new UserBornInfo;
  PbUserBorn pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserBornUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserBornTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserBornTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserBornTripodAdapter::UserBornTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserBorn>("xce_user", "user_born"));
}

UserBornTripodAdapter::~UserBornTripodAdapter() {
  client_.reset();
}

void UserBornUtil::parse(const PbUserBorn &data, UserBornInfoPtr &result) {
  result->set_astrology(data.astrology());
  result->set_birthDay(data.birth_day());
  result->set_birthMonth(data.birth_month());
  result->set_birthYear(data.birth_year());
  result->set_cityCode(data.city_code());
  result->set_gender(data.gender());
  result->set_homeCity(data.home_city());
  result->set_homeProvince(data.home_province());
  result->set_id(data.id());
}

}}} //namespace xce::adapter::userborn
