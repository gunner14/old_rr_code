#include <boost/format.hpp>
#include "IceLogger.h"
#include "MessageType.h"
#include "XmppTools.h"

#include "FunIMCommon.h"

#include "BusinessMeet.h"
#include "BadooServiceAdapter.h"
#include "TalkFunStateAdapter.h"
#include "talk/SpamClient/src/OzeinFilterAdapter.h"


using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::function::im::business;
using namespace com::renren::xoa2::sns::badoo;
using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::ozein::content::generation;

void MeetTask::getNotifyList(int rId, vector<int> &rIds) {
  const char *fun = "MeetTask::getNotifyList";

  rIds.clear();
  set<int> srIds;
  BadooLikeInfoList tmp;
  if (getLikeUser(fun, tmp, rId, BADOOS_LIMIT)) {
    for (BadooLikeInfoList::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
      srIds.insert(it->userId);
    }
  }

  tmp.clear();
  if (getBeLikeUser(fun, tmp, rId, BADOOS_LIMIT)) {
    for (BadooLikeInfoList::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
      srIds.insert(it->userId);
    }
  }

  for (set<int>::const_iterator it = srIds.begin(); it != srIds.end(); ++it) {
    rIds.push_back(*it);
  }

}

bool MeetTask::getBadooIdByRId(const char *fun, const string &rId, int32_t &bid) {
  int uid = 0;
  bool chk = false;

  try {
    uid = boost::lexical_cast<int>(rId);
    chk = getBadooIdByRId(fun, uid, bid);
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " rId:" << rId);
  }

  return chk;

}

bool MeetTask::getRIdByBId(const char *fun, const string &bId, int32_t &rid) {
  int uid = 0;
  bool chk = false;

  try {
    uid = boost::lexical_cast<int>(bId);
    chk = getRIdByBId(fun, uid, rid);
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " bId:" << bId);
  }

  return chk;

}


bool MeetTask::SpamFilterVerify(int from, const string &msg) {
  const char *fun = "MeetTask::SpamFilterVerify";
  const char *call = "OzeinFilterClient.checkerAndFilter";
  TimeStatN ts_spam; 
  vector<string> v;
  bool rv = true;
  v.push_back(msg);
  try {
    CheckResultMore spamobj = OzeinFilterClient::instance().checkerAndFilter(from, -1, 108, v, ""); 
    if (spamobj.getFlag() == 30) {
      rv = false;
      MCE_INFO(fun << "-->spam from " << from << " msg " << msg);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN(fun << "-->"
             "call " << call << " Ice err:" <<e);
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->"
             "call " << call << " err:" <<e.what());
  }

  FunStatManager::instance().Stat(call, ts_spam.getTime(), (ts_spam.getTime() >= RUN_TIME_OUT));

  return rv;
}


bool MeetTask::getBadooIdByRId(const char *fun, const int32_t rId, int32_t &bid) {
  TimeStatN ts;
  const char *call = "BadooAdapter.getBadooIdByRId";
  bool chk = false;
  try {
    bid = BadooAdapter::instance().getBadooIdByRId(rId);
    chk = true;

  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " rId:" << rId);
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " rId:" << rId);

  }

  if (bid == 0) {
    MCE_WARN(fun << "-->call:" << call << " error rid:" << rId);
  }

  if (IS_DEBUG_OUT) {
    MCE_INFO(fun << "-->call:" << call << " rId:" << rId << " bid:" << bid);
  }


  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;
}

bool MeetTask::getRIdByBId(const char *fun, const int32_t bId, int32_t &rid) {
  TimeStatN ts;
  const char *call = "BadooAdapter.getRIdByBId";
  bool chk = false;
  try {
    rid = BadooAdapter::instance().getRIdByBId(bId);
    chk = true;
  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " bId:" << bId);
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " bId:" << bId);

  }
  if (rid == 0) {
    MCE_WARN(fun << "-->error bid:" << bId);
  }

  if (IS_DEBUG_OUT) {
    MCE_INFO(fun << "-->call:" << call << " bId:" << bId << " rid:" << rid);
  }

  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;

}

