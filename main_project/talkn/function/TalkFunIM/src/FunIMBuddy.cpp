#include "FunIMBuddy.h"
#include "XmppTools.h"
#include "TalkCommon.h" // Jid
#include "BuddyCoreAdapter.h" // BuddyCoreAdapter
#include "BuddyByIdReplicaAdapter.h" // BuddyByIdCacheAdapter
#include "BuddyFriendsAdapter.h" 
#include "NotifyGateAdapter.h"
#include "NotifyDispatcherAdapter.h"
#include "talk/SpamClient/src/OzeinFilterAdapter.h"
#include "PhotoUrlTools.h"
#include "FeedMemcProxy/client/talk_cache_client.h"// TalkCacheClient
#include "TalkProtoAdapter.h"
#include "TalkFunStateAdapter.h"
#include "MessageType.h"
#include "UserCacheAdapter.h"
#include "FunIMCommon.h"
#include "TalkCacheLoaderAdapter.h"

using namespace MyUtil;
using namespace com::xiaonei::talk::function::im::buddy;
using namespace com::xiaonei::talk::function::im;
using namespace com::xiaonei::ozein::content::generation;
using namespace xce::buddy::adapter;
using namespace xce::notify;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::cache::loader;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::service::buddy;
using namespace xce::talk;
using namespace com::xiaonei::talk::proto;
using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::talk::cache;
using namespace xce::usercache;

// ---------------- FunIMBuddy -----------------
void FunIMBuddy::ProcessGetVcardRequest(const Str2StrMap& paras) {
  const string FuncName = "FunIMBuddy::ProcessGetVcardRequest";
  TimeStatN ts;

  Str2StrMap msg;
  string from_jid, id;
  ParasGetValue("actor_jid", from_jid, paras);
  ParasGetValue("iq:id", id, paras);
  msg["id"] = id;

  string iq_to;
  ParasGetValue("iq:to", iq_to, paras);
  if (iq_to.empty()) {
    MCE_WARN(FuncName << "--> wrong, no to attribute");
    SendToProtoExpress(FuncName, msg, IQ_RESULT, "GetVcardRequestError", iq_to, from_jid);
    return;
  }

  JidPtr to_jid;
  try {
    to_jid = stringToJid(iq_to);
    if(!to_jid)
      return;
  } catch(...) {
    MCE_WARN(FuncName << "stringToJid error");
    return;
  }
  
  TalkUserPtr ui;
  try {
    ui = TalkCacheClient::instance().GetUserByIdWithLoad(to_jid->userId);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkCacheClient::GetUserByIdWithLoad-->" << to_jid->userId<< " err:"<<e);
  }
  if (!ui) {
    MCE_WARN(FuncName << "--> TalkCacheClient  UserNotExist:" << to_jid->userId);
    SendToProtoExpress(FuncName, msg, IQ_RESULT, "GetVcardRequestError", iq_to, from_jid);
    return;
  }
  if(to_jid->userId != ui->id) {
    MCE_WARN(FuncName << "--> memcerror userid = " << to_jid->userId << " error_id = " << ui->id);
    try {
      ui = TalkCacheLoaderAdapter::instance().LoadById(to_jid->userId);
    } catch(Ice::Exception& e) {
      MCE_WARN(FuncName << "-->TalkCacheLoaderAdapter::LoadById-->" << to_jid->userId << ":" << e);
    }
  }
  
  msg["EXTVAL"] = buildHeadUrl(ui->headurl);
  msg["STATUS"] = ui->statusOriginal;
  msg["FN"] = ui->name;
  msg["richstatus"] = xmlEncode(ui->statusShifted.c_str());

  string vip;
  CheckVip(vip, to_jid->userId);
  if(!vip.empty())
    msg["vip"] = vip;
 
  SendToProtoExpress(FuncName, msg, IQ_RESULT, "GetVcardRequest", iq_to, from_jid);
  
  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 100.0));
}

