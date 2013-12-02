#include "FunIMState.h" 
#include <sys/time.h>
#include "XmppTools.h"
#include "Util.h"
#include "FunIMCommon.h"
#include "TalkFunStateAdapter.h"
#include "FeedMemcProxy/client/talk_cache_client.h"
#include "PhotoUrlTools.h"
#include "IMStorm.h"
#include "MessageType.h"
#include "IMStormAdapter.h"
#include "TalkStatAdapter.h"
#include "TalkProtoAdapter.h"
#include "MessageHolderAdapter.h"
#include "OnlineEventMediatorAdapter.h"
#include "TalkProxyAdapter.h"

using namespace MyUtil;
using namespace com::xiaonei::talk::function::im::state;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::function::im;
using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::talk::cache;
using namespace com::xiaonei::talk::cache::loader;
using namespace xce::talk;
using namespace com::xiaonei::talk::util;
using namespace talk::stat;
using namespace com::xiaonei::talk::proto;
using namespace talk::adapter;
using namespace com::xiaonei::talk::adapter;
using namespace xce::mediator::onlineevent;

//-------------------NotifyUserStateChanged-----------------
bool NotifyUserStateChanged::NotifyPrepare() {
  // 原Online presenceTask功能
  const string FuncName = "NotifyUserStateChanged::NotifyPrepare";

  if(!avatar_)
    return false;

  bool need_ver_ext = false;
  is_web_avatar_ = (avatar_->jid->endpoint == "web");
  if(avatar_->ver.empty() || is_web_avatar_)
    need_ver_ext = true;

  try { 
    self_other_avatars_ = TalkFunStateAdapter::instance().getUserAvatars(avatar_->jid->userId);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkFunStateAdapter::GetUserOnlineInfo--> " << e);
    return false;
  } 

  if(self_other_avatars_.size() == 0) {
    old_stat_ = OFFLINE;
  } else {
    old_stat_ = ONLINE;
    for(AvatarSeq::const_iterator i = self_other_avatars_.begin(); i != self_other_avatars_.end(); ++i) {
      if(need_ver_ext && (*i)->ver > avatar_->ver) {
        avatar_->ver = (*i)->ver;
        avatar_->ext = (*i)->ext;
      }
      if((*i)->stat < old_stat_)
        old_stat_ = ((*i)->stat);
    }
  }

  if(is_web_avatar_)
    new_stat_ = old_stat_;
  else
    new_stat_ = avatar_->stat;
 
  if(old_stat_ == INVISIABLE && new_stat_ == INVISIABLE) {
    // 隐身，状态修改只通知给自己
    type_ = AvatarToItsOtherAvatars;
  } else {
    if(old_stat_ != INVISIABLE && new_stat_ != INVISIABLE)
      // 非隐身，只把修改状态的副本通知给好友
      type_ = AvatarToAll;
    else
      // 隐身状态改变或非隐身到隐身切换，将所有在线副本通知给好友
      type_ = AllAvatarsToBuddies;
    try {
      buddies_ = TalkFunStateAdapter::instance().getBuddyJids(avatar_->jid->userId, IM_ONLINE);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "--> TalkFunStateAdapter.getBuddyAvatar id" << avatar_->jid->userId << e);
      return false;
    }
  }

  // 只有更改Doing时才通知给自己
  if(!is_web_avatar_) 
    for(AvatarSeq::iterator it = self_other_avatars_.begin(); it != self_other_avatars_.end(); ++it)
      if((*it)->jid->index == avatar_->jid->index ) {
        self_other_avatars_.erase(it);
        break;
      }

  MCE_DEBUG(FuncName << " self_other_avatars size: " << self_other_avatars_.size() << 
    " buddies_ size: " << buddies_.size() << " type: " << type_ << "new_stat: " 
    << new_stat_ << "old_stat: " << old_stat_);
 
  return true;
}

