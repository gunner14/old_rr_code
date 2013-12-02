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


#ifndef __USER_PASSPORT_ADAPTER_H__
#define __USER_PASSPORT_ADAPTER_H__

#include "UserPassportInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace xce {
namespace adapter {
namespace userpassport {

using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserPassportAdapter : virtual public MyUtil::ReplicatedClusterAdapterI<UserPassportManagerPrx>,
	virtual public MyUtil::Singleton<UserPassportAdapter>
{
public:
	UserPassportAdapter() : MyUtil::ReplicatedClusterAdapterI<UserPassportManagerPrx>( "ControllerUserBase",120,300,new MyUtil::OceChannel,true,"UPM"){
	}
	void reload(int id, const Ice::Context& ctx = Ice::Context());
	UserPassportInfoPtr getUserPassport(int uid, const Ice::Context& ctx = Ice::Context());
	void setUserPassport(int userId, const UserPassportInfoPtr& ptr);
	void createUserPassport(int userId, const UserPassportInfoPtr& ptr);
	UserPassportManagerPrx getUserPassport10sManager(int userId);
	UserPassportManagerPrx getUserPassportManager(int userId);
	UserPassportManagerPrx getUserPassportManagerOneway(int userId);

private:
//private:
		
	//UserPassportManagerPrx getUserPassport10sManager(int userId);
	//UserPassportManagerPrx getUserPassportManager(int userId);
	//UserPassportManagerPrx getUserPassportManagerOneway(int userId);

};

}
}
}

#endif

