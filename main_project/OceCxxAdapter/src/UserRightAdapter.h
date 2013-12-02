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


#ifndef __USER_RIGHT_ADAPTER_H__
#define __USER_RIGHT_ADAPTER_H__

#include "UserRightInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace userright {

using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserRightAdapter : public MyUtil::ReplicatedClusterAdapterI<UserRightManagerPrx>,
	public MyUtil::Singleton<UserRightAdapter>
{
public:
	UserRightAdapter() : MyUtil::ReplicatedClusterAdapterI<UserRightManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel, true,"URM"){
	}
	UserRightInfoPtr getUserRight(int userId, const Ice::Context& ctx = Ice::Context());
	void setUserRight(int id, const UserRightInfoPtr& info);
	void createUserRight(int id, const UserRightInfoPtr& info);
	void reload(int id, const Ice::Context& ctx = Ice::Context());
	UserRightManagerPrx getUserRight10sManager(int userId);		
	UserRightManagerPrx getUserRightManager(int userId);
	UserRightManagerPrx getUserRightManagerOneway(int userId);

};

}
}
}
#endif

