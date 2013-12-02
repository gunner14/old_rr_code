#include <boost/format.hpp>
#include "IceLogger.h"
#include "MessageType.h"
#include "XmppTools.h"

#include "FunIMCommon.h"

#include "BusinessMeet.h"
#include "BadooServiceAdapter.h"
#include "TalkFunStateAdapter.h"



using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::function::im::business;
using namespace com::renren::xoa2::sns::badoo;
using namespace com::xiaonei::talk::fun::state;




void BusinessMeet::business(int level, const Str2StrMap &paras, int type) {
  const char *fun = "BusinessMeet::business";
  int tsz = TaskManager::instance().size(level);
  int rsz = TaskManager::instance().runningThreadSize(level);
  //int wsz = TaskManager::instance().waitThreadSize(level);
  //int csz = TaskManager::instance().completedTaskSize(level);
  MCE_INFO(fun << "-->level:" << level
           << " tasksize:" << tsz
           << " runsize:" << rsz
           //<< " waitsize:" << wsz
           //<< " complitesize:" << csz
           );
  if (tsz < TASK_MAX_LEN_DEFAULT) {
    TaskManager::instance().execute(new MeetTask(level, paras, type));
  } else {
    MCE_WARN(fun << "-->overflow level:" << level
             << " tasksize:" << tsz
             << " runsize:" << rsz
             );
  }

}

// Task ===================================



void (MeetTask::*const MeetTask::queses_[TASK_COUNT])() = {
  &MeetTask::getPaylist,
  &MeetTask::message,
  &MeetTask::message2,
  &MeetTask::presence,
  &MeetTask::offline,
  &MeetTask::getSelfInfo,
  &MeetTask::getOtherInfo,

};
void MeetTask::handle() {

  TimeStatN ts;
  char fun[50];
  snprintf(fun, sizeof(fun), "MeetTask::handle%d", ques_);

  string from_uid;
  ParasGetValue("actor_uid", from_uid, paras_);
  try {
    rid_ = boost::lexical_cast<int>(from_uid);
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " from_uid:" << from_uid);
    return;
  }


  if (ques_ < 0 || ques_ > TASK_COUNT) {
    MCE_WARN(fun << "-->unknown ques:" << ques_);
  } else {
    (this->*queses_[ques_])();
  }

  FunStatManager::instance().Stat(fun, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
}

void MeetTask::message2() {
  const char *fun = "MeetTask::message2";
  const Str2StrMap &paras = paras_;
  if (IS_DEBUG_OUT) {
    for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
      MCE_DEBUG(fun << "-->key:" << it->first << " value:" << it->second);
    }
  }

  string from_uid, to, usrdata;
  ParasGetValue("message:to", to, paras);
  ParasGetValue("actor_uid", from_uid, paras);
  ParasGetValue("message.usrdata", usrdata, paras);

  size_t pos = to.find("@");
  if (pos == string::npos) {
    MCE_WARN(fun << "-->error to:" << to);
    return;
  }
  to = to.substr(0, pos);
  // 关键词过滤
  //if (!SpamFilterVerify(rid_, body)) {
  //  return;
  //}

  
  int to_meet_uid = 0;

  int to_rr_uid = 0;
  int from_meet_uid = 0;

  try {
    to_meet_uid = boost::lexical_cast<int>(to);
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " to:" << to);
    return;
  }

  // ========================================
  if (!getBadooIdByRId(fun, rid_, from_meet_uid) || from_meet_uid == 0) {
    return;
  }

  if (!getRIdByBId(fun, to_meet_uid, to_rr_uid) || to_rr_uid == 0) {
    return;
  }


  // 调用遇见业务保存消息
  //sendGossip(fun, from_meet_uid, to_meet_uid, body);


  // 拼map
  Str2StrMap msg;
  try {
    msg["from_meet_id"] = boost::lexical_cast<string>(from_meet_uid);
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " from_meet_uid:" << from_meet_uid);
  }

  msg["usrdata"] = usrdata;


  for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
    if (it->first.size() >= strlen("message.") && it->first.substr(0, strlen("message.")) == "message.") {
      msg[it->first] = it->second;
    }
  }

  if (IS_DEBUG_OUT) {
    for (Str2StrMap::const_iterator it = msg.begin(); it != msg.end(); ++it) {
      MCE_DEBUG(fun << "-->out key:" << it->first << " value:" << it->second);
    }
  }


  // 只给桌面的发 4
  SendToProto(fun, msg, OTHER, "MeetMessage2", "", to_rr_uid, 4);


}