void FunIMBuddy::ProcessClientAddBuddyRequest(const Str2StrMap &paras) {
  const string FuncName = "FunIMBuddy::ClientAddBuddyRequest::Handle";
  TimeStatN ts; 

  string buddycore_id, from_uid, to_jid, status, post_script;
  ParasGetValue("presence:id", buddycore_id, paras);  
  ParasGetValue("presence:to", to_jid, paras);
  ParasGetValue("presence.status", status, paras);
  ParasGetValue("presence.postscript", post_script, paras);
  ParasGetValue("actor_uid", from_uid, paras);

  Applyship apply;
  LexicalCast<string, int>(from_uid, apply.applicant);
  apply.accepter = stringToJid(to_jid)->userId;
  // 申请者和被申请者好友数需小于2000，防止刷人气
  try {
    int bc  = 0;
    bc = BuddyByIdCacheAdapter::instance().getFriendCount(apply.applicant);
    if(bc >= 2000) {
      MCE_INFO(FuncName << "--> from user : " << apply.applicant << " has " << bc << " friends so exit");
      return;
    }
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyByIdCache::getFriendCount error: " << e);
  }
  try {
    int bc = 0;
    bc = BuddyByIdCacheAdapter::instance().getFriendCount(apply.accepter);
    if(bc >= 2000) {
      MCE_INFO(FuncName << "--> to user : " << apply.applicant << " has " << bc << " friends so exit");
      return;
    }
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyByIdCache::getFriendCount error : " << e);
  }

  ApplyInfo buddycore_ai;
  // 验证好友申请附言
  if (!post_script.empty()) {
    vector<string> v;
    v.push_back(post_script);
    try {
      CheckResultMore spamobj = OzeinFilterClient::instance().checkerAndFilter(apply.applicant, -1, 108, v, "");
      if (spamobj.getFlag() == 30)
        return;
    } catch (Ice::Exception& e) {
      MCE_WARN(FuncName << "--> OzeinFilterClient.checkerAndFilter: " << e);
      return;
    } catch (std::exception& e) {
      MCE_WARN(FuncName << "--> OzeinFilterClient.checkerAndFilter: " << e.what());
      return;
    }
    (buddycore_ai.props)["WHY"] = post_script;
  }

  MCE_INFO(FuncName << "--> from:" << from_uid << " to:" << to_jid);
  // 验证通过处理申请
  Ice::Context ctx;
  ctx["id"] = buddycore_id;
  ctx["status"] = status;
  try {
    BuddyCoreAdapter::instance().addApply(apply, buddycore_ai, ctx);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyCoreAdapter::addApply excepte:" << e);
  }

  // 构造消息
  ApplyInfo notify_ai;
  TalkUserPtr userinfo;
  try {
    userinfo = TalkCacheClient::instance().GetUserByIdWithLoad(apply.applicant);
  }
  catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkCacheClient::GetUserByIdWithLoad-->" << e);
  }catch(...) {
    MCE_WARN(FuncName << "--> invoke TalkCacheClient.GetUserByIdWithLoad error");
  }
  if(!userinfo) {
    MCE_WARN(FuncName << "--> Get userinfo error");
    return;
  }
  MyUtil::IntSeq common_friends;
  try {
    common_friends = BuddyFriendsCacheAdapter::instance().getCommonFriends(apply.applicant, apply.accepter);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyFriendCache::getCommonFriends error : " << e);
  }
  LexicalCast<int, string>(apply.accepter, (notify_ai.props)["to"]);
  LexicalCast<int, string>(apply.applicant, (notify_ai.props)["source"]);
  LexicalCast<int, string>(apply.accepter, (notify_ai.props)["owner"]);
  LexicalCast<int, string>(apply.applicant, (notify_ai.props)["from"]);
  (notify_ai.props)["type"] = "256";
  (notify_ai.props)["schema_id"] = "256";
  LexicalCast<long, string>((long)time(NULL), (notify_ai.props)["time"]);
  LexicalCast<int, string>(apply.applicant, (notify_ai.props)["id"]);
  (notify_ai.props)["name"] = userinfo->name;
  LexicalCast<size_t, string>(common_friends.size(), (notify_ai.props)["sharecount"]);
  (notify_ai.props)["needCheck"] = "false";
  try {
    (notify_ai.props)["head"] = PhotoUrlHelper::instance().GetFullUrl(userinfo->headurl);
  } catch(...) {
    MCE_DEBUG(FuncName << "-->PhotoUrlHelper.GetFullUrl error");
  }
  (notify_ai.props)["net"] = "";
  (notify_ai.props)["why"] = post_script;

  //构造完毕调用notify所需要的接口
  try {
    NotifyDispatcherAdapter::instance().dispatch(notify_ai.props);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->NoifyDispatcherAdapter::dispath error: " << e);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 150.0));
}

