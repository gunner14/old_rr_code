#include "OceCxxAdapter/src/UserCollegeTripodAdapter.h"

namespace xce {
namespace adapter {

const std::string UserCollegeTripodAdapter::kNameSpace = "xce_network";
const std::string UserCollegeTripodAdapter::kCollegeBizId = "user_network_college";
const int UserCollegeTripodAdapter::kExpireTime = 30*24*3600;

UserCollegeTripodAdapter::UserCollegeTripodAdapter() {
  college_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbCollegeInfo,long>(kNameSpace, kCollegeBizId) );
}

bool UserCollegeTripodAdapter::getCollege(int user_id, xce::tripoddata::usernetwork::PbCollegeInfo* value) {   
  try {
    return college_client_->Get(user_id,value,kExpireTime);
  } catch (Ice::Exception& e) {
    MCE_WARN("UserCollegeTripodAdapter::getCollege Ice::Exception id = " << user_id << " " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("UserCollegeTripodAdapter::getCollege std::exception id = " << user_id << " " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("UserCollegeTripodAdapter::getCollege UNKNOWN exception id = " << user_id);
    return false;
  }
}

int UserCollegeTripodAdapter::getCollegeInfoCount(int user_id) {
  xce::tripoddata::usernetwork::PbCollegeInfo data;
  bool value = getCollege(user_id, &data);
  if (!value) {
    return 0;
  }
  return data.colleges_size();
}

mop::hi::svc::model::CollegeInfoPtr UserCollegeTripodAdapter::getCollegeInfoByCollegeId(int user_id, int collegeid) {
  xce::tripoddata::usernetwork::PbCollegeInfo data;
  bool value = getCollege(user_id, &data);
  if (!value) {
    return NULL;
  }
  for (int j = 0; j < data.colleges_size(); j++) {
    const xce::tripoddata::usernetwork::PbCollegeInfo::PbCollege& info = data.colleges(j);
    if( info.id() == collegeid ) {
      return parseFromProto(user_id,info);
    }
  }
}

mop::hi::svc::model::CollegeInfoSeq UserCollegeTripodAdapter::getCollegeInfoSeq(int user_id) {
  xce::tripoddata::usernetwork::PbCollegeInfo data;
  mop::hi::svc::model::CollegeInfoSeq result;
  bool value = getCollege(user_id, &data);
  if (!value) {
    return result;
  }
  for (int j = 0; j < data.colleges_size(); j++) {
    const xce::tripoddata::usernetwork::PbCollegeInfo::PbCollege& info = data.colleges(j);
    mop::hi::svc::model::CollegeInfoPtr tmp = parseFromProto(user_id,info);
    result.push_back(tmp);
  }
  return result;
}

mop::hi::svc::model::CollegeInfoPtr
UserCollegeTripodAdapter::parseFromProto(int user_id, const xce::tripoddata::usernetwork::PbCollegeInfo::PbCollege& data) {
  mop::hi::svc::model::CollegeInfoPtr result = new mop::hi::svc::model::CollegeInfo;
  result->userId = user_id;
  result->id = data.id();
  result->enrollYear = data.enroll_year();
  result->department = data.department();
  result->collegeId = data.college_id();
  result->collegeName = data.college_name();
  return result;
}

}}

