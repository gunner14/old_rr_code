/*
 * =====================================================================================
 *
 *       Filename:  UserPassportAdapter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年09月16日 12时02分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */


#ifndef __USER_STATE_ADAPTER_H__
#define __USER_STATE_ADAPTER_H__

#include "UserStateInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace userstate{

using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserStateAdapter : public MyUtil::ReplicatedClusterAdapterI<UserStateManagerPrx>,
	public MyUtil::Singleton<UserStateAdapter>
{
public:
	UserStateAdapter() : MyUtil::ReplicatedClusterAdapterI<UserStateManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel,true,"USM"){
	}
	void reload(int id, const Ice::Context& ctx = Ice::Context());

	void setKeepUseOn(int userid);
	void setKeepUseOff(int userid);
	void setActiveOn(int userid);
	void setActiveOff(int userid);
	void setZombieOn(int userid);
	void setZombieOff(int userid);
	UserStateInfoPtr getUserState(int userid, const Ice::Context& ctx = Ice::Context());
	void setUserState(int id, const UserStateInfoPtr& info);
	void createUserState(int id, const UserStateInfoPtr& info);

	UserStateManagerPrx getUserStateManager(int userId);
	UserStateManagerPrx getUserState10sManager(int userId);
	UserStateManagerPrx getUserStateManagerOneway(int userId);

};

}
}
}

#endif

