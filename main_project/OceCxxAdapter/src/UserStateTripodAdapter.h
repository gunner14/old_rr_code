#ifndef XCE_ADAPTER_USERSTATETRIPODADAPTER_H_
#define XCE_ADAPTER_USERSTATETRIPODADAPTER_H_

#include "UserStateInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userstate {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserState;

class UserStateTripodAdapter : public MyUtil::Singleton<UserStateTripodAdapter> {
public:
  UserStateInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserStateTripodAdapter();

private:
  friend class MyUtil::Singleton<UserStateTripodAdapter>;
  UserStateTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserState> > client_;
  const static int expiration;
};

class UserStateUtil {
public:
  static void parse(const PbUserState &data, UserStateInfoPtr &result);
};

}}} //namespace xce::adapter::userstate

#endif
