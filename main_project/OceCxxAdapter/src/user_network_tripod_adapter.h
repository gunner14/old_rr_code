#ifndef __USER_NETWORK_TRIPOD_ADAPTER_H__
#define __USER_NETWORK_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace adapter {


class UserNetworkTripodAdapter : public MyUtil::Singleton<UserNetworkTripodAdapter> {
 public:
  UserNetworkTripodAdapter();

  bool LoadNetwork(int user_id, bool use_master) {   
    return network_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetNetwork(int user_id, xce::tripoddata::usernetwork::PbNetworkHistory* value) {   
    return network_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveNetwork(int user_id) {   
    return network_client_->Remove(user_id);
  }

  bool LoadCollege(int user_id, bool use_master) {   
    return college_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetCollege(int user_id, xce::tripoddata::usernetwork::PbCollegeInfo* value) {   
    return college_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveCollege(int user_id) {   
    return college_client_->Remove(user_id);
  }

  bool LoadElementarySchool(int user_id, bool use_master) {   
    return elementary_school_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetElementarySchool(int user_id, xce::tripoddata::usernetwork::PbElementarySchoolInfo* value) {   
    return elementary_school_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveElementarySchool(int user_id) {   
    return elementary_school_client_->Remove(user_id);
  }

  bool LoadHighSchool(int user_id, bool use_master) {   
    return high_school_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetHighSchool(int user_id, xce::tripoddata::usernetwork::PbHighSchoolInfo* value) {   
    return high_school_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveHighSchool(int user_id) {   
    return high_school_client_->Remove(user_id);
  }

  bool LoadJuniorHighSchool(int user_id, bool use_master) {   
    return junior_high_school_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetJuniorHighSchool(int user_id, xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo* value) {   
    return junior_high_school_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveJuniorHighSchool(int user_id) {   
    return junior_high_school_client_->Remove(user_id);
  }

  bool LoadRegion(int user_id, bool use_master) {   
    return region_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetRegion(int user_id, xce::tripoddata::usernetwork::PbRegionInfo* value) {   
    return region_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveRegion(int user_id) {   
    return region_client_->Remove(user_id);
  }

  bool LoadUniversity(int user_id, bool use_master) {   
    return university_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetUniversity(int user_id, xce::tripoddata::usernetwork::PbUniversityInfo* value) {   
    return university_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveUniversity(int user_id) {   
    return university_client_->Remove(user_id);
  }

  bool LoadWorkplace(int user_id, bool use_master) {   
    return workplace_client_->Load(user_id,kExpireTime,use_master);
  }

  bool GetWorkplace(int user_id, xce::tripoddata::usernetwork::PbWorkplaceInfo* value) {   
    return workplace_client_->Get(user_id,value,kExpireTime);
  }

  bool RemoveWorkplace(int user_id) {   
    return workplace_client_->Remove(user_id);
  }

 private:
  static const std::string kNameSpace;

  static const std::string kCollegeBizId;
  static const std::string kElementarySchoolBizId;
  static const std::string kHighSchoolBizId;
  static const std::string kJuniorHighSchoolBizId;
  static const std::string kNetworkBizId;
  static const std::string kRegionBizId;
  static const std::string kUniversityBizId;
  static const std::string kWorkplaceBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbNetworkHistory,long> > network_client_;
  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbCollegeInfo,long> > college_client_;
  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbElementarySchoolInfo,long> > elementary_school_client_;
  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbHighSchoolInfo,long> > high_school_client_;
  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo,long> > junior_high_school_client_;
  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbRegionInfo,long> > region_client_;
  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbUniversityInfo,long> > university_client_;
  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbWorkplaceInfo,long> > workplace_client_;

};


}}


#endif