void NotifyUserStateChanged::handle() {
  const string FuncName = "NotifyUserStateChange::handle";

  try {
    user_ = TalkCacheClient::instance().GetUserByIdWithLoad(avatar_->jid->userId);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkCacheClient::GetUserBySeqWithLoad--> id:" << avatar_->jid->userId << e);
    return;
  }
  if(user_) {
    paras_["DOING"] = user_->statusOriginal;
    paras_["PHOTO"] = buildHeadUrl(user_->headurl);
  }

  CheckVip(paras_["VIP"], avatar_->jid->userId);
  paras_["HTMLDOING"] = xmlEncode(user_->statusShifted.c_str());

  switch(type_){
    case AllAvatarsToBuddies:
      NotifyAvatarToBuddies();
      NotifyItsOtherAvatars();
      NotifyOtherAvatarsToBuddies();
      break;
    case AvatarToAll:
      NotifyAvatarToBuddies();
      NotifyItsOtherAvatars();
      break;
    case AvatarToItsOtherAvatars:
      NotifyItsOtherAvatars();
      break;
    default:
      MCE_WARN(FuncName << "Unknown type");
  }

  // 给3G发送状态变化消息
  //如果是上下线的消息 获取actor的在线状态
  if(!is_web_avatar_ && (ONLINE == new_stat_ || OFFLINE == new_stat_ || INVISIABLE == new_stat_)) {
    int activetype = 0;
    OnlineStat activestat = OFFLINE;
    for(AvatarSeq::iterator ait = self_other_avatars_.begin(); ait != self_other_avatars_.end(); ++ait) {
      if((*(ait))->jid->endpoint[0] == 'W') {
        activetype |= 2;
        activestat = ONLINE;
      }
      else if((*(ait))->jid->endpoint[0] == 'T') {
        activetype |= 4;
        if((*(ait))->stat > INVISIABLE)
          activestat = ONLINE;
      }
      else if((*(ait))->jid->endpoint[0] == 'P') {
        activetype |= 8;
        activestat = ONLINE;
      }
    }
    if(ONLINE == new_stat_) {
      activetype |= GetOnlineTypeFromEndpoint(avatar_->jid->endpoint);
      activestat = ONLINE;
    }

    if(OFFLINE == activestat)
      activetype = 0;
    OnlineStatNotify stat;
    stat.uid = avatar_->jid->userId;
    stat.stat = activestat;
    stat.type = activetype;
    MCE_INFO("Send Stat To 3G-->" << stat.uid << " stat: " << stat.stat << " type " << stat.type);
    try {
      IMStormAdapter::instance().notifyonlinestat(stat);
    } catch(Ice::Exception& e) {
      MCE_WARN("NotifyUserStateChanged::handle-->IMStormAdapter::notifyonlinestat-->error:" << e);
    }
  }
} 

void NotifyUserStateChanged::NotifyItsOtherAvatars() {
  const string FuncName = "NotifyUserStateChanged::NotifyItsOtherAvatars";
  MCE_DEBUG(FuncName);

  if(self_other_avatars_.empty())
    return;

  Str2StrMap msg;
  string tpl = "NotifyUserStateChanged";
  if(avatar_->stat == OFFLINE) {
    msg["type"] = "unavailable";
    tpl = "NotifyUserStateChanged_unavailable";
  } else {
    if(!paras_["DOING"].empty())
      msg["status"] = paras_["DOING"];
    if(!paras_["HTMLDOING"].empty())
      msg["richstatus"] = paras_["HTMLDOING"];
    if(!paras_["VIP"].empty())
      msg["VIP"] = paras_["VIP"];
    msg["EXTVAL"] = paras_["PHOTO"];
    msg["ver"] = avatar_->ver;
    msg["ext"] = avatar_->ext;
    msg["show"] = onlineStatStr(avatar_->stat);
  }
  
  vector<string>  tmp;
  for(size_t i = 0; i < self_other_avatars_.size(); ++i) {
    if(self_other_avatars_.at(i)->jid->endpoint[0] != 'T')
      continue; 
    tmp.push_back(jidToString(self_other_avatars_.at(i)->jid));
    if(tmp.size() > 500) {
      SendToProtoExpressWithJids(FuncName, msg, PRESENCE_COMMON, tpl, jidToString(avatar_->jid), tmp);
      tmp.clear();
    }
  }
  if(tmp.size() > 0)
    SendToProtoExpressWithJids(FuncName, msg, PRESENCE_COMMON, tpl, jidToString(avatar_->jid), tmp);
}

