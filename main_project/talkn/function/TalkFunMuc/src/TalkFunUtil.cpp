#include "TalkFunUtil.h"
#include "TalkCommon.h"
#include "LoginMsgHandlerAdapter.h"
#include "UserCacheAdapter.h"
#include "XmppTools.h"
#include <boost/format.hpp> 
#include "TalkUtil/src/pugixml.hpp"
#include "talk/SpamClient/src/OzeinFilterAdapter.h"
using namespace com::xiaonei::ozein::content::generation;
using namespace com::xiaonei::talk::function::muc;
using namespace com::xiaonei::talk;
using namespace xce::usercache;
using namespace com::xiaonei::talk::common;
using namespace std;
using namespace pugi;
using namespace talk::adapter;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;

string TalkFunUtil::OnlineStat2String(OnlineStat state) {
  string ret_state = "offline";
  if (state == INVISIABLE)
    ret_state = "invisiable";
  else if (state == NOTAVAILABLE)
    ret_state = "notavailable";
  else if (state == BUSY)
    ret_state = "dnd";
  else if (state == AWAY)
    ret_state = "away";
  else if (state == ONLINE)
    ret_state = "online";
  return ret_state;
}

OnlineStat TalkFunUtil::String2OnlineStat(string state) {
  OnlineStat ret_state = OFFLINE;
  if (state == "online")
    ret_state = ONLINE;
  else if (state == "away")
    ret_state = AWAY;
  else if (state == "dnd")
    ret_state = BUSY;
  else if (state == "notavailable")
    ret_state = NOTAVAILABLE;
  else if (state == "invisiable")
    ret_state = INVISIABLE;
  return ret_state;
}

string TalkFunUtil::MucOnlineType2String(MucOnlineType type) {
  string ret_type;
  if (type == IM_ONLINE) 
    ret_type = "xntalk";
  else if (type == WP_ONLINE)
    ret_type = "web_pager";
  else if (type == WEB_ONLINE)
    ret_type = "web";
  else if (type == PHONE_ONLINE)
    ret_type = "phone";
  else
    ret_type == "unknown";
  return ret_type;
}
  
MucOnlineType TalkFunUtil::String2MucOnlineType(string type) {
  MucOnlineType ret_type = UNKNOWN;
  if (type == "xntalk")
    ret_type = IM_ONLINE;
  else if (type == "web_pager") 
    ret_type = WP_ONLINE;
  else if (type == "web")
    ret_type = WEB_ONLINE;
  else if (type == "phone")
    ret_type = PHONE_ONLINE;
  return ret_type; 
}

string TalkFunUtil::GetIDFromJID(const string &jid) {
  string ret;
  const size_t at_pos = jid.find('@');
  if (at_pos != string::npos) {
    ret = jid.substr(0,at_pos);
  }
  return ret;
}

string TalkFunUtil::GetOnlineTypeFromJid(const string &jid_str) {
  JidSeq jids;
  JidPtr jid = stringToJid(jid_str);
  if (jid) 
    jids.push_back(jid);  
  return GetOnlineTypeFromJids(jids);
}

/*说明：根据avatars取出优先级最高的在线状态
 * 参数1：一组avatar
 * 返回值：返回的在线状态
 */
string TalkFunUtil::GetOnlineTypeFromAvatars(const com::xiaonei::talk::common::AvatarSeq &avatars) {
  JidSeq jids;
  for (AvatarSeq::const_iterator avatar_iter=avatars.begin(); avatar_iter!=avatars.end(); avatar_iter++) 
    jids.push_back((*avatar_iter)->jid);  
  return GetOnlineTypeFromJids(jids);
}

/*说明：根据jids取出优先级最高的在线状态
 * 参数1：一组jid
 * 返回值：返回的在线状态
 */
