#ifndef __USER_ElementarySchool_TRIPOD_ADAPTER_H__
#define __USER_ElementarySchool_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"
#include "OceSlice/slice/UserNetwork.h"

namespace xce {
namespace adapter {


class UserElementarySchoolTripodAdapter : public MyUtil::Singleton<UserElementarySchoolTripodAdapter> {
 public:
  UserElementarySchoolTripodAdapter();

  mop::hi::svc::model::ElementarySchoolInfoSeq getElementarySchoolInfoSeq(int user_id);

 protected:
  bool getElementarySchool(int user_id, xce::tripoddata::usernetwork::PbElementarySchoolInfo* value);   

  mop::hi::svc::model::ElementarySchoolInfoNPtr parseFromProto(int user_id,
      const xce::tripoddata::usernetwork::PbElementarySchoolInfo::PbElementarySchool& data);   

 private:
  static const std::string kNameSpace;

  static const std::string kElementarySchoolBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbElementarySchoolInfo,long> > client_;

};


}}


#endif




