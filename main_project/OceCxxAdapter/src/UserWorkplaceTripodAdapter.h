#ifndef __USER_Workplace_TRIPOD_ADAPTER_H__
#define __USER_Workplace_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"
#include "OceSlice/slice/UserNetwork.h"

namespace xce {
namespace adapter {


class UserWorkplaceTripodAdapter : public MyUtil::Singleton<UserWorkplaceTripodAdapter> {
 public:
  UserWorkplaceTripodAdapter();

  mop::hi::svc::model::WorkplaceInfoSeq getWorkplaceInfoSeq(int user_id);

 protected:
  bool getWorkplace(int user_id, xce::tripoddata::usernetwork::PbWorkplaceInfo* value);   

  mop::hi::svc::model::WorkplaceInfoPtr parseFromProto(int user_id,
      const xce::tripoddata::usernetwork::PbWorkplaceInfo::PbWorkplace& data);   

 private:
  static const std::string kNameSpace;

  static const std::string kWorkplaceBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbWorkplaceInfo,long> > workplace_client_;

};


}}


#endif