string TalkFunUtil::GetOnlineTypeFromJids(const JidSeq &jids) {
  MucOnlineType online_type = UNKNOWN;
  for (JidSeq::const_iterator jid_iter=jids.begin(); jid_iter!=jids.end(); jid_iter++) {
    char first_char = ((*jid_iter)->endpoint).at(0); 
    if ('T'==first_char && online_type<IM_ONLINE)
      online_type = IM_ONLINE;
    else if ('W'==first_char && online_type<WP_ONLINE)
      online_type = WP_ONLINE;
    else if ('w'==first_char && online_type<WEB_ONLINE)
      online_type = WEB_ONLINE;
    else if ('P'==first_char && online_type<PHONE_ONLINE)
      online_type = PHONE_ONLINE;
  }
  return MucOnlineType2String(online_type);
}

/*说明：根据avatars取出优先级最高的人人桌面在线类型
 * 参数1：一组avatar
 * 返回值：返回的在线状态
 */
string TalkFunUtil::GetOnlineStatFromAvatars(const AvatarSeq &avatars) {
  OnlineStat temp_online_stat = ONLINE;
  for (AvatarSeq::const_iterator avatar_iter=avatars.begin(); avatar_iter!=avatars.end(); avatar_iter++) {
    if ((*avatar_iter)->stat < temp_online_stat)
      temp_online_stat = (*avatar_iter)->stat;
  }
  return OnlineStat2String(temp_online_stat);
}

/*说明：根据avatars取出优先级最高的现在类型和人人桌面在线类型
 * 参数1：一组avatar
 * 参数2：待检测jid
 * 参数3：现在状态
 * 参数3：现在类型
 * 返回值：返回的在线状态
 */
bool TalkFunUtil::GetOnlineTypeAndStateFromAvatars(const AvatarSeq &avatars, const string &from_jid,  
    string &online_type_str, string &online_stat_str, const bool is_login) {
  if (avatars.empty() && !is_login)
    return true;
  if (avatars.empty())
    return false;
  MucOnlineType online_type = String2MucOnlineType(online_type_str);
  MucOnlineType temp_online_type = UNKNOWN;
  OnlineStat online_stat= String2OnlineStat(online_stat_str);
  OnlineStat temp_online_stat = ONLINE;
  for (AvatarSeq::const_iterator avatar_iter=avatars.begin(); avatar_iter!=avatars.end(); avatar_iter++) {
    if (jidToString((*avatar_iter)->jid) == from_jid)
      continue;
    char first_char = (((*avatar_iter)->jid)->endpoint).at(0); 
    if ('T'==first_char && temp_online_type<IM_ONLINE)
      temp_online_type = IM_ONLINE;
    else if ('W'==first_char && temp_online_type<WP_ONLINE)
      temp_online_type = WP_ONLINE;
    else if ('w'==first_char && temp_online_type<WEB_ONLINE)
      temp_online_type = WEB_ONLINE;
    else if ('P'==first_char && temp_online_type<PHONE_ONLINE)
      temp_online_type = PHONE_ONLINE;
    else if ('T'==first_char && temp_online_type==IM_ONLINE && temp_online_stat>(*avatar_iter)->stat)
      temp_online_stat = (*avatar_iter)->stat; 
  }
  if (online_type > temp_online_type) {
    if (!is_login)
      online_type_str = MucOnlineType2String(temp_online_type);
    return true;
  }
  if (online_type==temp_online_type && online_type==IM_ONLINE && online_stat<temp_online_stat) {
    if (!is_login)
      online_stat_str = OnlineStat2String(temp_online_stat);
    return true;
  }
  return false;
} 

string TalkFunUtil::GetOnlineTypeFromType(const int type) {
  string online_type;
  if (type == WEB_ONLINE)
    online_type = "web";
  else if (type == WP_ONLINE)
    online_type = "web pager";
  else if (type == IM_ONLINE)
    online_type = "talk";
  else if (type == PHONE_ONLINE)
    online_type = "phone";
  return online_type;
}
  
/*说明：检测帐号时候是 vip
 * 参数1: 待检测 id
 * 返回值：是否是vip
 */
