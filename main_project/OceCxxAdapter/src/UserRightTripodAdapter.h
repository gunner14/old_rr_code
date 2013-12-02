#ifndef XCE_ADAPTER_USERRIGHTTRIPODADAPTER_H_
#define XCE_ADAPTER_USERRIGHTTRIPODADAPTER_H_

#include "UserRightInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userright {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserRight;

class UserRightTripodAdapter : public MyUtil::Singleton<UserRightTripodAdapter> {
public:
  UserRightInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserRightTripodAdapter();

private:
  friend class MyUtil::Singleton<UserRightTripodAdapter>;
  UserRightTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserRight> > client_;
  const static int expiration;
};

class UserRightUtil {
public:
  static void parse(const PbUserRight &data, UserRightInfoPtr &result);
};

}}} //namespace xce::adapter::userright

#endif
