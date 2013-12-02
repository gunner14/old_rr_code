#include "OzeinActivityAdapter.h"

using namespace MyUtil;
using namespace com::xiaonei::ozein::activity::generation;
using namespace std;

UserActivityPrx OzeinActivityAdapter::getManager(int id) {
	return locate<UserActivityPrx> (_managers, "Anti_Spam_Activity", id, TWO_WAY);
}

UserActivityPrx OzeinActivityAdapter::getManagerOneway(int id) {
	return locate<UserActivityPrx> (_managersOneway, "Anti_Spam_Activity", id, ONE_WAY);
}

int OzeinActivityAdapter::checkActivity(int fromid, int toid, int type, string content) {
  return getManager(toid)->checkActivity(fromid, toid, type, content);
}





