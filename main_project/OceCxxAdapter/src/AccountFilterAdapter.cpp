#include "AccountFilterAdapter.h"

using namespace xce::adapter::account;
using namespace xce::account;

AccountFilterAdapter::AccountFilterAdapter() :
    MyUtil::ReplicatedClusterAdapterI<AccountFilterPrx>("ControllerAccountFilter", 120, 300) {
}

void AccountFilterAdapter::AddAccount(const string &account, int id) {
  std::vector<AccountFilterPrx> allProxies = getAllProxySeq(0);
  for (std::vector<AccountFilterPrx>::const_iterator it = allProxies.begin(); it != allProxies.end(); ++it) {
    try {
      (*it)->ice_timeout(1000)->AddAccount(account, id);
    } catch (Ice::Exception& e) {
      MCE_WARN("AccountFilterAdapter::AddAccount Exception account: " << account
          << " ,id: " << id << " ,e: " << e.what()); 
    }
  }  
}

MyUtil::IntSeq AccountFilterAdapter::GetIdBySHA256(const MyUtil::StrSeq &digests) {
  try {
    return getProxy(0)->GetIdBySHA256(digests);
  } catch (Ice::Exception& e) {
    MCE_WARN("AccountFilterAdapter::AddAccount Exception digest.size: "
        << digests.size() << " ,e: " << e.what()); 
  }
  return MyUtil::IntSeq();
}
