#ifndef __TALK_GROUP_LOADER_ADMINMSG_I_H__
#define __TALK_GROUP_LOADER_ADMINMSG_I_H__

#include "ServiceI.h"
#include "TalkGroupAdminMsgLoader.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace group {
namespace admmsgloader{
using namespace std;
using namespace MyUtil;
//using namespace xce::talk;
using namespace com::xiaonei::talk::common;
const char *DB_INSTANCE = "im_groupchat";
const char *DB_ROUTE = "session_admin_message";
const char *DB_PREF = DB_ROUTE;

class TalkGroupAdminMsgLoaderI : public TalkGroupAdminMsgLoader, public Singleton<TalkGroupAdminMsgLoaderI> {

  enum {
    TABLE_COUNT = 100,
  };
  bool saveMsg_(int uid, const string &msg);
  bool delMsg_(int uid);
  bool getMsg_(int uid, bool del, MyUtil::StrSeq &msgs);
  string EscapeString(const string &raw_string, const bool mode=0); 
 public:
  virtual MyUtil::StrSeq getMsg(int uid, const Ice::Current& = Ice::Current());
  virtual MyUtil::StrSeq getdelMsg(int uid, const Ice::Current& = Ice::Current());
  virtual bool saveMsg(int uid, const string &msg, const Ice::Current& = Ice::Current());
  virtual void saveMsgAsyn(int uid, const string &msg, const Ice::Current& = Ice::Current());
  virtual void delMsgAsyn(int uid, const Ice::Current& = Ice::Current());
  virtual bool delMsg(int uid, const Ice::Current& = Ice::Current());

};
}
;
}
;
}
;
}
;
}
;

#endif

