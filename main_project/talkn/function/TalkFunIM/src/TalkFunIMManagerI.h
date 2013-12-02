#ifndef __TALK_FUN_IM_MANAGERI__
#define __TALK_FUN_IM_MANAGERI__

#include "ServiceI.h"
#include "TalkFunction.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace im {

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;

class TalkFunIMManagerI : public TalkFunIMManager, public Singleton<TalkFunIMManagerI> {
public:
  virtual void notify(const Str2StrMap &paras, const Ice::Current& = Ice::Current());
  // 兼容老接口
  // PresenceMsgHandler
  virtual void presenceSubscribe(const string& id, Ice::Int from, Ice::Int to, 
    const string& status, const Ice::Current& = Ice::Current());
  virtual void presenceSubscribeWithPostscript(const string& id, Ice::Int from, Ice::Int to, 
    const string& status, const string& postscript, const Ice::Current& = Ice::Current());
  virtual void presenceSubscribed(const string& id, Ice::Int from, Ice::Int to, 
    const Ice::Current& = Ice::Current());
  virtual void presenceUnsubscribed(const string& id, Ice::Int from, Ice::Int to, 
    const Ice::Current& = Ice::Current());
  virtual void webPresence(Ice::Int userId, const Ice::Current& current = Ice::Current());
  virtual void presence(const JidPtr& jid, const string& show, const string& ver,
    const string& subver, const string& ext, const Ice::Current& = Ice::Current());
  virtual void presenceWithSecver(const JidPtr& jid, const string& show, const string& ver,
    const string& subver, const string& ext, const string& secver, const Ice::Current& = Ice::Current());
  virtual void offline(const JidPtr& jid, const Ice::Current& = Ice::Current());
  virtual void userStatusChanged(int userId, int userStatus, const Ice::Current& = Ice::Current());
  // OnlineCenter
  virtual void OnlineCenterPresence(const JidPtr& jid, OnlineStat stat, const string& ver, 
    const string& ext, const Ice::Current& = Ice::Current());
};

};
};
};
};
};

#endif