bool MeetTask::sendGossip(const char *fun, const int32_t senderId, const int32_t receiverId, const std::string& message) {
  TimeStatN ts;
  const char *call = "BadooAdapter.sendGossip";
  bool chk = false;

  int fail;
  try {
    fail = BadooAdapter::instance().sendGossip(senderId, receiverId, message);
    if (!fail) {
      chk = true;
    } else {
      MCE_WARN(fun << "-->call:" << call << " return fail" 
               << " senderId:" << senderId
               << " receiverId:" << receiverId
               << " msg:" << message
               );

    }

  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " senderId:" << senderId
             << " receiverId:" << receiverId
             << " msg:" << message
             );
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " senderId:" << senderId
             << " receiverId:" << receiverId
             << " msg:" << message
             );

  }
  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;

}


bool MeetTask::getLikeUser(const char *fun, BadooLikeInfoList& rv, const int32_t rId, const int32_t limit) {
  TimeStatN ts;
  const char *call = "BadooAdapter.getLikeUser";
  bool chk = false;
  try {
    BadooAdapter::instance().getLikeUser(rv, rId, limit);
    chk = true;
  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " rId:" << rId);
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " rId:" << rId);

  }
  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;

}

bool MeetTask::getBeLikeUser(const char *fun, BadooLikeInfoList& rv, const int32_t rId, const int32_t limit) {
  TimeStatN ts;
  const char *call = "BadooAdapter.getBeLikeUser";
  bool chk = false;
  try {
    BadooAdapter::instance().getBeLikeUser(rv, rId, limit);
    chk = true;
  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " rId:" << rId);
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " rId:" << rId);

  }
  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;
}

bool MeetTask::getMutileLikeUser(const char *fun, BadooLikeInfoList& rv, const int32_t rId, const int32_t limit) {
  TimeStatN ts;
  const char *call = "BadooAdapter.getMutileLikeUser";
  bool chk = false;
  try {
    BadooAdapter::instance().getMutileLikeUser(rv, rId, limit);
    chk = true;
  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " rId:" << rId);
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " rId:" << rId);

  }
  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;
}


bool MeetTask::getUserBaseInfoByRId(const char *fun, BadooUserInfoList& rv, const IdWrappers& rIds) {
  TimeStatN ts;
  const char *call = "MeetTask::getUserBaseInfoByRId";
  bool chk = false;
  try {
    BadooAdapter::instance().getUserBaseInfoByRId(rv, rIds);
    if (IS_DEBUG_OUT) {
      MCE_DEBUG(fun << "-->" << call << " rv.size:" << rv.size());
      for (BadooUserInfoList::const_iterator it = rv.begin(); it != rv.end(); ++it) {
        MCE_DEBUG(fun << "-->" << call <<
                  " rv.rId:" << it->userId << " rv.bId:" << it->badooUserId
                  << " rv.name:" << it->name);
      }
    }
    chk = true;
  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " rIds:" << rIds.size());
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " rIds:" << rIds.size());

  }
  if (rv.size() != rIds.size()) {
    MCE_WARN(fun << "-->call:" << call << " uneq rv.size" << rv.size() << " rids.size:" << rIds.size());
  }
  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;

}

bool MeetTask::getUserBaseInfoByBId(const char *fun, BadooUserInfoList& rv, const vector<string>& sbIds) {
  int bId = 0;
  IdWrappers bIds;
  for (vector<string>::const_iterator it = sbIds.begin(); it != sbIds.end(); ++it) {
    try {
      bId = boost::lexical_cast<int>(*it);
      IdWrapper wbId;
      wbId.id = bId;
      bIds.push_back(wbId);
    } catch(std::exception& e) {
      MCE_WARN(fun << "-->lexical error:" << e.what() << " bId:" << *it);
    }

  }

  return getUserBaseInfoByBId(fun, rv, bIds);
}

