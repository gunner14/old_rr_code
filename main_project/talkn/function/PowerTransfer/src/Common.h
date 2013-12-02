#ifndef __TRANSFER_COMMON__
#define __TRANSFER_COMMON__

#include "Util.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "TalkCommon.h"


namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace transfer {

using namespace MyUtil;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;

enum OnlineType {
  UNKNOWTYPE = 0,
  WEB_ONLINE = 1,
  WP_ONLINE = 2,
  IM_ONLINE = 4,
  PHONE_ONLINE = 8, // PTalkPROXY_ONLINE or PIMSTORM_ONLINE
  PTalkPROXY_ONLINE = 16,
  PIMSTORM_ONLINE = 32
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

int Xml2Map(const string& FuncName, const string& xml, Str2StrMap& map);
void ClientEncode(string& body_string);
void Send2Proto(vector<string>& to_jids, Str2StrMap & msg);

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

//realtime feed reply
const int FEED_REPLY_BASE_CONFIG_TYPE = 110000;
const string FEED_REPLY_BASE_CONFIG_TYPE_STR = "110000";
const string FEED_BASE_CONFIG_TYPE_STR = "100000";
const string FEED_WEBPAGER_ITEM = "webpager";

const Ice::Long FEED_BASE_ID = 6100000000000000000;
const string PHOTO_SERVER = "http://photo.renren.com";

};
};
};
};
};

#endif
