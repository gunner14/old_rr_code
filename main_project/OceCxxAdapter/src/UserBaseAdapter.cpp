/*
 * UserBaseAdapter.cpp
 *
 */
#include "UserBaseAdapter.h"
#include "Date.h"

using namespace xce::adapter::userbase;

UserBaseFullViewPtr UserBaseAdapter::getUserBaseFullView(int id, const Ice::Context& ctx)  {
	return new UserBaseFullView(getUserBaseViewManager(id)->getUserBaseView3(id, ctx));
}

UserBaseCommonViewPtr UserBaseAdapter::getUserBaseCommonView(int id, const Ice::Context& ctx) {
	return new UserBaseCommonView(getUserBaseViewManager(id)->getUserCommonViewN(id, ctx));
}

UserBaseViewManagerPrx UserBaseAdapter::getUserBaseViewManager(int id)
{
	return getProxy(id);
}

UserBaseViewManagerPrx UserBaseAdapter::getUserBaseViewManagerOneway(int id)
{
	return getProxyOneway(id);
}

