#ifndef XCE_ADAPTER_ICON2URLDATA_ICON2URLDATATRIPODADAPTER_H_
#define XCE_ADAPTER_ICON2URLDATA_ICON2URLDATATRIPODADAPTER_H_

#include "vip_info.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace icon2urldata {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::vipinfo::PbVipIconUrl;

class Icon2UrlDataTripodAdapter : public MyUtil::Singleton<Icon2UrlDataTripodAdapter> {
public:
  bool Load(int user_id, bool is_master = false);
  bool Remove(int user_id);
  virtual ~Icon2UrlDataTripodAdapter();

private:
  friend class MyUtil::Singleton<Icon2UrlDataTripodAdapter>;
  Icon2UrlDataTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbVipIconUrl> > client_;
};

}}} //namespace xce::adapter::icon2urldata

#endif
