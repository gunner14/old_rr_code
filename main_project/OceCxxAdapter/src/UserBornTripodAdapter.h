#ifndef XCE_ADAPTER_USERBORNTRIPODADAPTER_H_
#define XCE_ADAPTER_USERBORNTRIPODADAPTER_H_

#include "UserBornInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userborn {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserBorn;

class UserBornTripodAdapter : public MyUtil::Singleton<UserBornTripodAdapter> {
public:
  UserBornInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserBornTripodAdapter();

private:
  friend class MyUtil::Singleton<UserBornTripodAdapter>;
  UserBornTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserBorn> > client_;
  const static int expiration;
};

class UserBornUtil {
public:
  static void parse(const PbUserBorn &data, UserBornInfoPtr &result);
};

}}} //namespace xce::adapter::userborn

#endif
