#include "UserConfigAdapter.h"

#include <boost/lexical_cast.hpp>

using namespace xce::adapter;
using namespace std;
using namespace mop::hi::svc::model;
using namespace MyUtil;
using namespace boost;

xce::adapter::UserConfigInfoPtr UserConfigAdapter::getUserConfig(int userid, const Ice::Context& ctx) {
  UserConfigInfoPtr info=new UserConfigInfo;
  info->parse(getUserConfigManager(userid)->getUserConfig(userid));
	return info;
}
 
void UserConfigAdapter::setUserConfig(int id, const UserConfigInfoPtr& info) {
  if (id <= 0) {
    return;
  }
  getUserConfigManager(id)->setUserConfig(id, info->demark());
}

void UserConfigAdapter::createUserConfig(int id, const UserConfigInfoPtr& info) {
  if (id <= 0) {
    return;
  }
  getUserConfig10sManager(id)->createUserConfig(info->demark());
}

void UserConfigAdapter::setUserConfig(int userId, const MyUtil::Str2StrMap& properties){
  if (userId <= 0) {
	return;
  }
  getUserConfig10sManager(userId)->setUserConfig(userId, properties);
}

void UserConfigAdapter::createUserConfig(int userId, const Str2StrMap& properties){
  if (userId <= 0) {
	return;
  }
  getUserConfig10sManager(userId)->createUserConfig(properties);
}

mop::hi::svc::model::UserConfigManagerPrx UserConfigAdapter::getUserConfig10sManager(int id) {
	return getProxy(id);
}

mop::hi::svc::model::UserConfigManagerPrx UserConfigAdapter::getUserConfigManager(int id) {
	return getProxy(id);
}

mop::hi::svc::model::UserConfigManagerPrx UserConfigAdapter::getUserConfigManagerOneway(int id) {
	return getProxyOneway(id);
}

void UserConfigAdapter::reload(int id, const Ice::Context& ctx)
{
     getUserConfigManager(id)->reload(id, ctx);
}
