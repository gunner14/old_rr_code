#ifndef __OZEINACTIVITY_H__
#define __OZEINACTIVITY_H__

#include "OzeinActivity.h"
#include "Singleton.h"
#include "AdapterI.h"

namespace com{
namespace xiaonei{
namespace ozein{
namespace activity{
namespace generation{
using namespace MyUtil;
using namespace std;
class OzeinActivityAdapter: public MyUtil::AdapterI, public MyUtil::AdapterISingleton<MyUtil::MsgFilterChannel, OzeinActivityAdapter> {
public:
	UserActivityPrx getManager(int id);
	UserActivityPrx getManagerOneway(int id);
  int checkActivity(int fromid, int toid, int type, string content);
private:
	virtual string name() {
		return "UserActivity";
	}
	virtual string endpoints() {
		return "@UserActivity";
	}
	virtual size_t cluster() {
		return 0;
	}
	vector<UserActivityPrx> _managersOneway;
	vector<UserActivityPrx> _managers;
};
};
};
};
};
};


#endif
