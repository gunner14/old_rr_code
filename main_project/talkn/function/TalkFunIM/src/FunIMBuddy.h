#ifndef __FUN_IM_BUDDY__
#define __FUN_IM_BUDDY__

#include "FunIMCommon.h"
#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace im {
namespace buddy {

using namespace MyUtil;
using namespace com::xiaonei::talk::function::im;

class FunIMBuddy : public Singleton<FunIMBuddy> {
public:
  // 处理人人桌面获取好友信息(vCard)请求
  void ProcessGetVcardRequest(const Str2StrMap& paras);
  // 处理人人桌面发送的好友申请
  void ProcessClientAddBuddyRequest(const Str2StrMap &paras);
  // 处理人人桌面发送的接受好友申请
  void ProcessClientAddBuddyAccept(const Str2StrMap &paras);
  // 处理人人桌面发送的拒绝好友申请
  void ProcessClientAddBuddyReject(const Str2StrMap &paras);
  // 向人人桌面发送好友申请消息
  void NotifyClientAddBuddyRequest(const string& id, Ice::Int from, Ice::Int to, 
    const string& status, const string& postscript);
  // 向人人桌面发送接受好友申请消息
  void NotifyClientAddBuddyAccept(const string& id, Ice::Int from, Ice::Int to);
  // 向人人桌面发送拒绝好友申请消息
  void NotifyClientAddBuddyReject(const string& id, Ice::Int from, Ice::Int to);
};

};
};
};
};
};
};

#endif
