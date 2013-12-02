#ifndef __USER_JuniorHighSchool_TRIPOD_ADAPTER_H__
#define __USER_JuniorHighSchool_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"
#include "OceSlice/slice/UserNetwork.h"

namespace xce {
namespace adapter {


class UserJuniorHighSchoolTripodAdapter : public MyUtil::Singleton<UserJuniorHighSchoolTripodAdapter> {
 public:
  UserJuniorHighSchoolTripodAdapter();

  mop::hi::svc::model::JuniorHighSchoolInfoSeq getJuniorHighSchoolInfoSeq(int user_id);

 protected:
  bool getJuniorHighSchool(int user_id, xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo* value);   

  mop::hi::svc::model::JuniorHighSchoolInfoNPtr parseFromProto(int user_id,
      const xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo::PbJuniorHighSchool& data);   

 private:
  static const std::string kNameSpace;

  static const std::string kJuniorHighSchoolBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo,long> > client_;

};


}}


#endif




