#include "OceCxxAdapter/src/UserHighSchoolTripodAdapter.h"

namespace xce {
namespace adapter {

const std::string UserHighSchoolTripodAdapter::kNameSpace = "xce_network";
const std::string UserHighSchoolTripodAdapter::kHighSchoolBizId = "user_network_high_school";
const int UserHighSchoolTripodAdapter::kExpireTime = 30*24*3600;

UserHighSchoolTripodAdapter::UserHighSchoolTripodAdapter() {
  highschool_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbHighSchoolInfo,long>(kNameSpace, kHighSchoolBizId) );
}

bool UserHighSchoolTripodAdapter::getHighSchool(int user_id, xce::tripoddata::usernetwork::PbHighSchoolInfo* value) {   
  try {
    return highschool_client_->Get(user_id,value,kExpireTime);
  } catch (Ice::Exception& e) {
    MCE_WARN("UserHighSchoolTripodAdapter::getHighSchool Ice::Exception id = " << user_id << " " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("UserHighSchoolTripodAdapter::getHighSchool std::exception id = " << user_id << " " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("UserHighSchoolTripodAdapter::getHighSchool UNKNOWN exception id = " << user_id);
    return false;
  }
}

mop::hi::svc::model::HighSchoolInfoSeq UserHighSchoolTripodAdapter::getHighSchoolInfoSeq(int user_id) {
  xce::tripoddata::usernetwork::PbHighSchoolInfo data;
  mop::hi::svc::model::HighSchoolInfoSeq result;
  bool value = getHighSchool(user_id, &data);
  if (!value) {
    return result;
  }
  for (int j = 0; j < data.highschools_size(); j++) {
    const xce::tripoddata::usernetwork::PbHighSchoolInfo::PbHighSchool& info = data.highschools(j);
    mop::hi::svc::model::HighSchoolInfoPtr tmp = parseFromProto(user_id,info);
    result.push_back(tmp);
  }
  return result;
}

mop::hi::svc::model::HighSchoolInfoPtr
UserHighSchoolTripodAdapter::parseFromProto(int user_id, const xce::tripoddata::usernetwork::PbHighSchoolInfo::PbHighSchool& data) {
  mop::hi::svc::model::HighSchoolInfoPtr result = new mop::hi::svc::model::HighSchoolInfo;
  result->userId = user_id;
  result->id = data.id();
  result->enrollYear = data.enroll_year();
  result->HighSchoolId = data.high_school_id();
  result->HighSchoolName = data.high_school_name();
  result->hClass1 = data.h_class1();
  result->hClass2 = data.h_class2();
  result->hClass3 = data.h_class3();
  return result;
}

}}

