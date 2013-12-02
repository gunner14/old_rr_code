#ifndef XCE_ADAPTER_HATDATA_HATDATATRIPODADAPTER_H_
#define XCE_ADAPTER_HATDATA_HATDATATRIPODADAPTER_H_

#include "vip_info.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace hatdata {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::vipinfo::PbVipIconUrl;

class HatDataTripodAdapter : public MyUtil::Singleton<HatDataTripodAdapter> {
public:
  bool Load(int user_id, bool is_master = false);
  bool Remove(int user_id);
  virtual ~HatDataTripodAdapter();

private:
  friend class MyUtil::Singleton<HatDataTripodAdapter>;
  HatDataTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbVipIconUrl> > client_;
};

}}} //namespace xce::adapter::hatdata

#endif
