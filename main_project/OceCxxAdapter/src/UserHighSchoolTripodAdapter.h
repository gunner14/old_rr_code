#ifndef __USER_HighSchool_TRIPOD_ADAPTER_H__
#define __USER_HighSchool_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"
#include "OceSlice/slice/UserNetwork.h"

namespace xce {
namespace adapter {


class UserHighSchoolTripodAdapter : public MyUtil::Singleton<UserHighSchoolTripodAdapter> {
 public:
  UserHighSchoolTripodAdapter();

  mop::hi::svc::model::HighSchoolInfoSeq getHighSchoolInfoSeq(int user_id);

 protected:
  bool getHighSchool(int user_id, xce::tripoddata::usernetwork::PbHighSchoolInfo* value);   

  mop::hi::svc::model::HighSchoolInfoPtr parseFromProto(int user_id,
      const xce::tripoddata::usernetwork::PbHighSchoolInfo::PbHighSchool& data);   

 private:
  static const std::string kNameSpace;

  static const std::string kHighSchoolBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbHighSchoolInfo,long> > highschool_client_;

};


}}


#endif




