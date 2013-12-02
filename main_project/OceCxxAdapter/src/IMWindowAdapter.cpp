#include "IMWindowAdapter.h"
#include <boost/lexical_cast.hpp>
#include <string>
using namespace MyUtil;
using namespace talk::window;
using namespace boost;
using namespace std;
IMWindowManagerPrx IMWindowAdapter::getManager(int id) {
  return locate<IMWindowManagerPrx> (_managers, "M", id, TWO_WAY);
}

IMWindowManagerPrx IMWindowAdapter::getManagerOneway(int id) {
  return locate<IMWindowManagerPrx> (_managersOneway, "M", id, ONE_WAY);
}

void IMWindowAdapter::IMWindowReminder(const JidPtr& jid) {
	getManagerOneway(jid->userId)->IMWindowReminder(jid);
}

void IMWindowAdapter::ShowNotify(const NotifyInfoPtr ni, const int uid) {
	getManagerOneway(0)->ShowNotify(ni, uid);
}

void IMWindowAdapter::ReloadConfig() {
	getManagerOneway(0)->ReloadConfig();
}
void IMWindowAdapter::SendApplist(string info, const JidPtr &jid) {
	getManagerOneway(jid->userId)->SendApplist(info, jid);
}
void IMWindowAdapter::ShowWindow(const string window_title, const int uid) {
	getManagerOneway(uid)->ShowWindow(window_title, uid);
}
