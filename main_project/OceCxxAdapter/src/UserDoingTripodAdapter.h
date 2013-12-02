#ifndef XCE_ADAPTER_USERDOINGTRIPODADAPTER_H_
#define XCE_ADAPTER_USERDOINGTRIPODADAPTER_H_

#include "UserDoingInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userdoing {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserDoing;

class UserDoingTripodAdapter : public MyUtil::Singleton<UserDoingTripodAdapter> {
public:
  UserDoingInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserDoingTripodAdapter();

private:
  friend class MyUtil::Singleton<UserDoingTripodAdapter>;
  UserDoingTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserDoing> > client_;
  const static int expiration;
};

class UserDoingUtil {
public:
  static void parse(const PbUserDoing &data, UserDoingInfoPtr &result);
};

}}} //namespace xce::adapter::userdoing

#endif