void NotifyUserStateChanged::NotifyAvatarToBuddies() {
  const string FuncName = "NotifyUserStateChanged::NotifyAvatarToBuddies";
  MCE_DEBUG(FuncName);

  if(buddies_.empty())
    return;
  
  Str2StrMap msg;
  string tpl = "NotifyUserStateChanged";
  if(avatar_->stat == OFFLINE || avatar_->stat == INVISIABLE) {
    msg["type"] = "unavailable";
    tpl = "NotifyUserStateChanged_unavailable";
  } else {
    if(!paras_["DOING"].empty())
      msg["status"] = paras_["DOING"];
    if(!paras_["HTMLDOING"].empty())
      msg["richstatus"] = paras_["HTMLDOING"];
    if(!paras_["VIP"].empty())
      msg["VIP"] = paras_["VIP"];
    msg["EXTVAL"] = paras_["PHOTO"];
    msg["ver"] = avatar_->ver;
    msg["ext"] = avatar_->ext;
    msg["show"] = onlineStatStr(avatar_->stat);
  }

  vector<string> tmp;
  for(JidSeqMap::const_iterator it = buddies_.begin(); it != buddies_.end(); ++it) {
    for(size_t i = 0; i < it->second.size(); ++i) {
      tmp.push_back(jidToString(it->second.at(i)));
      if(tmp.size() > 500) {
        SendToProtoExpressWithJids(FuncName, msg, PRESENCE_COMMON, tpl, jidToString(avatar_->jid), tmp);
        tmp.clear();
      }
    }
  }
  if(tmp.size() > 0)
    SendToProtoExpressWithJids(FuncName, msg, PRESENCE_COMMON, tpl, jidToString(avatar_->jid), tmp);
}

void NotifyUserStateChanged::NotifyOtherAvatarsToBuddies() {
  const string FuncName = "NotifyUserStateChanged::NotifyOtherAvatarsToBuddies";
  MCE_DEBUG(FuncName);

  if(self_other_avatars_.empty() || buddies_.empty())
    return;

  bool invisible = false;
  if(avatar_->stat == INVISIABLE)
      invisible = true;

  Str2StrMap msg;
  string tpl = "NotifyUserStateChanged";
  if(invisible) {
    msg["type"] = "unavailable";
    tpl = "NotifyUserStateChanged_unavailable";
  } else {
    if(!paras_["DOING"].empty())
      msg["status"] = paras_["DOING"];
    if(!paras_["HTMLDOING"].empty())
      msg["richstatus"] = paras_["HTMLDOING"];
    if(!paras_["VIP"].empty())
      msg["VIP"] = paras_["VIP"];
    msg["EXTVAL"] = paras_["PHOTO"];
  }

  vector<string> tmp;
  for(JidSeqMap::const_iterator it = buddies_.begin(); it != buddies_.end(); ++it) {
    for(size_t i = 0; i < it->second.size(); ++i) {
      tmp.push_back(jidToString(it->second.at(i)));

      if(tmp.size() > 500) {
        for(size_t j = 0; j < self_other_avatars_.size();  ++j){
          if(!invisible) {
            msg["ver"] = self_other_avatars_.at(j)->ver;
            msg["ext"] = self_other_avatars_.at(j)->ext;
            msg["show"] = onlineStatStr(self_other_avatars_.at(j)->stat);
          }

          SendToProtoExpressWithJids(FuncName, msg, PRESENCE_COMMON, tpl,
            jidToString(self_other_avatars_.at(j)->jid), tmp);
        }
        tmp.clear();
      }
    }
  }
  if(tmp.size() > 0) {
    for(size_t j = 0; j < self_other_avatars_.size();  ++j){
      if(!invisible) {
        msg["ver"] = self_other_avatars_.at(j)->ver;
        msg["ext"] = self_other_avatars_.at(j)->ext;
        msg["show"] = onlineStatStr(self_other_avatars_.at(j)->stat);
      }
      SendToProtoExpressWithJids(FuncName, msg, PRESENCE_COMMON, tpl, 
        jidToString(self_other_avatars_.at(j)->jid), tmp);
    }
  }
}