void FunIMBuddy::ProcessClientAddBuddyAccept(const Str2StrMap &paras) {
  const string FuncName = "FunIMBuddy::ClientAddBuddyAccept::Handle";
  TimeStatN ts;

  string buddycore_id, to_jid, from_uid;
  ParasGetValue("presence:id", buddycore_id, paras);
  ParasGetValue("presence:to", to_jid, paras);
  ParasGetValue("actor_uid", from_uid, paras);

  Applyship apply;
  LexicalCast<string, int>(from_uid, apply.applicant);
  apply.accepter = stringToJid(to_jid)->userId;
  // 申请者和被申请者好友数需小于2000，防止刷人气
  try {
    int bc = 0;
    bc = BuddyByIdCacheAdapter::instance().getFriendCount(apply.applicant);
    if(bc >= 2000) {
      MCE_INFO(FuncName << "--> from user : " << apply.applicant << " has " << bc << " friends so exit");
      return;
    }
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyByIdCache::getFriendCount --> error : " << e);
  }
  try {
    int bc = 0;
    bc = BuddyByIdCacheAdapter::instance().getFriendCount(apply.accepter);
    if(bc >= 2000) {
      MCE_INFO(FuncName << "--> to user : " << apply.applicant << " has " << bc << " friends so exit");
      return;
    }
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyByIdCache::getFriendCount error : " << e);
  }

  // 处理请求
  Ice::Context ctx;
  ctx["id"] = buddycore_id;
  MCE_INFO(FuncName << "--> from:" << from_uid << " to:" << to_jid << "  id:" << buddycore_id);
  try {
    BuddyCoreAdapter::instance().addApply(apply, ApplyInfo(), ctx);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyCoreAdapter::addApply error" << e);
  }
  try {
    NotifyGateAdapter::instance().RemoveLogicBySource(apply.applicant, 256, apply.accepter);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->NotifyGateAdapter::RemoveLogicBySource error" << e);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 100.0));
}

void FunIMBuddy::ProcessClientAddBuddyReject(const Str2StrMap &paras) {
  const string FuncName = "FunIMBuddy::ClientAddBuddyReject::Handle";
  MCE_DEBUG(FuncName);
  TimeStatN ts;

  string buddycore_id, to_jid, from_uid;
  ParasGetValue("presence:id", buddycore_id, paras);
  ParasGetValue("presence:to", to_jid, paras);
  ParasGetValue("actor_uid", from_uid, paras);

  Applyship apply;
  apply.applicant = stringToJid(to_jid)->userId;
  LexicalCast<string, int>(from_uid, apply.accepter);

  Ice::Context ctx;
  ctx["id"] = buddycore_id;
  MCE_INFO(FuncName << "--> from:" << from_uid << " to:" << to_jid);
  try {
    BuddyCoreAdapter::instance().denyApply(apply, ctx);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->BuddyCoreAdapter::denyApply error" << e);
  }
  try {
    NotifyGateAdapter::instance().RemoveLogicBySource(apply.accepter, 256, apply.applicant);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->NotifyGateAdapter::RemoveLogicBySource error" << e);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 100.0));
}

void FunIMBuddy::NotifyClientAddBuddyRequest(const string& id, Ice::Int from, Ice::Int to,
  const string& status, const string& postscript) {
  const string FuncName = "FunIMBuddy::NotifyClientAddBuddyRequest";
  MCE_INFO(FuncName << "--> from:" << from << " to:" << to << "  id:" << id);

  TimeStatN ts;

  // 验证好友数，防止刷人气
  int to_friend_count = 0;
  try {
    to_friend_count = BuddyByIdCacheAdapter::instance().getFriendCount(to);
  }  catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "--> BuddyByIdCacheAdapter::getFriendCount --> getid = " << to << " err : " << e);
  }
  if(to_friend_count >= 2000) {
    MCE_INFO(FuncName << "--> id = " << id << " from = " << from << " to = " << to << " status = "
      << status <<"  postscipt:"<< postscript << " to.friends.size = " << to_friend_count);
    return;
  }

  // 向好友添加被申请者发送好友申请消息
  TalkUserPtr from_user;
  try {
    from_user = TalkCacheClient::instance().GetUserByIdWithLoad(from);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkCacheClient::GetUserByIdWithLoad-->"<<e << ", line:"<<__LINE__);
  }
  if(from_user) {
    Ice::Int common_friends_count = 0;
    string number_of_buddies;
    try {
      common_friends_count = BuddyFriendsCacheAdapter::instance().getCommonFriends(from, to).size();
    } catch (Ice::Exception &e) {
       MCE_WARN("BuddyList::load -> get buddies error" << e);
    }
    LexicalCast<int, string>(common_friends_count, number_of_buddies);

    Str2StrMap msg_to;
    msg_to["id"] = id;
    msg_to["nick"] = from_user->name;
    msg_to["EXTVAL"] = buildHeadUrl(from_user->headurl);
    msg_to["status"] = status;
    msg_to["postscript"] = postscript;
    msg_to["numberofbuddies"] = number_of_buddies;
    SendToProto(FuncName, msg_to, PRESENCE_COMMON, "NotifyClientAddBuddyRequest_To",
      createJid(from), to, IM_ONLINE);
  }

  // 向好友申请者发送好友列表更新消息
  TalkUserPtr to_user;
  try {
    to_user = TalkCacheClient::instance().GetUserByIdWithLoad(to);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkCacheClient::GetUserByIdWithLoad-->" << e);
  }
  if(to_user){
    Str2StrMap msg_from;
    LexicalCast<long, string>((long)time(NULL), msg_from["id"]);
    msg_from["jid"] = createJid(to);
    msg_from["name"] = to_user->name;
    SendToProto(FuncName, msg_from, IQ_SET, "NotifyClientAddBuddyRequest_From",
      createJid(from), from, IM_ONLINE);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 150.0));
}

