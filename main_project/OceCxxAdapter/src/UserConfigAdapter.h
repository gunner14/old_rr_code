#ifndef __USER_CONFIGR_ADAPTER_H__
#define __USER_CONFIGR_ADAPTER_H__

#include "UserConfig.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "UserConfigInfo.h"


namespace xce {
namespace adapter {

class UserConfigAdapter : public MyUtil::ReplicatedClusterAdapterI<mop::hi::svc::model::UserConfigManagerPrx>,
	public MyUtil::Singleton<UserConfigAdapter> {
public:
	UserConfigAdapter() : MyUtil::ReplicatedClusterAdapterI<mop::hi::svc::model::UserConfigManagerPrx> ("ControllerUserConfigR",120,300){
	}
  void reload(int id, const Ice::Context& ctx = Ice::Context());
	UserConfigInfoPtr getUserConfig(int userid, const Ice::Context& ctx = Ice::Context());
  void setUserConfig(int id, const UserConfigInfoPtr& info);
  void createUserConfig(int id, const UserConfigInfoPtr& info);

  //新加的供WriterAgent使用的接口
  void setUserConfig(int userId, const MyUtil::Str2StrMap& properties);
  void createUserConfig(int userId, const MyUtil::Str2StrMap& properties);

private:
	// UserConfigAgent will use these function
	mop::hi::svc::model::UserConfigManagerPrx getUserConfigManager(int userId);
	mop::hi::svc::model::UserConfigManagerPrx getUserConfig10sManager(int userId);
	mop::hi::svc::model::UserConfigManagerPrx getUserConfigManagerOneway(
			int userId);

};

}
}

#endif