//--------------------------FunIMState-------------------------
// 其他终端在各自接入模块调用上线函数,这里只是发送上线通知
void FunIMState::ProcessOtherUserStateChanged(const JidPtr& jid, OnlineStat new_stat, const string& ver, 
  const string& ext) {
  const string FuncName = "FunIMState::ProcessOtherUserStateChanged";
  TimeStatN ts;

  if(!jid)
    return;

  // 除renren桌面外，其他终端只有离线/在线两种状态，不需要判断是否是隐身状态
  // 所以不需要在之前获得旧状态。
  AvatarPtr avatar = new Avatar;
  avatar->jid = jid;
  avatar->stat = new_stat;
  avatar->ver = ver;
  avatar->ext = ext;
  try {
    NotifyUserStateChangedPtr notifier = new NotifyUserStateChanged(avatar);
    if(notifier && notifier->NotifyPrepare())
      TaskManager::instance().execute(notifier);
  } catch(...) {
    MCE_WARN(FuncName << "--> NotifyUserStateChanged Task error");
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

void FunIMState::NotifyWebOnlineForProcessUserDoingChanged(Ice::Int user_id) {
  const string FuncName = "FunIMState::NotifyWebOnlineForProcessUserDoingChanged";
  MCE_INFO(FuncName << "id : " << user_id);
  TimeStatN ts;

  AvatarPtr avatar;
  JidPtr jid;
  try {
    avatar = new Avatar();
    jid = new Jid();
  } catch(...) {
    MCE_WARN(FuncName << " new Avatar or Jid error");
  }
  if(avatar && jid) {
    jid->userId = user_id;
    jid->endpoint = "web";
    jid->index = 1; 
    avatar->jid = jid;
    avatar->ver = "";
    avatar->ext = "";
    avatar->stat = ONLINE;
    avatar->stamp = time(NULL);
  }
  // 通知web Jid更新下用户的Doing
  try {
    NotifyUserStateChangedPtr notifier = new NotifyUserStateChanged(avatar);
    if(notifier && notifier->NotifyPrepare())
      TaskManager::instance().execute(notifier);
  } catch(...) {
    MCE_WARN(FuncName << "--> NotifyUserStateChanged Task error");
  }

  NotifyWebOfflineForProcessUserDoingChanged::instance().push(jid->userId);

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

void FunIMState::SendOnlineBuddyAndOfflineMessageToClient(const JidPtr& jid, const string& show, const string& ver, 
  const string& subver, const string& ext, const string& secver) {
  const string FuncName = "FunIMState::SendOnlineBuddyAndOfflineMessageToClient";
  TimeStatN ts;

  MCE_INFO(FuncName << "--> " << jidToString(jid)
    << " show:" << show
    << " ver:" << ver << " subver:" << subver
    << " ext:" << ext << "  secver:"<< secver);

  // 处理版本号
  string merged_ver = ver;
  if (secver != "") {
    const size_t point_pos = merged_ver.find_first_of('.');
    if (point_pos != string::npos) {
      const size_t non_zero_pos = merged_ver.find_first_not_of('0', point_pos+1);
      if (non_zero_pos!=string::npos)
        merged_ver.erase(point_pos+1, non_zero_pos - point_pos-1);
    }
    merged_ver += ".";
    merged_ver += secver;
  }
  try {
    TalkStatAdapter::instance().versionStat(jid->userId, merged_ver, subver);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkStatAdapter::instance().versionStat--> " << jidToString(jid) << " Ice::Exception : " << e);
  }

  // 获取在线好友信息
  AvatarMap avatars;
  try {
    avatars = TalkFunStateAdapter::instance().getBuddyAvatarsAndSelf(jid->userId);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkFunStateAdapter::getBuddyAvatarsAndSelf-->userId:" << jid->userId << " " << e);
    return;
  } catch(...) {
    MCE_WARN(FuncName << "-->TalkFunStateAdapter::getBuddyAvatarsAndSelf--->userId:" << jid->userId << " unknow exception");
    return;
  }

  AvatarMap::iterator ait = avatars.begin(); 
  while(true) {
    // 每次最多处理30个好友
    MyUtil::IntSeq online_ids;
    int count = 0; 
    for(;ait != avatars.end() && count < 30; ++ait, ++count)
      online_ids.push_back(ait->first);
    if(count == 0)
      break;

    TalkUserMap usermaps;
    try {
      usermaps = TalkCacheClient::instance().GetUserBySeqWithLoad(jid->userId, online_ids);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "-->TalkCacheClient::GetUserBySeqWithLoad-->" << jid->userId << " " <<e);
      continue;
    } catch(...) {
      MCE_WARN(FuncName << "-->TalkCacheClient::GetUserBySeqWithLoad-->" << jid->userId << " unknow exception");
      continue;
    }

    map<int, UserCachePtr> user_vip;
    try {
      user_vip = UserCacheAdapter::instance().GetUserCache(online_ids);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "-->UserCacheAdapter::GetUserCache-->userId:" << jid->userId << " Ice::Exception:" << e);
      continue;
    } catch(...) {
      MCE_WARN(FuncName << "-->UserCacheAdapter::GetUserCache-->userId:" << jid->userId << " unknown Exception:");
      continue;
    }

    // 构造消息
    Str2StrMapSeq msgs;
    for(TalkUserMap::iterator it = usermaps.begin(); it != usermaps.end(); ++it) {
      TalkUserPtr u = it->second;
      AvatarMap::iterator eit = avatars.find(u->id);
      if (eit != avatars.end()) {
        // 只要有一个副本隐身，则视为不在线
        bool isInvisiable = false;
        for (size_t j = 0; j < eit->second.size(); ++j) {
          if(eit->second.at(j)->stat == INVISIABLE) {
            isInvisiable = true;
            break;
          }
        }
        if(isInvisiable)
          continue;

        string vip;
        CheckVipWithoutGet(vip, u->id, user_vip);
        for (size_t j = 0; j < eit->second.size(); ++j) {
          // 跳过同一在线副本
          if(eit->first == jid->userId && eit->second.at(j)->jid->index == jid->index)
            continue;

          Str2StrMap msg;
          msg["status"] = u->statusOriginal;
          string show = onlineStatStr(eit->second.at(j)->stat);
          if (show != "")
          msg["show"] = show;
          msg["nick"] = u->name;
          msg["EXTVAL"] = buildHeadUrl(u->headurl);
          msg["ver"] = eit->second.at(j)->ver;
          msg["ext"] = eit->second.at(j)->ext;
          msg["richstatus"] = xmlEncode(u->statusShifted);
          msg["vip"] = vip;
          LexicalCast<int, string>(PRESENCE_COMMON, msg["mesage_type"]);
          msg["config_type"] = TALKFUNIM_TPL_TYPE;
          msg["item_type"] = "OnlineBuddyPresence";
          msg["from_jid"] = jidToString(eit->second.at(j)->jid);
          msg["to_jid"] = jidToString(jid);
          msgs.push_back(msg);
        }
      }
    }

    try {
      TalkProtoAdapter::instance().BatchReceiveExpress(msgs);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "-->TalkProtoAdapter::Receive:: error: " << e);
    }
  }

  try {
    MessageHolderAdapter::instance().loadOfflineMessage(jid);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->MessageHolderAdapter::loadOfflineMessage-->" << e);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

// 处理人人桌面用户状态改变(上线 离开 忙碌 隐身)
void FunIMState::ProcessClientUserStateChanged(const Str2StrMap &paras) {
  const string FuncName = "FunIMState::ProcessClientUserStateChanged";
  TimeStatN ts;

  string jid_str, type, ver, subver, ext, show, secver;
  ParasGetValue("actor_jid", jid_str, paras);
  ParasGetValue("presence:type", type, paras);
  ParasGetValue("presence.c:ver", ver, paras);
  ParasGetValue("presence.c:ext", ext, paras);
  ParasGetValue("presence.c:subver", subver, paras);
  ParasGetValue("presence.c:secver", secver, paras);
 
  if (type == "invisible") {
    show = "invisible";
  } else if(type=="") {
    ParasGetValue("presence.show", show, paras);
  }

  MCE_INFO(FuncName << "-->" << jid_str << " show:" << show  << " ver:" 
    << ver << " subver:" << subver << " secver:" << secver << " ext:" << ext);
 
  JidPtr jid;
  try {
    jid = stringToJid(jid_str);
  } catch(...) {
    MCE_WARN(FuncName << "-->jidToString error");
    return;
  }

  AvatarPtr avatar = new Avatar;
  avatar->jid = jid;
  avatar->stat = onlineStat(show);
  avatar->ver = ver;
  avatar->ext = ext;
  try {
    NotifyUserStateChangedPtr notifier = new NotifyUserStateChanged(avatar);
    if(notifier && notifier->NotifyPrepare())
      TaskManager::instance().execute(notifier);
  } catch(...) {
    MCE_WARN(FuncName << "--> NotifyUserStateChanged Task error");
  }

  try{
    AvatarNPtr ava = new AvatarN();
    ava->jid = jid;
    ava->ext = ext;
    ava->ver = ver;
    ava->type=MemcGetOnlineType(jid->endpoint);
    ava->stat=MemcGetOnlineStat(show);
    ava->hearttimestamp = time(NULL);
    TalkFunStateAdapter::instance().AvatarNOnline(ava);
  }catch(Ice::Exception& e){
    MCE_WARN(FuncName << "-->MemcOnlineLogic--> userid:" << jid->userId << " error:" << e);
  }

  if (jid->endpoint[0] == 'W') {
    try {
      OnlineEventMediatorAdapter::instance().sendOnlineEvent(jid->userId,1);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "-->OnlineEventMediatorAdapter::sendOnlineEvent-->" << e);
    }
  } else {
    try{
      OnlineEventMediatorAdapter::instance().sendOnlineEvent(jid->userId,2);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "-->OnlineEventMediatorAdapter::sendOnlineEvent" << e);
    }
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

// 处理用户离线
void FunIMState::ProcessUserOffline(const JidPtr& jid) {
  const string FuncName = "FunIMState::ProcessUserOffline";
  TimeStat ts;

  if(!jid)
    return;
  MCE_INFO(FuncName << " --> " << jidToString(jid));

  AvatarPtr avatar = new Avatar;
  avatar->jid = jid;
  avatar->ext = "";
  avatar->ver = "";
  avatar->stat = OFFLINE;
  try {
    NotifyUserStateChangedPtr notifier = new NotifyUserStateChanged(avatar);
    if(notifier && notifier->NotifyPrepare())
      TaskManager::instance().execute(notifier);
  } catch(...) {
    MCE_WARN(FuncName << "--> NotifyUserStateChanged Task error");
  }

  try {
    if (jid && !jid->endpoint.empty() && jid->endpoint[0] != 'W')
      TalkFunStateAdapter::instance().AvatarNOffline(jid);
  } catch(Ice::Exception& e) {
    MCE_WARN("PresenceHandlerI::offline-->TalkFunStateAdapter::AvatarNOffline-->" << jidToString(jid) << "err" << e);
  }

  try{
    OnlineEventMediatorAdapter::instance().sendOnlineEvent(jid->userId, 0);
  }catch(Ice::Exception& e){
    MCE_WARN("PresenceHandlerI::offline-->OnlineEventMediatorAdapter::sendOnlineEvent-->" << jidToString(jid) << "error" << e);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

void FunIMState::KickUserWhenHisConfigChanged(int userId, int userStatus) {
  const string FuncName = "FunIMState::KickUserWhenHisConfigChanged";
  TimeStatN ts;
 
  if(userStatus == 0){
    return;
  }
  MCE_INFO(FuncName << "--> " << userId << " " << userStatus);
  KickUserTimer::instance().push(userId);

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

//--------------NotifyWebOfflineForProcessUserDoingChanged-------
void NotifyWebOfflineForProcessUserDoingChanged::push(int user_id) {
  IceUtil::Mutex::Lock lock(mutex_);
  users_.push_back(user_id);
}

void NotifyWebOfflineForProcessUserDoingChanged::handle() {
  const string FuncName = "NotifyWebOfflineForProcessUserDoingChanged";
  TimeStatN ts;
  vector<int> tmp_users;

  {
    IceUtil::Mutex::Lock lock(mutex_);
    users_.swap(tmp_users);
  }
  if(tmp_users.empty())
    return;

  JidPtr jid = new Jid;
  jid->index = 1;
  jid->endpoint = "web";
  Str2StrMap msg;
  for (size_t i=0; i<tmp_users.size(); i++) {
    jid->userId = tmp_users.at(i);
    JidSeqMap onlineMap;
    try {
      onlineMap = TalkFunStateAdapter::instance().getBuddyJidsAndSelf(tmp_users.at(i), IM_ONLINE, IM_ONLINE);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "-->TalkFunStateAdapter::getBuddyJidsAndSelf-->" << e);
    }

    if(onlineMap.empty())
      continue;

    vector<int> to_id;
    for(JidSeqMap::iterator it = onlineMap.begin(); it != onlineMap.end(); ++it)
      to_id.push_back(it->first);

    SendToProtoWithIds(FuncName, msg, PRESENCE_OFFLINE, "NotifyUserStateChanged_unavailable",
      jidToString(jid), to_id, IM_ONLINE);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

void KickUserTimer::handle(){
  while(true){
    pair<int, time_t> p;
    {
      IceUtil::Mutex::Lock lock(mutex_);
      if(users_.empty())
        break;
      p = users_.front();
      if((time(NULL)-p.second) < 60)
        break;
      users_.pop_front();
    }

    JidSeq jids;
    try {
      jids = TalkFunStateAdapter::instance().getUserJids(p.first, IM_ONLINE);
    } catch(Ice::Exception& e) {
      MCE_WARN("KickUserTimer::handle-->TalkFunStateAdapter::getUserJids-->" << e);
    }
    MCE_INFO("KickUserTimer::handle --> user:" << p.first << " onlinesize:" << jids.size() << " size:" << users_.size());
    if(jids.empty()) {
      continue;
    }
    for(size_t i = 0; i < jids.size(); i++) {
      try {
        MCE_INFO("KickUserTimer::handle -- > close " << jidToString(jids.at(i)));
        TalkProxyAdapter::instance().kick(jids.at(i));
      } catch(Ice::Exception& e) {
        MCE_WARN("KickUserTimer::handle-->TalkProxyAdapter::kick-->" << e << " " << jidToString(jids.at(i)));
      }
    }
  }
}