void FunIMBuddy::NotifyClientAddBuddyAccept(const string& id, Ice::Int from, Ice::Int to) {
  const string FuncName = "FunIMBuddy::NotifyClientAddBuddyAccept";
  MCE_INFO(FuncName << "--> from:" << from << " to:" << to << "  id:" << id);
  TimeStatN ts;

  TalkUserPtr from_user;
  TalkUserPtr to_user;
  try {
    from_user = TalkCacheClient::instance().GetUserByIdWithLoad(from);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkCacheClient::GetUserByIdWithLoad-->" << e);
  }
  try {
    to_user = TalkCacheClient::instance().GetUserByIdWithLoad(to);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkCacheClient::GetUserByIdWithLoad-->" << e);
  }
  if(!from_user || !to_user) {
    MCE_INFO(FuncName << "--> error from_user or to_user is null");
    return;
  }

  // 向被申请者发送好友列表更新
  Str2StrMap msg;
  LexicalCast<long, string>((long)time(NULL), msg["id"]);
  msg["name"] = to_user->name;
  msg["jid"] = createJid(to);
  SendToProto(FuncName, msg, IQ_SET, "NotifyClientAddBuddyAccept_From_Iq",
    createJid(from), from, IM_ONLINE);

  // 向申请者发送申请被接受消息
  msg.clear();
  msg["id"] = id;
  msg["nick"] = from_user->name;
  msg["EXTVAL"] = buildHeadUrl(from_user->headurl);
  SendToProto(FuncName, msg, PRESENCE_COMMON, "NotifyClientAddBuddyAccept_To_Presence",
    createJid(from), to, IM_ONLINE);

  // 向申请者发送好友列表更新
  msg.clear();
  LexicalCast<int, string>((int)time(NULL), msg["id"]);
  msg["name"] = from_user->name;
  msg["jid"] = createJid(from);
  SendToProto(FuncName, msg, IQ_SET, "NotifyClientAddBuddyAccept_To_Iq",
    createJid(to), to, IM_ONLINE);

  // 向申请者和被申请者发送在线状态信息
  AvatarSeq to_avatars;
  AvatarSeq from_avatars;
  try{
    to_avatars = TalkFunStateAdapter::instance().getUserAvatars(to, IM_ONLINE);
  } catch(Ice::Exception& e) {
   MCE_WARN(FuncName << "-->TalkFunStateAdapter::getUserAvatars-->" << e);
  }
  try{
    from_avatars = TalkFunStateAdapter::instance().getUserAvatars(from, IM_ONLINE);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkFunStateAdapter::getUserAvatars-->" << e);
  }
  // 是否VIP
  string from_vip, to_vip;
  CheckVip(from_vip, from);
  CheckVip(to_vip, to);

  // 向申请者发送被申请者在线状态
  msg.clear();
  msg["nick"] = from_user->name;
  msg["EXTVAL"] = from_user->headurl;
  msg["status"] = from_user->statusOriginal;
  msg["richstatus"] = xmlEncode(from_user->statusShifted);
  msg["vip"] = from_vip;
  for(size_t i = 0; i < from_avatars.size(); ++i) {
    AvatarPtr avatar = from_avatars.at(i);
    if(!avatar)
      continue;

    msg["ver"] = avatar->ver;
    msg["ext"] = avatar->ext;
    SendToProto(FuncName, msg, PRESENCE_COMMON, "NotifyClientAddBuddyAccept_All_Presence",
      jidToString(avatar->jid), to, IM_ONLINE);
  }

  // 向被申请者发送申请者在线信息
  msg.clear();
  msg["nick"] = to_user->name;
  msg["EXTVAL"] = to_user->headurl;
  msg["status"] = to_user->statusOriginal;
  msg["richstatus"] = xmlEncode(to_user->statusShifted);
  msg["vip"] = to_vip;
  for(size_t i = 0; i < to_avatars.size(); ++i) {
    AvatarPtr avatar = to_avatars.at(i);
    if(!avatar)
      continue;

    msg["ver"] = avatar->ver;
    msg["ext"] = avatar->ext;
    SendToProto(FuncName, msg, PRESENCE_COMMON, "NotifyClientAddBuddyAccept_All_Presence",
      jidToString(avatar->jid), to, IM_ONLINE);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 200.0));
}

void FunIMBuddy::NotifyClientAddBuddyReject(const string& id, Ice::Int from, Ice::Int to) {
  const string FuncName = "FunIMBuddy::NotifyClientAddBuddyReject";
  MCE_INFO(FuncName << "--> from:" << from << " to:" << to << "  id:" << id);
  TimeStatN ts;

  // 向申请者发送拒绝消息
  Str2StrMap msg;
  SendToProto(FuncName, msg, PRESENCE_COMMON, "NotifyClientAddBuddyReject_To_Presence",
    createJid(from), to, IM_ONLINE);

  msg.clear();
  LexicalCast<int, string>(int(time(NULL)), msg["id"]);
  msg["jid"] = createJid(from);
  SendToProto(FuncName, msg, IQ_SET, "NotifyClientAddBuddyReject_To_Iq", createJid(from), to, IM_ONLINE);

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 100.0));
}

