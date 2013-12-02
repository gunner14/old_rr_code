#ifndef XCE_ADAPTER_ACCOUNT_ACCOUNTCONNECTTRIPODADAPTER_H_
#define XCE_ADAPTER_ACCOUNT_ACCOUNTCONNECTTRIPODADAPTER_H_

#include "UtilCxx/src/Singleton.h"
#include "tripod2/cc/core/tripod_client.h"

namespace xce {
namespace adapter {
namespace account {

class AccountConnectTripodAdapter : public MyUtil::Singleton<AccountConnectTripodAdapter> {
public:
  int Get(const string& cid, int type, bool use_master = false);
  bool Load(const string& cid, int type, bool use_master = false);
  virtual ~AccountConnectTripodAdapter();

private:
  friend class MyUtil::Singleton<AccountConnectTripodAdapter>;
  AccountConnectTripodAdapter();
  boost::shared_ptr<xce::tripod::TripodClient> client_;
  const static int expiration;
};

}}} //namespace

#endif
