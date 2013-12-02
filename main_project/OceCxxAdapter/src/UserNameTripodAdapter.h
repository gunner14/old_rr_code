#ifndef XCE_ADAPTER_USERNAMETRIPODADAPTER_H_
#define XCE_ADAPTER_USERNAMETRIPODADAPTER_H_

#include "UserNameInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace username {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserName;

class UserNameTripodAdapter : public MyUtil::Singleton<UserNameTripodAdapter> {
public:
  UserNameInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserNameTripodAdapter();

private:
  friend class MyUtil::Singleton<UserNameTripodAdapter>;
  UserNameTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserName> > client_;
  const static int expiration;
};

class UserNameUtil {
public:
  static void parse(const PbUserName &data, UserNameInfoPtr &result);
};

}}} //namespace xce::adapter::username

#endif
