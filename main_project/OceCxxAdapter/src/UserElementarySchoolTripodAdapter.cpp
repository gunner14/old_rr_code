#include "OceCxxAdapter/src/UserElementarySchoolTripodAdapter.h"

namespace xce {
namespace adapter {

const std::string UserElementarySchoolTripodAdapter::kNameSpace = "xce_network";
const std::string UserElementarySchoolTripodAdapter::kElementarySchoolBizId = "user_network_elementary_school";
const int UserElementarySchoolTripodAdapter::kExpireTime = 30*24*3600;

UserElementarySchoolTripodAdapter::UserElementarySchoolTripodAdapter() {
  client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbElementarySchoolInfo,long>(kNameSpace, kElementarySchoolBizId) );
}

bool UserElementarySchoolTripodAdapter::getElementarySchool(int user_id, xce::tripoddata::usernetwork::PbElementarySchoolInfo* value) {   
  try {
    return client_->Get(user_id,value,kExpireTime);
  } catch (Ice::Exception& e) {
    MCE_WARN("UserElementarySchoolTripodAdapter::getElementarySchool Ice::Exception id = " << user_id << " " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("UserElementarySchoolTripodAdapter::getElementarySchool std::exception id = " << user_id << " " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("UserElementarySchoolTripodAdapter::getElementarySchool UNKNOWN exception id = " << user_id);
    return false;
  }
}

mop::hi::svc::model::ElementarySchoolInfoSeq UserElementarySchoolTripodAdapter::getElementarySchoolInfoSeq(int user_id) {
  xce::tripoddata::usernetwork::PbElementarySchoolInfo data;
  mop::hi::svc::model::ElementarySchoolInfoSeq result;
  bool value = getElementarySchool(user_id, &data);
  if (!value) {
    return result;
  }
  for (int j = 0; j < data.elementaryschools_size(); j++) {
    const xce::tripoddata::usernetwork::PbElementarySchoolInfo::PbElementarySchool& info = data.elementaryschools(j);
    mop::hi::svc::model::ElementarySchoolInfoNPtr tmp = parseFromProto(user_id,info);
    result.push_back(tmp);
  }
  return result;
}

mop::hi::svc::model::ElementarySchoolInfoNPtr
UserElementarySchoolTripodAdapter::parseFromProto(int user_id, const xce::tripoddata::usernetwork::PbElementarySchoolInfo::PbElementarySchool& data) {
  mop::hi::svc::model::ElementarySchoolInfoNPtr result = new mop::hi::svc::model::ElementarySchoolInfoN;
  result->userId = user_id;
  result->id = data.id();
  result->elementarySchoolId = data.elementary_school_id();
  result->elementarySchoolName = data.elementary_school_name();
  result->elementarySchoolYear = data.elementary_school_year();
  return result;
}

}}