bool MeetTask::getUserBaseInfoByBId(const char *fun, BadooUserInfoList& rv, const IdWrappers& bIds) {
  TimeStatN ts;
  const char *call = "MeetTask::getUserBaseInfoByBId";
  bool chk = false;
  try {
    BadooAdapter::instance().getUserBaseInfoByBId(rv, bIds);
    chk = true;
  } catch (TTransportException& e) {
    MCE_WARN(fun << "-->call:" << call << " TTransportException:" << e.what()
             << " bIds:" << bIds.size());
  } catch (std::exception& e) {
    MCE_WARN(fun << "-->call:" << call << " error:" << e.what()
             << " bIds:" << bIds.size());

  }

  MCE_INFO(fun << "-->call:" << call << " uneq rv.size" << rv.size() << " bids.size:" << bIds.size());

  if (rv.size() != bIds.size()) {
    MCE_WARN(fun << "-->call:" << call << " uneq rv.size" << rv.size() << " bids.size:" << bIds.size());
  }



  FunStatManager::instance().Stat(call, ts.getTime(), (ts.getTime() > RUN_TIME_OUT));
  return chk;

}

bool MeetTask::getUserBaseInfoByRId2(const char *fun, BadooUserInfo& rv, const int rId) {
  bool chk = false;
  IdWrappers rIds;
  IdWrapper tmp;
  tmp.id = rId;
  rIds.push_back(tmp);
  BadooUserInfoList rvs;
  chk = getUserBaseInfoByRId(fun, rvs, rIds);
  if (chk && rvs.size() == 1) {
    rv = rvs[0];

  } else {
    chk = false;
  }

  return chk;

}


void MeetTask::getBaseInfo(const BadooUserInfo &info, const char *pref, int idx, Str2StrMap &msg) {
  const char *fun = "MeetTask::getBaseInfo";
  string k_pfx;
  string key;
  k_pfx = str(boost::format("%s.%d.") % pref % idx);


  key = str(boost::format("%s%s") % k_pfx % "name");
  msg[key] = info.name;
  key = str(boost::format("%s%s") % k_pfx % "url");
  msg[key] = info.tinyUrl;

  key = str(boost::format("%s%s") % k_pfx % "mood");
  msg[key] = info.mood;

  key = str(boost::format("%s%s") % k_pfx % "province");
  msg[key] = info.provinceName;
  key = str(boost::format("%s%s") % k_pfx % "city");
  msg[key] = info.cityName;


  try {
    key = str(boost::format("%s%s") % k_pfx % "bid");
    msg[key] = boost::lexical_cast<string>(info.badooUserId);;

    key = str(boost::format("%s%s") % k_pfx % "charm");
    msg[key] = boost::lexical_cast<string>(info.charm);

    key = str(boost::format("%s%s") % k_pfx % "sex");
    msg[key] = boost::lexical_cast<string>(info.sex);

    key = str(boost::format("%s%s") % k_pfx % "age");
    msg[key] = boost::lexical_cast<string>(info.age);

  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what()
             << " bid:" << info.badooUserId
             << " charm:" << info.charm
             << " sex:" << info.sex
             << " age:" << info.age
             );
  }


}

void MeetTask::getListInfo(const BadooLikeUserInfo &info, const char *pref, int idx, Str2StrMap &msg) {
  const char *fun = "MeetTask::getListInfo";
  string k_pfx;
  string key;
  k_pfx = str(boost::format("%s.%d.") % pref % idx);

  key = str(boost::format("%s%s") % k_pfx % "bid");
  try {
    msg[key] = boost::lexical_cast<string>(info.badooUserId);;
  } catch(std::exception& e) {
    MCE_WARN(fun << "-->lexical error:" << e.what() << " bid:" << info.badooUserId);
  }


  key = str(boost::format("%s%s") % k_pfx % "name");
  msg[key] = info.name;
}

