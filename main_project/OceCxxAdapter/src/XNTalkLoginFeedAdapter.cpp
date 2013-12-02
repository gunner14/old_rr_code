#include "XNTalkLoginFeedAdapter.h"

using namespace MyUtil;
using namespace xce::xntalk;
LoginFeedManagerPrx LoginFeedAdapter::getManager(int id) {
  return locate<LoginFeedManagerPrx> (_managers, "M", id, TWO_WAY);
//  return getProxy(id);
}

LoginFeedManagerPrx LoginFeedAdapter::getManagerOneway(int id) {
  return locate<LoginFeedManagerPrx> (_managersOneway, "M", id, ONE_WAY);
//  return getProxyOneway(id);
}

void LoginFeedAdapter::SendFeed(int id) {
  getManagerOneway(id)->SendFeed(id);
}

void LoginFeedAdapter::ReloadConfig() {
  getManagerOneway(0)->ReloadConfig();
}

void LoginFeedAdapter::SendInstallFeed(int uid, string version) {
  getManagerOneway(0)->SendInstallFeed(uid, version);
}
