#ifndef IMWINDOWTESTADAPTER_H__
#define IMWINDOWTESTADAPTER_H__

#include "Singleton.h"
#include "AdapterI.h"
#include "IMWindow.h"

namespace talk {
namespace window{


using namespace MyUtil;
using namespace com::xiaonei::talk::common;

class IMWindowAdapter: public MyUtil::AdapterI, public MyUtil::AdapterISingleton<MyUtil::TalkChannel, IMWindowAdapter> {
public:
  IMWindowAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }

  void ReloadConfig();
  void SendApplist(string info, const JidPtr &jid);
  void ShowWindow(const string window_title, const int uid);
  void ShowNotify(const NotifyInfoPtr ni, const int uid);
  void IMWindowReminder(const JidPtr& jid);

	IMWindowManagerPrx getManager(int id);
	IMWindowManagerPrx getManagerOneway(int id);
private:

  virtual string name() {
    return "M";
  }
  virtual string endpoints() {
    return "@IMWindow";
  }
  virtual size_t cluster() {
    return 1;
  }
  vector<IMWindowManagerPrx> _managersOneway;
  vector<IMWindowManagerPrx> _managers;
};

};
};


#endif
