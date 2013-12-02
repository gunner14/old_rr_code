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


#ifndef __USER_URL_ADAPTER_H__
#define __USER_URL_ADAPTER_H__

#include "UserUrlInfo.h"
#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userurl{
using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserUrlAdapter : public MyUtil::ReplicatedClusterAdapterI<UserUrlManagerPrx>,
	public MyUtil::Singleton<UserUrlAdapter>
{
public:
	UserUrlAdapter() : MyUtil::ReplicatedClusterAdapterI<UserUrlManagerPrx>("ControllerUserBase",120,300,new MyUtil::OceChannel, true,"U"){
	}
	UserUrlInfoPtr getUserUrl(int userId, const Ice::Context& ctx = Ice::Context());
	void setUserUrl(int userid, const UserUrlInfoPtr& info);
  void createUserUrl(int userid, const UserUrlInfoPtr& info);
	void reload(int id, const Ice::Context& ctx = Ice::Context());

	UserUrlManagerPrx getUserUrlManager(int userId);
	UserUrlManagerPrx getUserUrl10sManager(int userId);
	UserUrlManagerPrx getUserUrlManagerOneway(int userId);
};

}
}
}
#endif

