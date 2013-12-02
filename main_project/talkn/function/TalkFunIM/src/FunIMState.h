#ifndef __FUN_IM_STATE__
#define __FUN_IM_STATE__

#include "TaskManager.h"
#include <Ice/Ice.h>
#include <IceUtil/Mutex.h>
#include "FunIMCommon.h"
#include "TalkCommon.h"
#include "TalkLogic.h"
#include "Singleton.h"
#include "FeedMemcProxy/client/talk_cache_client.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace im {
namespace state {

using namespace com::xiaonei::talk::function::im;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::cache::loader;
using namespace MyUtil;

class NotifyUserStateChanged : public Task {
  // 发送用户在线状态变化消息
public:
  NotifyUserStateChanged(const AvatarPtr& avatar) : avatar_(avatar) {}
  bool NotifyPrepare();
  void handle();

private:
  // 原Online presenceTask成员
  AvatarPtr avatar_;
  OnlineStat old_stat_;
  OnlineStat new_stat_;
  // 原Presence PresenceNotifyTask成员
  AvatarSeq self_other_avatars_;
  JidSeqMap buddies_;
  NotifyType type_;
  TalkUserPtr user_;
  MessageSeq msg_seq_;
  Str2StrMap paras_;
  bool is_web_avatar_;

  void NotifyItsOtherAvatars();
  void NotifyAvatarToBuddies();
  void NotifyOtherAvatarsToBuddies();
};
typedef IceUtil::Handle<NotifyUserStateChanged> NotifyUserStateChangedPtr;

class FunIMState : public Singleton<FunIMState> {
public:
  // 处理用户状态变化 替代online presence
  void ProcessOtherUserStateChanged(const JidPtr& jid, OnlineStat new_stat, const string& ver, 
    const string& ext);
  // 用户Doing改变
  void NotifyWebOnlineForProcessUserDoingChanged(Ice::Int user_id);
  void SendOnlineBuddyAndOfflineMessageToClient(const JidPtr& jid, const string& show, const string& ver, 
    const string& subver, const string& ext, const string& secver);
  void ProcessClientUserStateChanged(const Str2StrMap& paras);
  void ProcessUserOffline(const JidPtr& jid);
  void KickUserWhenHisConfigChanged(int userId, int userStatus);
}; 

class NotifyWebOfflineForProcessUserDoingChanged : public Timer,
  public Singleton<NotifyWebOfflineForProcessUserDoingChanged> {
public:
  // 合并原WebOnlineEvictorTask和BatchOfflineNotifyTask
  NotifyWebOfflineForProcessUserDoingChanged() : Timer(2000) {}
  virtual void handle();
  void push(int userId);
private:
  vector<int> users_;
  IceUtil::Mutex mutex_;
};

class KickUserTimer : public MyUtil::Timer, public Singleton<KickUserTimer>{
public:
  KickUserTimer():Timer(30*1000){}
  void handle();
  void push(int userid){
    IceUtil::Mutex::Lock lock(mutex_);
    users_.push_back(make_pair<int, time_t>(userid, time(NULL)));
  }
public:
  IceUtil::Mutex mutex_;
  list<pair<int, time_t> > users_;
};

};
};
};
};
};
};
#endif
