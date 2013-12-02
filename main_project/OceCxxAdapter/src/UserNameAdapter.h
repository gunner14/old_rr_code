/*
 * =====================================================================================
 *
 *       Filename:  UserBaseAdapter.h
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


#ifndef __USER_NAME_ADAPTER_H__
#define __USER_NAME_ADAPTER_H__

#include "UserNameInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace username {

using namespace ::xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserNameAdapter : public MyUtil::ReplicatedClusterAdapterI<UserNameManagerPrx>,
	public MyUtil::Singleton<UserNameAdapter> {

public:
	UserNameAdapter() : MyUtil::ReplicatedClusterAdapterI<UserNameManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel,true,"UNM"){
	}
	void reload(int id, const Ice::Context& ctx = Ice::Context());
	
  UserNameInfoPtr getUserName(int uid, const Ice::Context& ctx = Ice::Context());
  void setUserName(int id, const UserNameInfoPtr& info);
  void createUserName(int id, const UserNameInfoPtr& info);
		
	UserNameManagerPrx getUserNameManager(int userId);
	UserNameManagerPrx getUserName10sManager(int userId);
	UserNameManagerPrx getUserNameManagerOneway(int userId);
};

}
}
}

#endif

