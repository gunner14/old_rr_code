#ifndef __TALK_GROUP_LOADER_MEMBER_I_H__
#define __TALK_GROUP_LOADER_MEMBER_I_H__

#include "ServiceI.h"
#include "TalkGroupMemberLoader.h"
#include "FeedMemcProxy/client/talk_group_member_list.h"
#include<set>
namespace com {
namespace xiaonei {
namespace talk {
namespace group {
namespace memberloader{
using namespace std;
using namespace MyUtil;
//using namespace xce::talk;
using namespace com::xiaonei::talk::common;
const char *DB_INSTANCE = "im_groupchat";
const char *DB_ROUTE = "session_member_str";
const char *DB_PREF = "session_member_str";

class TalkGroupMemberLoaderManagerI : public TalkGroupMemberLoaderManager, public Singleton<TalkGroupMemberLoaderManagerI> {
  bool getGroupMemberFromDb(const MyUtil::LongSeq& idSeq, GroupMemberDict& member_list);
  bool getGroupMemberFromDbOneMod(long mod, const MyUtil::LongSeq& idSeq, GroupMemberDict& member_list);
  void SerializeMembers(const GroupMemberSeq& members, string &info_str);
  void strSplit(const string &list_str,GroupMemberSeq& group_member);

  bool updateMembers(const GroupMemberDict& memberdict);
  bool updateMembersOneMod(long mod, const GroupMemberDict& memberdict);

  bool DeleteGroup_(const MyUtil::LongSeq &idSeq);
  bool DeleteGroupOneMod(long mod, const MyUtil::LongSeq &idSeq);
  bool AddNewMember_(const long sid, const GroupMemberSeq &members);
  bool RemoveMember_(const long sid, const MyUtil::IntSeq &idSeq);
  bool SetMember_(const GroupMemberDict& memberdict);

  enum {
    TABLE_COUNT = 100,
  };

  // 统计需要，把rpc接口，和具体实现分开了
 public:
    TalkGroupMemberLoaderManagerI();
		virtual GroupMemberDict LoadGroupMember(const MyUtil::LongSeq& idSeq, const Ice::Current& = Ice::Current());
    // 同步接口
		virtual bool AddNewGroup(const GroupMemberDict& memberdict, const Ice::Current& = Ice::Current());
		virtual bool DeleteGroup(const MyUtil::LongSeq &idSeq, const Ice::Current& = Ice::Current());
		virtual bool AddNewMember(const long sid, const GroupMemberSeq &members, const Ice::Current& = Ice::Current());
		virtual bool RemoveMember(const long sid, const MyUtil::IntSeq &idSeq, const Ice::Current& = Ice::Current());
		virtual bool SetMember(const GroupMemberDict& memberdict, const Ice::Current& = Ice::Current());
    // 异步接口
		virtual void AddNewGroupAsyn(const GroupMemberDict& memberdict, const Ice::Current& = Ice::Current());
		virtual void DeleteGroupAsyn(const MyUtil::LongSeq &idSeq, const Ice::Current& = Ice::Current());
		virtual void AddNewMemberAsyn(const long sid, const GroupMemberSeq &members, const Ice::Current& = Ice::Current());
		virtual void RemoveMemberAsyn(const long sid, const MyUtil::IntSeq &idSeq, const Ice::Current& = Ice::Current());
		virtual void SetMemberAsyn(const GroupMemberDict& memberdict, const Ice::Current& = Ice::Current());

	private:
		set<string> _prop;

};
}
;
}
;
}
;
}
;
}
;

#endif

