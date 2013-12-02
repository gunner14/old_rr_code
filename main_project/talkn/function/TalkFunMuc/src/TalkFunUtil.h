#ifndef TALKFUNUTIL_H_
#define TALKFUNUTIL_H_
#include <Ice/Ice.h>
#include "Singleton.h"
#include "TalkCommon.h"
#include "TaskManager.h"
namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace muc{

enum MucOnlineType {
	UNKNOWN = 0,
	PHONE_ONLINE = 1,
	WEB_ONLINE = 2,
	WP_ONLINE = 4,
	IM_ONLINE = 8 
};
enum MucPrivilegeType {
  kOwner = 0,
  kAdmin = 1,
  kMember = 2
};

class TalkFunUtil :virtual public MyUtil::Singleton<TalkFunUtil> {
public:
  std::string GetIDFromJID(const std::string &rjid);
  std::string GetOnlineTypeFromJid(const std::string &jid);
  std::string GetOnlineTypeFromJids(const com::xiaonei::talk::common::JidSeq &jids);
  std::string GetOnlineTypeFromAvatars(const com::xiaonei::talk::common::AvatarSeq &avatars);
  std::string GetOnlineTypeFromType(const int type);
  std::string GetOnlineStatFromAvatars(const com::xiaonei::talk::common::AvatarSeq &avatars);
  bool GetOnlineTypeAndStateFromAvatars(const com::xiaonei::talk::common::AvatarSeq &avatars, const std::string &from_jid, 
      std::string &client_type, std::string &online_type, const bool is_login);

  bool CheckPassport(const std::string username, const std::string passport);
  int CheckVip(const Ice::Int uid);
  std::string XMLEncode(const std::string &source);
  std::string GetCurrentTime();
  void Map2String(const MyUtil::Str2StrMap &para, std::string &data);
  void String2Map(const std::string &data, MyUtil::Str2StrMap &para);

  std::string PrivilegeInt2String(const int level);
  int PrivilegeString2Int(const std::string &level);
  bool SpamString(const std::string &str, const int uid);
  std::string DBEscapeString(std::string &raw_string); 
  int GetHour();
  bool CheckApplyLimit(const int uid); 
  std::string CutUtf8(const string &src,const size_t limitlen); 
private:
  std::string OnlineStat2String(com::xiaonei::talk::common::OnlineStat state);
  com::xiaonei::talk::common::OnlineStat String2OnlineStat(std::string state);
  std::string MucOnlineType2String(MucOnlineType type);
  MucOnlineType String2MucOnlineType(std::string type);
};

class HourTimer: public MyUtil::Timer {
public:
  HourTimer(): Timer(60*60*1000){}
	virtual void handle();
};

const int apply_limit = 50;
class TalkFunCache :virtual public MyUtil::Singleton<TalkFunCache> {
public:
  bool CheckApplyLimit(const int uid);
  void ClearApplyCount();
private:
  MyUtil::Int2IntMap apply_count_;
  IceUtil::Mutex apply_count_mutex_;
};

}
}
}
}
}

#endif
