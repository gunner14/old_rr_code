#ifndef __USER_BORN_ADAPTER_H__
#define __USER_BORN_ADAPTER_H__

#include "UserBornInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace userborn {

using namespace ::xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserBornAdapter : public MyUtil::ReplicatedClusterAdapterI<UserBornManagerPrx>,
	public MyUtil::Singleton<UserBornAdapter>
{
public:
	UserBornAdapter() : MyUtil::ReplicatedClusterAdapterI<UserBornManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel,true,"UBOM"){
	}
	UserBornInfoPtr getUserBorn(int id, const Ice::Context& ctx = Ice::Context());
	void reload(int id);
  void setUserBorn(int id, const UserBornInfoPtr& info);
  void createUserBorn(int id, const UserBornInfoPtr& info);
		

	UserBornManagerPrx getUserBornManager(int userId);
	UserBornManagerPrx getUserBorn10sManager(int userId);

};

}
}
}

#endif

