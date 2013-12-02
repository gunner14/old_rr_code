#ifndef XCE_ADAPTER_USERPASSPORTTRIPODADAPTER_H_
#define XCE_ADAPTER_USERPASSPORTTRIPODADAPTER_H_

#include "UserPassportInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userpassport {

using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::userbase::PbUserPassport;

class UserPassportTripodAdapter : public MyUtil::Singleton<UserPassportTripodAdapter> {
public:
  UserPassportInfoPtr Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  virtual ~UserPassportTripodAdapter();

private:
  friend class MyUtil::Singleton<UserPassportTripodAdapter>;
  UserPassportTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbUserPassport> > client_;
  const static int expiration;
};

class UserPassportUtil {
public:
  static void parse(const PbUserPassport &data, UserPassportInfoPtr &result);
};

}}} //namespace xce::adapter::userpassport

#endif
