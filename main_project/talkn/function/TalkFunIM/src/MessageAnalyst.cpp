#include "MessageAnalyst.h"
#include "TaskManager.h"
#include "XmppTools.h"
#include "IMTaskManagerLevel.h"
#include "FunIMCommon.h"
#include "FunIMBuddy.h"
#include "FunIMOther.h"
#include "FunIMState.h"
#include "BusinessMeet.h"

using namespace com::xiaonei::talk::function::im;
using namespace MyUtil;
using namespace com::xiaonei::talk::util;

void MessageAnalyst::InitAnalystTable() {
  analyst_table_.push_back(AnalystTableItem("iq.vCard:xmlns", "vcard-temp", GET_VCARD_REQUEST));
  analyst_table_.push_back(AnalystTableItem("iq.ping:xmlns", "urn:xmpp:ping", PING_REQUEST));
  analyst_table_.push_back(AnalystTableItem("presence:type", "subscribe", CLIENT_ADD_BUDDY_REQUEST));
  analyst_table_.push_back(AnalystTableItem("presence:type", "subscribed", CLIENT_ADD_BUDDY_ACCEPT));
  analyst_table_.push_back(AnalystTableItem("presence:type", "unsubscribed", CLIENT_ADD_BUDDY_REJECT));
  /*
  analyst_table_.push_back(AnalystTableItem("iq.query:xmlns", "jabber:iq:occasional:roster", GET_MEET_PAY_LIST));
  analyst_table_.push_back(AnalystTableItem("message:type", "occasional:chat", MEET_MESSAGE));
  analyst_table_.push_back(AnalystTableItem("message:type", "occasional:chatext", MEET_MESSAGE2));
  analyst_table_.push_back(AnalystTableItem("iq.query:xmlns", "occasional:info:user", GET_MEET_INFO));
  analyst_table_.push_back(AnalystTableItem("iq.query:xmlns", "occasional:info:self", GET_MEET_SELFINFO));

  analyst_table_.push_back(AnalystTableItem("presence.x:xmlns", "http://conference.talk.renren.com/offline", DESK_OFFLINE));
  */
  // 总是保持在最后
  //analyst_table_.push_back(AnalystTableItem("presence.priority", "*", CLIENT_USER_STATE_CHANGED));
}

Jobs MessageAnalyst::GetJobType(const Str2StrMap &paras) {
  // 查询analyst table
  vector<AnalystTableItem>::const_iterator i;
  Str2StrMap::const_iterator j;
  for(i = analyst_table_.begin(); i != analyst_table_.end(); ++i) {
    j = paras.find(i->key_);
    if(j != paras.end()) 
      if(j->second == i->value_ || i->value_ == "*") {
        MCE_INFO("MessageAnalyst::Analyse : " << i->key_);
        return i->type_;
      }
  }

  return UNKNOW;
}

void MessageAnalyst::Analyse(const Str2StrMap &paras) {
  switch(GetJobType(paras)) {
    case GET_VCARD_REQUEST:
      buddy::FunIMBuddy::instance().ProcessGetVcardRequest(paras);
      break;
    case PING_REQUEST:
      other::FunIMOther::instance().ProcessPingRequest(paras);
      break;
    case CLIENT_ADD_BUDDY_REQUEST:
      // 申请添加好友
      buddy::FunIMBuddy::instance().ProcessClientAddBuddyRequest(paras); 
      break;
    case CLIENT_ADD_BUDDY_ACCEPT:
      // 同意添加好友
      buddy::FunIMBuddy::instance().ProcessClientAddBuddyAccept(paras); 
      break;
    case CLIENT_ADD_BUDDY_REJECT:
      // 拒绝添加好友
      buddy::FunIMBuddy::instance().ProcessClientAddBuddyReject(paras); 
      break;

    case GET_MEET_PAY_LIST:
      // 遇见业务请求
      business::BusinessMeet::instance().business(TaskLevel::TASK_MEET, paras, business::MeetTask::GET_PAY_LIST);
      break;

    case MEET_MESSAGE:
      // 遇见业务聊天
      business::BusinessMeet::instance().business(TaskLevel::TASK_MEET, paras, business::MeetTask::MEET_MESSAGE);
      break;

    case MEET_MESSAGE2:
      // 遇见业务聊天
      business::BusinessMeet::instance().business(TaskLevel::TASK_MEET, paras, business::MeetTask::MEET_MESSAGE2);
      break;

    case GET_MEET_INFO:
      // 遇见业务聊天
      business::BusinessMeet::instance().business(TaskLevel::TASK_MEET, paras, business::MeetTask::GET_OTHER_INFO);
      break;

    case GET_MEET_SELFINFO:
      // 遇见业务聊天
      business::BusinessMeet::instance().business(TaskLevel::TASK_MEET, paras, business::MeetTask::GET_SELF_INFO);
      break;


    case DESK_OFFLINE:
      // 遇见业务聊天
      business::BusinessMeet::instance().business(TaskLevel::TASK_MEET, paras, business::MeetTask::OFFLINE);
      break;

    case CLIENT_USER_STATE_CHANGED:
      state::FunIMState::instance().ProcessClientUserStateChanged(paras);

      break;
    default:
      MCE_WARN("TalkFunIM --> unknown IM Message");
  }
}
