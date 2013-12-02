#ifndef __USER_STAGE_ADAPTER_H__
#define __USER_STAGE_ADAPTER_H__

#include "UserStageInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace userstage {

using namespace ::xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserStageAdapter : public MyUtil::ReplicatedClusterAdapterI<UserStageManagerPrx>,
	public MyUtil::Singleton<UserStageAdapter>
{
public:
	UserStageAdapter() : MyUtil::ReplicatedClusterAdapterI<UserStageManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel,true,"USGM"){
	}
	UserStageInfoPtr getUserStage(int id, const Ice::Context& ctx = Ice::Context());
	void setUserStage(int id, const UserStageInfoPtr& ptr);
	void createUserStage(int id, const UserStageInfoPtr& ptr);
	void reload(int id, const Ice::Context& ctx = Ice::Context());
	UserStageManagerPrx getUserStage10sManager(int userId);
	UserStageManagerPrx getUserStageManager(int userId);

private:
};

}
}
}

#endif