void MeetTask::notifyBadooList(string &from_jid,
                              const BadooLikeInfoList &pay_list,
                              const BadooLikeInfoList &payed_list,
                              const BadooLikeInfoList &both_list
                              ) {
  const char *fun = "MeetTask::notifyBadooList";

  // 要求去重，一个用户不能在多个组里面…………崩溃的需求，早晚得鸡巴改,just fuck it
  set <int> bidset;
  for (BadooLikeInfoList::const_iterator it = both_list.begin(); it != both_list.end(); ++it) {
    bidset.insert(it->userId);
  }

  Str2StrMap msg;
  string id;
  ParasGetValue("iq:id", id, paras_);
  msg["id"] = id;

  string k_pfx;
  string key;
  int count = 0;
  const int CN_GROUP = 3;
  const char *groups[CN_GROUP] = {"pay", "payed", "both"};
  const BadooLikeInfoList *all_list[CN_GROUP] = {&pay_list, &payed_list, &both_list};

  for (int i = 0; i < CN_GROUP; ++i) {
    const BadooLikeInfoList &tmp = *all_list[i];
    count = 0;
    for (BadooLikeInfoList::const_iterator it = tmp.begin(); it != tmp.end(); ++it, ++count) {
      if (i < 2 && bidset.find(it->userId) != bidset.end())
        continue;
      getListInfo(*it, groups[i], count, msg);
    }
  }

  SendToProtoExpress(fun, msg, IQ_RESULT, "GetMeetPayList", from_jid, from_jid);

}


void MeetTask::notifyPresence(string &from_jid,
                    const BadooLikeInfoList &pay_list,
                    const BadooLikeInfoList &payed_list,
                    const BadooLikeInfoList &both_list
                    ) {

  const char *fun = "MeetTask::notifyPresence";
  // 获取遇见id列表
  const int CN_GROUP = 3;
  const BadooLikeInfoList *all_list[CN_GROUP] = {&pay_list, &payed_list, &both_list};
  map<int, int> r2bids;
  for (int i = 0; i < CN_GROUP; ++i) {
    const BadooLikeInfoList &tmp = *all_list[i];
    for (BadooLikeInfoList::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
      r2bids[it->userId] = it->badooUserId;
    }
  }

  vector<int> rids;
  for (map<int, int>::const_iterator it = r2bids.begin(); it != r2bids.end(); ++it) {
    rids.push_back(it->first);
  }
  // 取在线人的详细信息
  JidSeqMap jids_map;
  vector<string> sjids;

  try {
    jids_map = TalkFunStateAdapter::instance().getUsersJids(rids, 4);
  } catch (Ice::Exception &e) {
    MCE_WARN(fun << "-->TalkFunStateAdapter:getUsersJids error:" << e << " rids.size:" << rids.size());
  }
  MCE_INFO(fun << "-->get online size:" << rids.size() << " onlineusr:" << jids_map.size());
  IdWrappers bids;
  for (JidSeqMap::const_iterator it = jids_map.begin(); it != jids_map.end(); ++it) {
    map<int, int>::const_iterator jt = r2bids.find(it->first);
    if (jt != r2bids.end()) {
      IdWrapper tmp;
      tmp.id = jt->second;
      bids.push_back(tmp);
      for (JidSeq::const_iterator kt = it->second.begin(); kt != it->second.end(); ++kt) {
        sjids.push_back(jidToString(*kt));
      }
    }
  }
  BadooUserInfoList binfos;
  // --------------------
  if (getUserBaseInfoByBId(fun, binfos, bids)) {
    Str2StrMap msg_me;
    int count = 0;
    for (BadooUserInfoList::const_iterator it = binfos.begin(); it != binfos.end(); ++it, ++count) {
      getBaseInfo(*it, "info", count, msg_me);
    }
    SendToProtoExpress(fun, msg_me, OTHER, "MeetPresence", from_jid, from_jid);

  }

  MCE_INFO(fun << "-->presence size:" << sjids.size());
  if (IS_DEBUG_OUT) {
    for (vector<string>::const_iterator it = sjids.begin(); it != sjids.end(); ++it) {
      MCE_DEBUG(fun << "-->jid:" << *it);
    }
  }

  BadooUserInfo actor_info;
  if (!sjids.empty() && getUserBaseInfoByRId2(fun, actor_info, rid_)) {
    Str2StrMap msg_other;
    getBaseInfo(actor_info, "info", 0, msg_other);
    SendToProtoExpressWithJids(fun, msg_other, OTHER, "MeetPresence", from_jid, sjids);
  }

}