int TalkFunUtil::CheckVip(const Ice::Int uid) {
	MCE_DEBUG("TalkFunUtil::CheckVip-->userId:"<<uid);
  int vip = 1;   //普通用户
  IntSeq vip_uids; 
  vip_uids.push_back(uid);
	map<int, UserCachePtr> userVips;
	try{
		userVips = UserCacheAdapter::instance().GetUserCache(vip_uids);
	}catch(Ice::Exception& e){
		MCE_WARN("TalkFunUtil::CheckVip-->UserCacheAdapter::GetUserCache-->userId:"<<uid<<" Ice::Exception:"<<e);
    return 0;  //错误返回
	}
	MCE_DEBUG("TalkFunUtil::CheckVip-->ssss:");
	UserCachePtr vipInfo; 
	map<int, UserCachePtr>::iterator it = userVips.find(uid);
	if(it != userVips.end()) {
		if(it->second->IsVipMember()) 
      vip = 2;    // VIP
	}
	MCE_DEBUG("TalkFunUtil::CheckVip-->userId:"<<uid <<"   vip:"<<vip);
  return vip;
}

/*说明：检测帐号状态
 * 参数1: 登录帐号
 * 参数2: 登录密码
 * 返回值：是否登录成功
 */
bool TalkFunUtil::CheckPassport(const string user, const string pass) {
  ostringstream os;
  os << "md5_" << pass;
	PassportPtr passport;
	//try two times
	for(int i=0; i<2; i++){
		try {
			passport = LoginMsgHandlerAdapter::instance().verify(user, os.str());
		} catch(Ice::Exception& e) {
			MCE_WARN("TalkFunUtil::CheckPassport --> user:" << user <<"  password:"<< pass<<"     LoginMsgHandlerAdapter::instance().verify    err:" <<e);
			continue;
		}
		break;
	}
	if(!passport){
//		error_msg = "TalkFunUtil::CheckPassport --> verify exception user:"<<user<<"  password:"<<pass;
    return false;
	}
  
  bool ret = false;
	if (passport->result == -1) {
		// user or password invalid
	} else if (passport->result == -2) {
		// invalid user status
	} else 
    ret = true;
  return ret; 
}
  

string TalkFunUtil::GetCurrentTime() {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  return str(boost::format("%4d-%02d-%02d %02d:%02d:%02d") %(tm->tm_year + 1900) %(tm->tm_mon + 1) %tm->tm_mday %tm->tm_hour %tm->tm_min %tm->tm_sec);
}
  
void TalkFunUtil::Map2String(const Str2StrMap &para, string &data) {
  ostringstream os;
  os << "<data>";
  for (Str2StrMap::const_iterator iter=para.begin(); iter!=para.end(); iter++) {
    os << "<item key=\"" << iter->first <<"\" value=\"" << iter->second <<"\" />";
  }
  os << "</data>";
  data = os.str();
}
  
void TalkFunUtil::String2Map(const string &data, Str2StrMap &para) {
	xml_document xml;
	if (xml.load(data.c_str())) {
    xml_node data_node = xml.child("data");
	  pugi::xml_node_iterator iter;
    for (iter=data_node.begin(); iter!=data_node.end(); iter++) {
      xml_attribute key_attr = iter->attribute("key");
      xml_attribute value_attr = iter->attribute("value");
      if (key_attr && value_attr)
        para.insert(pair<string,string>(key_attr.value(), value_attr.value()));
    }
	} 
}

string TalkFunUtil::PrivilegeInt2String(const int level) {
  string ret;
  if (level == kOwner)
    ret = "owner";
  else if (level == kAdmin)
    ret = "admin";
  else if (level == kMember)
    ret = "member";
  return ret;
}
  
int TalkFunUtil::PrivilegeString2Int(const std::string &level) {
  int ret = -1;
  if (level == "owner")
    ret = kOwner;
  else if (level == "admin")
    ret = kAdmin;
  else if (level == "member")
    ret = kMember;
  return ret;
}

