#include "UserStageTripodAdapter.h"

namespace xce {
namespace adapter {
namespace userstage {

const int UserStageTripodAdapter::expiration = 3600 * 24 * 30;

UserStageInfoPtr UserStageTripodAdapter::Get(int user_id) {
  UserStageInfoPtr result = new UserStageInfo;
  PbUserStage pbInfo;
  if(client_->Get(user_id, &pbInfo, expiration)) {    
    UserStageUtil::parse(pbInfo, result);
  }
  return result;
}

bool UserStageTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(user_id, expiration, is_master); 
}

bool UserStageTripodAdapter::Remove(int user_id) {
  return client_->Remove(user_id);
}

UserStageTripodAdapter::UserStageTripodAdapter() {
  client_.reset(new SingleTableTripodClient<PbUserStage>("xce_user", "user_stage"));
}

UserStageTripodAdapter::~UserStageTripodAdapter() {
  client_.reset();
}

void UserStageUtil::parse(const PbUserStage &data, UserStageInfoPtr &result) {
  result->set_id(data.id());
  result->set_univ(data.univ());
  result->set_stage(data.stage());
}

}}} //namespace xce::adapter::userstage
