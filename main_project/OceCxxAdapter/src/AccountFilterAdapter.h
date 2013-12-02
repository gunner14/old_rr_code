#ifndef XCE_ADAPTER_ACCOUNT_ACCOUNTFILTER_ADAPTER_H_
#define XCE_ADAPTER_ACCOUNT_ACCOUNTFILTER_ADAPTER_H_

#include "AccountFilter.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace account {

class AccountFilterAdapter : public MyUtil::Singleton<AccountFilterAdapter>,
    public MyUtil::ReplicatedClusterAdapterI<xce::account::AccountFilterPrx> {
public:
  void AddAccount(const string &account, int id);
  MyUtil::IntSeq GetIdBySHA256(const MyUtil::StrSeq &digests);

private:
  friend class MyUtil::Singleton<AccountFilterAdapter>;
  AccountFilterAdapter();
};

}}}
#endif
