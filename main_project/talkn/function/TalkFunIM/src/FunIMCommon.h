#ifndef __FUN_IM_COMMON__
#define __FUN_IM_COMMON__

#include "TaskManager.h"
#include "Util.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "TalkCommon.h"
#include "UserCacheAdapter.h"

#define TALKFUNIM_TPL_TYPE "30000"

using namespace xce::usercache;

namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace im {

using namespace MyUtil;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;

enum OnlineType {
  UNKNOWTYPE = 0,
  WEB_ONLINE = 1,
  WP_ONLINE = 2,
  IM_ONLINE = 4,
  PHONE_ONLINE = 8 
};

enum OnlineStatN {
  ONLINEN = 1,
  AWAYN = 2,
  BUSYN = 3,
  NOTAVAILABLEN = 4,
  INVISIABLEN = 5
};

//封装boost::lexical_cast, T1转成T2 
template<typename T1, typename T2>
void LexicalCast(const T1 &a, T2 &b) {
  try {
    b = boost::lexical_cast<T2>(a);
  } catch(boost::bad_lexical_cast& e) {
    MCE_WARN("TalkFunIM::LexicalCast" << e.what());
  }
}

inline void ParasGetValue(const string& key, string& value, const Str2StrMap& paras) {
  Str2StrMap::const_iterator i = paras.find(key);
  if(i != paras.end())
    value = i->second;
}

void SendToProto(const string& func_name, Str2StrMap &msg, const int& message_type, 
  const string& item_type, const string& from_jid, const int& to_id, const int& online_type);
void SendToProtoExpress(const string& func_name, Str2StrMap &msg, const int& message_type, 
  const string& item_type, const string& from_jid, string& to_jid);
void SendToProtoExpressWithJids(const string& func_name, Str2StrMap &msg, const int& message_type, 
  const string& item_type, const string& from_jid, vector<string>& to_jid);
void SendToProtoWithIds(const string& func_name, Str2StrMap &msg, const int& message_type, 
  const string& item_type, const string& from_jid, vector<int>& to_id, const int& online_type);

void CheckVip(string& vip_string, const int& uid);
void CheckVipWithoutGet(string& vip_string, const int& uid, map<int, UserCachePtr>& userVips);


inline OnlineType GetOnlineTypeFromEndpoint(const string& jid){
  if(jid.empty())
    return UNKNOWTYPE;
  if ('W' == jid.at(0)) {
    return WP_ONLINE;
  } else if ('T' == jid.at(0)) {
    return IM_ONLINE;
  } else if ('w' == jid.at(0)){
    return WEB_ONLINE;
  } else if ('P' == jid.at(0)) {
    return PHONE_ONLINE;
  } else{
    return UNKNOWTYPE;
  }
}

inline OnlineStat OnlineStatNToOnlineStat(OnlineStatN stat) {
  switch(stat) {
    case ONLINEN:
      return ONLINE;
    case AWAYN:
      return AWAY;
    case BUSYN:
      return BUSY;
    case NOTAVAILABLEN:
      return NOTAVAILABLE;
    case INVISIABLEN:
      return INVISIABLE;
    default:
      return ONLINE;
  }
}

};
};
};
};
};

#endif
