#include "OceCxxAdapter/src/UserJuniorHighSchoolTripodAdapter.h"

namespace xce {
namespace adapter {

const std::string UserJuniorHighSchoolTripodAdapter::kNameSpace = "xce_network";
const std::string UserJuniorHighSchoolTripodAdapter::kJuniorHighSchoolBizId = "user_network_junior_high_school";
const int UserJuniorHighSchoolTripodAdapter::kExpireTime = 30*24*3600;

UserJuniorHighSchoolTripodAdapter::UserJuniorHighSchoolTripodAdapter() {
  client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo,long>(kNameSpace, kJuniorHighSchoolBizId) );
}

bool UserJuniorHighSchoolTripodAdapter::getJuniorHighSchool(int user_id, xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo* value) {   
  try {
    return client_->Get(user_id,value,kExpireTime);
  } catch (Ice::Exception& e) {
    MCE_WARN("UserJuniorHighSchoolTripodAdapter::getJuniorHighSchool Ice::Exception id = " << user_id << " " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("UserJuniorHighSchoolTripodAdapter::getJuniorHighSchool std::exception id = " << user_id << " " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("UserJuniorHighSchoolTripodAdapter::getJuniorHighSchool UNKNOWN exception id = " << user_id);
    return false;
  }
}

mop::hi::svc::model::JuniorHighSchoolInfoSeq UserJuniorHighSchoolTripodAdapter::getJuniorHighSchoolInfoSeq(int user_id) {
  xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo data;
  mop::hi::svc::model::JuniorHighSchoolInfoSeq result;
  bool value = getJuniorHighSchool(user_id, &data);
  if (!value) {
    return result;
  }
  for (int j = 0; j < data.juniorhighschools_size(); j++) {
    const xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo::PbJuniorHighSchool& info = data.juniorhighschools(j);
    mop::hi::svc::model::JuniorHighSchoolInfoNPtr tmp = parseFromProto(user_id,info);
    result.push_back(tmp);
  }
  return result;
}

mop::hi::svc::model::JuniorHighSchoolInfoNPtr
UserJuniorHighSchoolTripodAdapter::parseFromProto(int user_id, const xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo::PbJuniorHighSchool& data) {
  mop::hi::svc::model::JuniorHighSchoolInfoNPtr result = new mop::hi::svc::model::JuniorHighSchoolInfoN;
  result->userId = user_id;
  result->id = data.id();
  result->juniorHighSchoolId = data.junior_high_school_id();
  result->juniorHighSchoolName = data.junior_high_school_name();
  result->juniorHighSchoolYear = data.junior_high_school_year();
  return result;
}

}}

