#ifndef __TALK_CHECK_ADD_FRIEND_ADAPTER__
#define __TALK_CHECK_ADD_FRIEND_ADAPTER__

#include "Singleton.h"
#include "AdapterI.h"
#include "TalkCheckAddFriend.h"

namespace talk {
namespace adapter {

using namespace MyUtil;
using namespace com::renren::antispam::friendcode::generation;

class TalkCheckAddFriendAdapter : public MyUtil::AdapterI, 
  public MyUtil::AdapterISingleton<MyUtil::MsgFilterChannel, TalkCheckAddFriendAdapter> {

public:
  CheckAddFriendPrx getManager(int id);
  CheckAddFriendPrx getManagerOneway(int id);
  bool isNeedCode(int userId);
private:
  virtual string name() {
    return "CheckAddFriend";
  }
  virtual string endpoints() {
    return "@CheckAddFriend";
  }
  virtual size_t cluster() {
    return 0;
  }
  vector<CheckAddFriendPrx> _managersOneway;
  vector<CheckAddFriendPrx> _managers;
};

};
};

#endif
