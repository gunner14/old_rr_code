#ifndef __USER_Region_TRIPOD_ADAPTER_H__
#define __USER_Region_TRIPOD_ADAPTER_H__

#include <string>
#include "boost/shared_ptr.hpp"
#include "OceProto/proto/user_network.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UtilCxx/src/Singleton.h"
#include "OceSlice/slice/UserNetwork.h"

namespace xce {
namespace adapter {


class UserRegionTripodAdapter : public MyUtil::Singleton<UserRegionTripodAdapter> {
 public:
  UserRegionTripodAdapter();

  mop::hi::svc::model::RegionInfoSeq getRegionInfoSeq(int user_id);

 protected:
  bool getRegion(int user_id, xce::tripoddata::usernetwork::PbRegionInfo* value);   

  mop::hi::svc::model::RegionInfoPtr parseFromProto(int user_id,
      const xce::tripoddata::usernetwork::PbRegionInfo::PbRegion& data);   

 private:
  static const std::string kNameSpace;

  static const std::string kRegionBizId;

  static const int kExpireTime;

  boost::shared_ptr<xce::tripod::SingleTableTripodClient<xce::tripoddata::usernetwork::PbRegionInfo,long> > region_client_;

};


}}


#endif




