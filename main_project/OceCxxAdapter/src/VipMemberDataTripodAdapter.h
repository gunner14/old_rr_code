#ifndef XCE_ADAPTER_VIPMEMBERDATA_VIPMEMBERDATATRIPODADAPTER_H_
#define XCE_ADAPTER_VIPMEMBERDATA_VIPMEMBERDATATRIPODADAPTER_H_

#include "vip_info.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace vipmemberdata {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::vipinfo::PbVipMemberInfo;

class VipMemberDataTripodAdapter : public MyUtil::Singleton<VipMemberDataTripodAdapter> {
public:
  bool Load(int user_id, bool is_master = false);
  bool Remove(int user_id);
  virtual ~VipMemberDataTripodAdapter();

private:
  friend class MyUtil::Singleton<VipMemberDataTripodAdapter>;
  VipMemberDataTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbVipMemberInfo> > client_;
};

}}} //namespace xce::adapter::vipmemberdata

#endif
