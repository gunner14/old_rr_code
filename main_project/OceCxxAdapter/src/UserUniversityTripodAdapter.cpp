#include "OceCxxAdapter/src/UserUniversityTripodAdapter.h"

namespace xce {
namespace adapter {

const std::string UserUniversityTripodAdapter::kNameSpace = "xce_network";
const std::string UserUniversityTripodAdapter::kUniversityBizId = "user_network_university";
const int UserUniversityTripodAdapter::kExpireTime = 30*24*3600;

UserUniversityTripodAdapter::UserUniversityTripodAdapter() {
  university_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbUniversityInfo,long>(kNameSpace, kUniversityBizId) );
}

bool UserUniversityTripodAdapter::getUniversity(int user_id, xce::tripoddata::usernetwork::PbUniversityInfo* value) {   
  try {
    return university_client_->Get(user_id,value,kExpireTime);
  } catch (Ice::Exception& e) {
    MCE_WARN("UserUniversityTripodAdapter::getUniversity Ice::Exception id = " << user_id << " " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("UserUniversityTripodAdapter::getUniversity std::exception id = " << user_id << " " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("UserUniversityTripodAdapter::getUniversity UNKNOWN exception id = " << user_id);
    return false;
  }
}

int UserUniversityTripodAdapter::getUniversityInfoCount(int user_id) {
  xce::tripoddata::usernetwork::PbUniversityInfo data;
  bool value = getUniversity(user_id, &data);
  if (!value) {
    return 0;
  }
  return data.universities_size();
}


mop::hi::svc::model::UniversityInfoPtr UserUniversityTripodAdapter::getUniversityInfoByUnivId(int user_id, int universityid) {
  xce::tripoddata::usernetwork::PbUniversityInfo data;
  bool value = getUniversity(user_id, &data);
  if (!value) {
    return NULL;
  }
  for (int j = 0; j < data.universities_size(); j++) {
    const xce::tripoddata::usernetwork::PbUniversityInfo::PbUniversity& info = data.universities(j);
    if( info.id() == universityid ) {
      return parseFromProto(user_id,info);
    }
  }
}

mop::hi::svc::model::UniversityInfoSeq UserUniversityTripodAdapter::getUniversityInfoSeq(int user_id) {
  xce::tripoddata::usernetwork::PbUniversityInfo data;
  mop::hi::svc::model::UniversityInfoSeq result;
  bool value = getUniversity(user_id, &data);
  if (!value) {
    return result;
  }
  for (int j = 0; j < data.universities_size(); j++) {
    const xce::tripoddata::usernetwork::PbUniversityInfo::PbUniversity& info = data.universities(j);
    mop::hi::svc::model::UniversityInfoPtr tmp = parseFromProto(user_id,info);
    result.push_back(tmp);
  }
  return result;
}


mop::hi::svc::model::UniversityInfoPtr
UserUniversityTripodAdapter::parseFromProto(int user_id, const xce::tripoddata::usernetwork::PbUniversityInfo::PbUniversity& data) {
  mop::hi::svc::model::UniversityInfoPtr result = new mop::hi::svc::model::UniversityInfo;
  result->userId = user_id;
  result->id = data.id();
  result->enrollYear = data.enroll_year();
  result->dorm = data.dorm();
  result->department = data.department();
  result->typeOfCourse = data.type_of_course();
  result->universityId = data.university_id();
  result->universityName = data.university_name();
  return result;
}
 

}}

