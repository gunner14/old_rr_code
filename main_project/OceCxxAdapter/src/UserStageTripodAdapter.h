#ifndef XCE_ADAPTER_USERSTAGETRIPODADAPTER_H_
#define XCE_ADAPTER_USERSTAGETRIPODADAPTER_H_

#include "UserStageInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userstage {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserStage;

class UserStageTripodAdapter : public MyUtil::Singleton<UserStageTripodAdapter> {
public:
  UserStageInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserStageTripodAdapter();

private:
  friend class MyUtil::Singleton<UserStageTripodAdapter>;
  UserStageTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserStage> > client_;
  const static int expiration;
};

class UserStageUtil {
public:
  static void parse(const PbUserStage &data, UserStageInfoPtr &result);
};

}}} //namespace xce::adapter::userstage

#endif
