#ifndef __USER_UNIVERSITY_TRIPOD_ADAPTER_H__
#define __USER_UNIVERSITY_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"
#include "OceSlice/slice/UserNetwork.h"

namespace xce {
namespace adapter {


class UserUniversityTripodAdapter : public MyUtil::Singleton<UserUniversityTripodAdapter> {
 public:
  UserUniversityTripodAdapter();

  int getUniversityInfoCount(int user_id);

  mop::hi::svc::model::UniversityInfoPtr getUniversityInfoByUnivId(int user_id, int universityid);

  mop::hi::svc::model::UniversityInfoSeq getUniversityInfoSeq(int user_id);

 protected:
  bool getUniversity(int user_id, xce::tripoddata::usernetwork::PbUniversityInfo* value);   

  mop::hi::svc::model::UniversityInfoPtr parseFromProto(int user_id,
      const xce::tripoddata::usernetwork::PbUniversityInfo::PbUniversity& data);   

 private:
  static const std::string kNameSpace;

  static const std::string kUniversityBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbUniversityInfo,long> > university_client_;

};


}}


#endif




