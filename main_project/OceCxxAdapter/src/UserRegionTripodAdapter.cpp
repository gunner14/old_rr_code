#include "OceCxxAdapter/src/UserRegionTripodAdapter.h"

namespace xce {
namespace adapter {

const std::string UserRegionTripodAdapter::kNameSpace = "xce_network";
const std::string UserRegionTripodAdapter::kRegionBizId = "user_network_region";
const int UserRegionTripodAdapter::kExpireTime = 30*24*3600;

UserRegionTripodAdapter::UserRegionTripodAdapter() {
  region_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbRegionInfo,long>(kNameSpace, kRegionBizId) );
}

bool UserRegionTripodAdapter::getRegion(int user_id, xce::tripoddata::usernetwork::PbRegionInfo* value) {   
  try {
    return region_client_->Get(user_id,value,kExpireTime);
  } catch (Ice::Exception& e) {
    MCE_WARN("UserRegionTripodAdapter::getRegion Ice::Exception id = " << user_id << " " << e.what());
    return false;
  } catch (std::exception& e) {
    MCE_WARN("UserRegionTripodAdapter::getRegion std::exception id = " << user_id << " " << e.what());
    return false;
  } catch (...) {
    MCE_WARN("UserRegionTripodAdapter::getRegion UNKNOWN exception id = " << user_id);
    return false;
  }
}

 mop::hi::svc::model::RegionInfoSeq UserRegionTripodAdapter::getRegionInfoSeq(int user_id) {
   xce::tripoddata::usernetwork::PbRegionInfo data;
   mop::hi::svc::model::RegionInfoSeq result;
   bool value = getRegion(user_id, &data);
   if (!value) {
     return result;
   }
   for (int j = 0; j < data.regions_size(); j++) {
     const xce::tripoddata::usernetwork::PbRegionInfo::PbRegion& info = data.regions(j);
     mop::hi::svc::model::RegionInfoPtr tmp = parseFromProto(user_id,info);
     result.push_back(tmp);
   }
   return result;
 }


mop::hi::svc::model::RegionInfoPtr
UserRegionTripodAdapter::parseFromProto(int user_id, const xce::tripoddata::usernetwork::PbRegionInfo::PbRegion& data) {
  mop::hi::svc::model::RegionInfoPtr result = new mop::hi::svc::model::RegionInfo;
  result->userId = user_id;
  result->id = data.id();
  result->regionId = data.region_id();
  result->provinceName = data.province_name();
  result->cityName = data.city_name();
  return result;
}

}}

