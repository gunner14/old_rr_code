#include "FunIMCommon.h"
#include "XmppTools.h"
#include "TalkProtoAdapter.h"
#include "UserCacheAdapter.h"
#include "FeedMemcProxy/client/talk_cache_client.h"
#include "ReceiversHandler.h"

using namespace com::xiaonei::talk::function::im;
using namespace com::xiaonei::talk::proto;
using namespace com::xiaonei::talk::cache;
using namespace xce::usercache;
using namespace com::xiaonei::talk::util;

void com::xiaonei::talk::function::im::SendToProto(const string& func_name, Str2StrMap& msg, const int& message_type,
  const string& item_type, const string& from_jid, const int& to_id, const int& online_type) {
  // 发送to uid到Proto，Proto取Jid
  LexicalCast<int, string>(message_type, msg["message_type"]);
  msg["config_type"] = TALKFUNIM_TPL_TYPE;
  msg["item_type"] = item_type;
  if(!from_jid.empty())
    msg["from_jid"] = from_jid;
  LexicalCast<int, string>(to_id, msg["to_id"]);
  LexicalCast<int, string>(online_type, msg["online_type"]);
  try {
    TalkProtoAdapter::instance().Receive(msg);
  } catch(Ice::Exception& e) {
    MCE_WARN(func_name << "-->TalkProtoAdapter::Receive:  from_jid:" << from_jid << 
      "to_id" << to_id << " error:" << e);
  }
}

void com::xiaonei::talk::function::im::SendToProtoExpress(const string& func_name, Str2StrMap& msg, 
  const int& message_type, const string& item_type, const string& from_jid, string& to_jid) {
  // 发送to jid到Proto，Proto 不取在线状态
  LexicalCast<int, string>(message_type, msg["message_type"]);
  msg["config_type"] = TALKFUNIM_TPL_TYPE;
  msg["item_type"] = item_type;
  if(!from_jid.empty())
    msg["from_jid"] = from_jid;
  msg["to_jid"] = to_jid;
  try {
    TalkProtoAdapter::instance().ReceiveExpress(msg);
  } catch(Ice::Exception& e) {
    MCE_WARN(func_name << "-->TalkProtoAdapter::Receive:  from_jid:" << from_jid << 
      "to_jid" << to_jid << " error:" << e);
  }
}

void com::xiaonei::talk::function::im::SendToProtoExpressWithJids(const string& func_name, Str2StrMap& msg, 
  const int& message_type, const string& item_type, const string& from_jid, vector<string>& to_jid) {
  // 发送批量to jid到Proto，Proto 不取在线状态
  LexicalCast<int, string>(message_type, msg["message_type"]);
  msg["config_type"] = TALKFUNIM_TPL_TYPE;
  msg["item_type"] = item_type;
  if(!from_jid.empty())
    msg["from_jid"] = from_jid;
  msg["to_jid"] = ReceiversHandler::ReceiversAssembly(to_jid);
  try {
    TalkProtoAdapter::instance().ReceiveExpress(msg);
  } catch(Ice::Exception& e) {
    MCE_WARN(func_name << "-->TalkProtoAdapter::Receive:  from_jid:" << from_jid << " error:" << e);
  }
}

void com::xiaonei::talk::function::im::SendToProtoWithIds(const string& func_name, Str2StrMap& msg, 
  const int& message_type, const string& item_type, const string& from_jid, vector<int>& to_id, 
  const int& online_type) {
  // 发送批量to uid到Proto，Proto获取Jid
  LexicalCast<int, string>(message_type, msg["message_type"]);
  msg["config_type"] = TALKFUNIM_TPL_TYPE;
  msg["item_type"] = item_type;
  msg["from_jid"] = from_jid;
  msg["to_id"] = ReceiversHandler::ReceiversAssembly(to_id);
  LexicalCast<int, string>(online_type, msg["online_type"]);
  try {
    TalkProtoAdapter::instance().Receive(msg);
  } catch(Ice::Exception& e) {
    MCE_WARN(func_name << "-->TalkProtoAdapter::Receive:  from_jid:" << from_jid << 
      "to_id" << msg["to_id"] << " error:" << e);
  }
}

void com::xiaonei::talk::function::im::CheckVip(string& vip_string, const int& uid) {
  IntSeq idSeq;
  idSeq.push_back(uid);
  map<int, UserCachePtr> userVips;
  try{
    userVips = UserCacheAdapter::instance().GetUserCache(idSeq);
  }catch(Ice::Exception& e){
    MCE_WARN("FunIMBuddy::CheckVip-->UserCacheAdapter::GetUserCache-->userId:"<< uid <<"  Ice::Exception:" << e);
    return;
  }

  UserCachePtr vipInfo;
  int vip = 0;
  map<int, UserCachePtr>::iterator it = userVips.find(uid);
  if(it != userVips.end() && it->second){
    vipInfo = it->second;
    vip = vipInfo->level();
    if(vip == 0){
      vip = 1;
    }
    if(vipInfo->IsVipMember()){
    }else if(vipInfo->IsEverVipMember()){
      vip = -vip;
    }else{
      vip = 0;
    }
  }
  LexicalCast<int, string>(vip, vip_string);
}

void com::xiaonei::talk::function::im::CheckVipWithoutGet(string& vip_string, const int& uid, 
  map<int, UserCachePtr>& userVips) {
  int vip = 0;
  map<int, UserCachePtr>::iterator it = userVips.find(uid);
  if(it != userVips.end() && it->second){
    UserCachePtr vipInfo = it->second;
    vip = vipInfo->level();
    if(vip == 0){
      vip = 1;
    }
    if(vipInfo->IsVipMember()){
    }else if(vipInfo->IsEverVipMember()){
      vip = -vip;
    }else{
      vip = 0;
    }
  }
  LexicalCast<int, string>(vip, vip_string);
}

