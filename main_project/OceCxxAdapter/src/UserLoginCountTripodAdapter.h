#ifndef XCE_ADAPTER_USERLOGINCOUNTTRIPODADAPTER_H_
#define XCE_ADAPTER_USERLOGINCOUNTTRIPODADAPTER_H_

#include "Singleton.h"
#include "tripod2/cc/core/single_table_tripod_client.h"

namespace xce {
namespace adapter {
namespace userlogincount {

using xce::tripod::TripodClient;

class UserLoginCountTripodAdapter : public MyUtil::Singleton<UserLoginCountTripodAdapter> {
public:
  int Get(int user_id);
  bool Load(int user_id, bool is_master = false);
  //删除缓存，仅供中间层使用
  bool Remove(int user_id);
  long Inc(int user_id, int step);
  virtual ~UserLoginCountTripodAdapter();

private:
  friend class MyUtil::Singleton<UserLoginCountTripodAdapter>;
  UserLoginCountTripodAdapter();
  boost::shared_ptr<TripodClient> client_;
  const static int expiration;
};

}}} //namespace xce::adapter::userlogincount

#endif
