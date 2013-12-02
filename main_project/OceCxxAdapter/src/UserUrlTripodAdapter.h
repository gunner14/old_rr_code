#ifndef XCE_ADAPTER_USERURLTRIPODADAPTER_H_
#define XCE_ADAPTER_USERURLTRIPODADAPTER_H_

#include "UserUrlInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userurl {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserUrl;

class UserUrlTripodAdapter : public MyUtil::Singleton<UserUrlTripodAdapter> {
public:
  UserUrlInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserUrlTripodAdapter();

private:
  friend class MyUtil::Singleton<UserUrlTripodAdapter>;
  UserUrlTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserUrl> > client_;
  const static int expiration;
};

class UserUrlUtil {
public:
  static void parse(const PbUserUrl &data, UserUrlInfoPtr &result);
};

}}} //namespace xce::adapter::userurl

#endif