void MeetTask::message() {
  const char *fun = "MeetTask::message";
  const Str2StrMap &paras = paras_;
  if (IS_DEBUG_OUT) {
    for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
      MCE_DEBUG(fun << "-->key:" << it->first << " value:" << it->second);
    }
  }


  string from_uid, to, body, usrdata;
  ParasGetValue("message:to", to, paras);
  ParasGetValue("actor_uid", from_uid, paras);
  ParasGetValue("message.body", body, paras);
  //ParasGetValue("message.usrdata", usrdata, paras);

  //<format xmlns="xn:format" ink="3355443" height="10" bold="0" italic="0" face="SimSun"/>

  /*
  if (paras.find("message.format") != paras.end()) {
    string fmt_xmlns, fmt_ink, fmt_height, fmt_bold, fmt_italic, fmt_face;
    ParasGetValue("message.format:", , paras);
    ParasGetValue("message.format:", , paras);
    ParasGetValue("message.format:", , paras);
    ParasGetValue("message.format:", , paras);
    ParasGetValue("message.format:", , paras);
    ParasGetValue("message.format:", , paras);
    msg["fmt_have"] = "yes";
    msg["fmt_xmlns"] = fmt_xmlns;
    msg["fmt_ink"] = fmt_ink;
    msg["fmt_height"] = fmt_height;
    msg["fmt_bold"] = fmt_bold;
    msg["fmt_italic"] = fmt_italic;
    msg["fmt_face"] = fmt_face;

  }
  */

  size_t pos = to.find("@");
  if (pos == string::npos) {
    MCE_WARN(fun << "-->error to:" << to);
    return;
  }
  to = to.substr(0, pos);
  // 关键词过滤
  if (!SpamFilterVerify(rid_, body)) {
    return;
  }

  
  int to_meet_uid = 0;

  int to_rr_uid = 0;
  int from_meet_uid = 0;

  try {
    to_meet_uid = boost::lexical_cast<int>(to);
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " to:" << to);
    return;
  }

  // ========================================
  if (!getBadooIdByRId(fun, rid_, from_meet_uid) || from_meet_uid == 0) {
    return;
  }

  if (!getRIdByBId(fun, to_meet_uid, to_rr_uid) || to_rr_uid == 0) {
    return;
  }


  // 调用遇见业务保存消息
  sendGossip(fun, from_meet_uid, to_meet_uid, body);


  // 拼map
  Str2StrMap msg;
  try {
    msg["from_meet_id"] = boost::lexical_cast<string>(from_meet_uid);
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " from_meet_uid:" << from_meet_uid);
  }

  msg["body"] = body;
  if (!usrdata.empty())
      msg["usrdata"] = usrdata;


  for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
    if (it->first.size() >= strlen("message.") && it->first.substr(0, strlen("message.")) == "message.") {
      msg[it->first] = it->second;
    }
  }

  if (IS_DEBUG_OUT) {
    for (Str2StrMap::const_iterator it = msg.begin(); it != msg.end(); ++it) {
      MCE_DEBUG(fun << "-->out key:" << it->first << " value:" << it->second);
    }
  }


  // 只给桌面的发 4
  SendToProto(fun, msg, OTHER, "MeetMessage", "", to_rr_uid, 4);
}






void MeetTask::getPaylist() {
  const char *fun = "MeetTask::getPaylist";
  const Str2StrMap &paras = paras_;
  if (IS_DEBUG_OUT) {
    for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
      MCE_DEBUG(fun << "-->key:" << it->first << " value:" << it->second);
    }
  }



  string from_jid, from_uid, id;
  ParasGetValue("actor_jid", from_jid, paras);



  int f_bid = 0;
  BadooLikeInfoList llist, bllist, mlist;
  BadooLikeUserInfo actor_info;
  vector<int> uids; // 用户相关所有信息列表(关注+被关注的,去重)
  // -------------------------------------
  if (!getBadooIdByRId(fun, rid_, f_bid) || f_bid <= 0) {
    // rpc 调用出错 || 该用户不是遇见用户
    return;
  }

  getLikeUser(fun, llist, rid_, BADOOS_LIMIT);
  getBeLikeUser(fun, bllist, rid_, BADOOS_LIMIT);
  getMutileLikeUser(fun, mlist, rid_, BADOOS_LIMIT);
  if (IS_DEBUG_OUT) {
    MCE_DEBUG(fun << "-->like:" << llist.size() << " belike:" << bllist.size() << " mulike:" << mlist.size());
  }
  // -------------------------------------

  notifyBadooList(from_jid, llist, bllist, mlist);
  notifyPresence(from_jid, llist, bllist, mlist);
}