string TalkFunUtil::XMLEncode(const string &source) {
  ostringstream ss;
  for (size_t i=0; i<source.length(); ++i) {
    switch (source[i]) {
      case '&':
         ss << "&amp;";
        break;
      case '<':
        ss << "&lt;";
        break;
      case '>':
        ss << "&gt;";
        break;
      case '\'':
        ss << "&apos;";
        break;
      case '"':
        ss << "&quot;";
        break;
      default:
        ss <<  source[i];
    }
  }
  return ss.str();
}

/*说明：过滤非法字符串
 * 参数1: 字符串
 * 参数2: 用户id
 * 返回值：字符串是否合法
 */
bool TalkFunUtil::SpamString(const string &str, const int uid) {
  bool ret = true;
	vector<string> v; 
	v.push_back(str);
  try {
    CheckResultMore spamobj = OzeinFilterClient::instance().checkerAndFilter(uid, -1, 108, v, ""); 
    if (spamobj.getFlag() == 30) 
      ret = false; 
  } catch (Ice::Exception& e) {
    MCE_WARN("TalkFunUtil::SpamString -->  uid:" << uid <<
             "call OzeinFilterClient.checkerAndFilter ERR  line:"<<__LINE__<<" err:"<<e);
    ret = false;
  } catch (std::exception& e) {
    MCE_WARN("TalkFunUtil::SpamString --> uid:" << uid <<
             "call OzeinFilterClient.checkerAndFilter ERR  line:"<<__LINE__<<" err:"<<e.what());
    ret = false;
  }
  return ret;
}

string TalkFunUtil::DBEscapeString(string &raw_string) {
  ostringstream os;
  for (size_t i=0; i<raw_string.length(); ++i) {
    if (raw_string[i]=='\'' ||raw_string[i]=='_' ||raw_string[i]=='%' ||raw_string[i]=='\"' ) {
      os << '\\';
    }
    os << raw_string[i];
  }
  return os.str();
}

int TalkFunUtil::GetHour() {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  return tm->tm_hour;
};

string TalkFunUtil::CutUtf8(const string &src,const size_t limitlen) {
  float len = 0;
  size_t i = 0;

  while (i < src.length()) {                      
    unsigned char current_char = src[i];    
    if (current_char>=0x00 && current_char<=0x7F)   
      len+=1, ++i;
    else if (current_char>=0xC0 && current_char<=0xDF) {
      current_char = src[i];
      if (current_char>=0x80 && current_char<=0xBF)   
        ++len;
      else
        break;
      i += 2;
    } else if (current_char>=0xE0 && current_char<=0xEF) {
      size_t j = 1 ;
      for ( ; j<3; ++j) {
        current_char = src[i+j];
        if (!(current_char>=0x80 && current_char<=0xBF))    
          break;
      }
      if (3 == j)
        ++len;
      else
        break;
      i += 3;
    }
    else if (current_char>=0xF0 && current_char<=0xF7) {
      size_t j = 1 ;
      for ( ; j<4; ++j) {
        current_char = src[i+j];
        if (!(current_char>=0x80 && current_char<=0xBF))            
          break;
      }
      if (4 == j)
        ++len;
      else
        break;
      i += 4;
    }
    else 
      break;

    if (limitlen <= (size_t)len)
      break;
  }
  return src.substr(0, i);
}

bool TalkFunUtil::CheckApplyLimit(const int uid) {
  return TalkFunCache::instance().CheckApplyLimit(uid);
}
bool TalkFunCache::CheckApplyLimit(const int uid) {
  bool ret = true;
  IceUtil::Mutex::Lock  lock(apply_count_mutex_);
  Int2IntMap::iterator apply_count_iter = apply_count_.find(uid);
  if (apply_count_iter == apply_count_.end()) {
    apply_count_[uid] = 0;
  } else {
    if (apply_count_iter->second >= apply_limit)
      ret = false;
    else
      apply_count_iter->second++;
  }
  return ret;
}

void TalkFunCache::ClearApplyCount() {
  IceUtil::Mutex::Lock  lock(apply_count_mutex_);
  Int2IntMap().swap(apply_count_); 
}

void HourTimer::handle() {
  if (TalkFunUtil::instance().GetHour() == 0)
    TalkFunCache::instance().ClearApplyCount();
}

