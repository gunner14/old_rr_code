#include "OceCxxAdapter/src/UserWorkplaceTripodAdapter.h"

namespace xce {
namespace adapter {

const std::string UserWorkplaceTripodAdapter::kNameSpace = "xce_network";
const std::string UserWorkplaceTripodAdapter::kWorkplaceBizId = "user_network_workplace";
const int UserWorkplaceTripodAdapter::kExpireTime = 30*24*3600;

UserWorkplaceTripodAdapter::UserWorkplaceTripodAdapter() {
  workplace_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbWorkplaceInfo,long>(kNameSpace, kWorkplaceBizId) );
}

bool UserWorkplaceTripodAdapter::getWorkplace(int user_id, xce::tripoddata::usernetwork::PbWorkplaceInfo* value) {   
  try {
    return workplace_client_->Get(user_id,value,kExpireTime);
  } catch (Ice::Exception& e) {
    MCE_WARN("UserWorkplaceTripodAdapter::getWorkplace Ice::Exception id = " << user_id << " " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("UserWorkplaceTripodAdapter::getWorkplace std::exception id = " << user_id << " " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("UserWorkplaceTripodAdapter::getWorkplace UNKNOWN exception id = " << user_id);
    return false;
  }
}

 mop::hi::svc::model::WorkplaceInfoSeq UserWorkplaceTripodAdapter::getWorkplaceInfoSeq(int user_id) {
   xce::tripoddata::usernetwork::PbWorkplaceInfo data;
   mop::hi::svc::model::WorkplaceInfoSeq result;
   bool value = getWorkplace(user_id, &data);
   if (!value) {
     return result;
   }
   for (int j = 0; j < data.workplaces_size(); j++) {
     const xce::tripoddata::usernetwork::PbWorkplaceInfo::PbWorkplace& info = data.workplaces(j);
     mop::hi::svc::model::WorkplaceInfoPtr tmp = parseFromProto(user_id,info);
     result.push_back(tmp);
   }
   return result;
 }


mop::hi::svc::model::WorkplaceInfoPtr
UserWorkplaceTripodAdapter::parseFromProto(int user_id, const xce::tripoddata::usernetwork::PbWorkplaceInfo::PbWorkplace& data) {
  mop::hi::svc::model::WorkplaceInfoPtr result = new mop::hi::svc::model::WorkplaceInfo;
  result->userId = user_id;
  result->id = data.id();
  result->workplaceId = data.workplace_id();
  result->workplaceName = data.workplace_name();
  result->joinMonth = data.join_month();
  result->joinYear = data.join_year();
  result->address = data.address();
  result->description = data.description();
  result->province = data.province();
  result->cityId = data.city_id();
  result->cityName = data.city_name();
  result->quitMonth = data.quit_month();
  result->quitYear = data.quit_year();
  result->type = data.type();
  result->jobTitleId = data.job_title_id();
  result->positionId = data.position_id();
  return result;
}

}}