void MeetTask::presence() {
  // presence功能放到了获取好友列表里面
}


void MeetTask::offline() {
  const char *fun = "MeetTask::offline";
  const Str2StrMap &paras = paras_;
  if (IS_DEBUG_OUT) {
    for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
      MCE_DEBUG(fun << "-->key:" << it->first << " value:" << it->second);
    }
  }
  int f_bid = 0;
  if (!getBadooIdByRId(fun, rid_, f_bid) || f_bid <= 0) {
    // rpc 调用出错 || 该用户不是遇见用户
    return;
  }

  string f_jid, f_endpoint, f_index;
  ParasGetValue("actor_jid", f_jid, paras);
  ParasGetValue("actor_jid_index", f_index, paras);
  ParasGetValue("actor_jid_endpoint", f_endpoint, paras);


  vector<int> rIds;
  getNotifyList(rid_, rIds);

  Str2StrMap msg;
  msg["actor_jid_endpoint"] = f_endpoint;
  msg["actor_jid_index"] = f_index;
  try {
    msg["from_bid"] = boost::lexical_cast<string>(f_bid);

  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " f_bid:" << f_bid << " f_jid:" << f_jid);
  }

  SendToProtoWithIds(fun, msg, OTHER, "MeetOffline", f_jid, rIds, 4);
}

void MeetTask::getSelfInfo() {
  const char *fun = "MeetTask::getSelfInfo";
  const Str2StrMap &paras = paras_;
  if (IS_DEBUG_OUT) {
    for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
      MCE_DEBUG(fun << "-->key:" << it->first << " value:" << it->second);
    }
  }


  string from_jid, id;
  ParasGetValue("actor_jid", from_jid, paras);
  ParasGetValue("iq:id", id, paras);


  Str2StrMap msg;
  msg["id"] = id;

  BadooUserInfo actor_info;
  if (getUserBaseInfoByRId2(fun, actor_info, rid_)) {
    getBaseInfo(actor_info, "info", 0, msg);
    SendToProtoExpress(fun, msg, IQ_RESULT, "GetMeetSelfInfo", from_jid, from_jid);
  } else {
    SendToProtoExpress(fun, msg, IQ_RESULT, "GetMeetSelfInfoError", from_jid, from_jid);
  }


}

void MeetTask::getOtherInfo() {
  const char *fun = "MeetTask::getOtherInfo";
  const Str2StrMap &paras = paras_;

  IdWrappers bIds;
  vector<string> sbIds;
  for (Str2StrMap::const_iterator it = paras.begin(); it != paras.end(); ++it) {
    if (IS_DEBUG_OUT) {
      MCE_DEBUG(fun << "-->key:" << it->first << " value:" << it->second);
    }
    const string &key = it->first;
    const string &value = it->second;
    size_t pos = key.find("iq.query.item@");
    if (pos != string::npos) {
      if (key.find(":to", pos)!= string::npos) {
        pos = value.find("@");
        if (pos != string::npos) {
          sbIds.push_back(value.substr(0, pos));
        } else {
          MCE_WARN(fun << "-->error to:" << value);
        }

      }
    } else if (key == "iq.query.item:to") {
        pos = value.find("@");
        if (pos != string::npos) {
          sbIds.push_back(value.substr(0, pos));
        } else {
          MCE_WARN(fun << "-->error to:" << value);
        }

    }
  }
  MCE_INFO(fun << "-->sbIds.size:" << sbIds.size());
  if (IS_DEBUG_OUT) {
    for (vector<string>::const_iterator it = sbIds.begin(); it != sbIds.end(); ++it) {
      MCE_DEBUG(fun << "-->bid:" << *it);
    }
  }

  
  BadooUserInfoList binfos;
  if (getUserBaseInfoByBId(fun, binfos, sbIds)) {

    Str2StrMap msg;
    string from_jid, id;
    ParasGetValue("actor_jid", from_jid, paras);
    ParasGetValue("iq:id", id, paras);
    msg["id"] = id;

    int count = 0;
    for (BadooUserInfoList::const_iterator it = binfos.begin(); it != binfos.end(); ++it, ++count) {
      getBaseInfo(*it, "info", count, msg);
    }
    SendToProtoExpress(fun, msg, OTHER, "GetMeetOtherInfo", from_jid, from_jid);

  }

}

