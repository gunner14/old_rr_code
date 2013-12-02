#ifndef __USER_CONFIGR_TRIPOD_ADAPTER_H__
#define __USER_CONFIGR_TRIPOD_ADAPTER_H__
#include "Singleton.h"
#include "OceProto/proto/UserConfig.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "UserConfigInfo.h"

namespace xce {
namespace adapter {


class UserConfigTripodAdapter: public MyUtil::Singleton<UserConfigTripodAdapter> {
public:

  UserConfigTripodAdapter() : kUserConfigNamespace("xce_user"), kUserConfigBizId("user_config"), kExpireSeconds(0){
    MCE_DEBUG( "UserConfigTripodAdapter: Construct UserConfigTripodAdapter");
    MCE_DEBUG( "UserConfigTripodAdapter: Create SingleTableTripodClient");
    client_ = new xce::tripod::SingleTableTripodClient<xce::tripod::UserConfigData, long> (kUserConfigNamespace, kUserConfigBizId);
  }

  virtual ~UserConfigTripodAdapter() {
    MCE_DEBUG( "UserConfigTripodAdapter: Deconstruct UserConfigTripodAdapter");
    if (client_){
      MCE_DEBUG( "UserConfigTripodAdapter: Delete SingleTableTripodClient");
      delete client_;
    }
  }

  bool reload(int id, const Ice::Context& ctx = Ice::Context());
  UserConfigInfoPtr getUserConfig(int userid, const Ice::Context& ctx = Ice::Context());
  bool remove(int id);

private:
//  void printData(const xce::tripod::UserConfigData& data);
  xce::tripod::SingleTableTripodClient<xce::tripod::UserConfigData, long>* client_;
  const std::string kUserConfigNamespace;
  const std::string kUserConfigBizId;
  const int kExpireSeconds;
//  const std::string kUserConfigNamespace = "xce_user";
//  const std::string kUserConfigBizId = "user_config";
//  const int kExpireSeconds = 0;
};

}
}

#endif

