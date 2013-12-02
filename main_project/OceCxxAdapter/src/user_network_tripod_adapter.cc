#include "OceCxxAdapter/src/user_network_tripod_adapter.h"



namespace xce {
namespace adapter {

const std::string UserNetworkTripodAdapter::kNameSpace = "xce_network";

const std::string UserNetworkTripodAdapter::kCollegeBizId = "user_network_college";
const std::string UserNetworkTripodAdapter::kElementarySchoolBizId = "user_network_elementary_school";
const std::string UserNetworkTripodAdapter::kHighSchoolBizId = "user_network_high_school";
const std::string UserNetworkTripodAdapter::kJuniorHighSchoolBizId = "user_network_junior_high_school";
const std::string UserNetworkTripodAdapter::kNetworkBizId = "user_network_network";
const std::string UserNetworkTripodAdapter::kRegionBizId = "user_network_region";
const std::string UserNetworkTripodAdapter::kUniversityBizId = "user_network_university";
const std::string UserNetworkTripodAdapter::kWorkplaceBizId = "user_network_workplace";

const int UserNetworkTripodAdapter::kExpireTime = 30*24*3600;


UserNetworkTripodAdapter::UserNetworkTripodAdapter() {
  network_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbNetworkHistory,long>(kNameSpace, kNetworkBizId) );
  college_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbCollegeInfo,long>(kNameSpace, kCollegeBizId) );
  elementary_school_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbElementarySchoolInfo,long>(kNameSpace, kElementarySchoolBizId) );
  high_school_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbHighSchoolInfo,long>(kNameSpace, kHighSchoolBizId) );
  junior_high_school_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo,long>(kNameSpace, kJuniorHighSchoolBizId) );
  region_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbRegionInfo,long>(kNameSpace, kRegionBizId) );
  university_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbUniversityInfo,long>(kNameSpace, kUniversityBizId) );
  workplace_client_.reset(
    new xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbWorkplaceInfo,long>(kNameSpace, kWorkplaceBizId) );
}

 

}}

