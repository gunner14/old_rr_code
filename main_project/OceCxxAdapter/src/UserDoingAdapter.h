/*
 * =====================================================================================
 *
 *       Filename:  UserDoingAdapter.h
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


#ifndef __USER_DOING_ADAPTER_H__
#define __USER_DOING_ADAPTER_H__

#include "UserDoingInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include <IceUtil/IceUtil.h>

namespace xce {
namespace adapter {
namespace userdoing {

using namespace ::xce::userbase;
using namespace MyUtil;

class UserDoingAdapter : public MyUtil::ReplicatedClusterAdapterI<UserDoingManagerPrx>,
	public MyUtil::Singleton<UserDoingAdapter>
{
public:
	UserDoingAdapter() : MyUtil::ReplicatedClusterAdapterI<UserDoingManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel,true,"UDM"){
	}
	UserDoingInfoPtr getUserDoing(int userId, const Ice::Context& ctx = Ice::Context());
	void setUserDoing(int userId, const UserDoingInfoPtr& ptr);
	void createUserDoing(int userId, const UserDoingInfoPtr& ptr);
	void reload(int userId);
	UserDoingManagerPrx getUserDoing10sManager(int userId);	
	UserDoingManagerPrx getUserDoingManager(int userId);
	UserDoingManagerPrx getUserDoingManagerOneway(int userId);
};

}
}
}

#endif

