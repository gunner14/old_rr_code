#ifndef __USER_BASE_ADAPTER_H__
#define __USER_BASE_ADAPTER_H__

#include "UserBaseInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace userbase {

using namespace ::xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserBaseAdapter : public MyUtil::ReplicatedClusterAdapterI<UserBaseViewManagerPrx>,
	public MyUtil::Singleton<UserBaseAdapter>
{
public:
	UserBaseAdapter() : MyUtil::ReplicatedClusterAdapterI<UserBaseViewManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel,true,"UBVM"){
	}
	UserBaseFullViewPtr getUserBaseFullView(int id, const Ice::Context& ctx = Ice::Context());
	UserBaseCommonViewPtr getUserBaseCommonView(int id, const Ice::Context& ctx = Ice::Context());
		

	UserBaseViewManagerPrx getUserBaseViewManager(int userId);
	UserBaseViewManagerPrx getUserBaseViewManagerOneway(int userId);
};

}
}
}

#endif

