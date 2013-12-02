#ifndef __USER_COLLEGE_TRIPOD_ADAPTER_H__
#define __USER_COLLEGE_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"
#include "OceSlice/slice/UserNetwork.h"

namespace xce {
namespace adapter {


class UserCollegeTripodAdapter : public MyUtil::Singleton<UserCollegeTripodAdapter> {
 public:
  UserCollegeTripodAdapter();

  int getCollegeInfoCount(int user_id);

  mop::hi::svc::model::CollegeInfoPtr getCollegeInfoByCollegeId(int user_id, int collegeid);

  mop::hi::svc::model::CollegeInfoSeq getCollegeInfoSeq(int user_id);

 protected:
  bool getCollege(int user_id, xce::tripoddata::usernetwork::PbCollegeInfo* value);   

  mop::hi::svc::model::CollegeInfoPtr parseFromProto(int user_id,
      const xce::tripoddata::usernetwork::PbCollegeInfo::PbCollege& data);   

 private:
  static const std::string kNameSpace;

  static const std::string kCollegeBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbCollegeInfo,long> > college_client_;

};


}}


#endif




