#ifndef __TALK_GROUPVIEW_LOADER_I_H__
#define __TALK_GROUPVIEW_LOADER_I_H__

#include "ServiceI.h"
#include "TalkGroupViewLoader.h"
#include "FeedMemcProxy/client/talk_group_view_client.h"
namespace com {
namespace xiaonei {
namespace talk {
namespace groupview {
namespace loader {
using namespace MyUtil;
	
const char *DB_INSTANCE = "im_groupchat";
const char *DB_ROUTE = "session_view";
const char *DB_PREF = "session_view";

class GroupViewLoaderManagerI : public GroupViewLoaderManager, public Singleton<GroupViewLoaderManagerI> {


public:
	enum {
		LEN_SOLID_GROUP = 100, // 固定群数量
		LEN_DISCUSS_GROUP = 100, //讨论组
		LEN_CONVERSATION_GROUP = 100,//两人聊天
	};

	virtual	bool Update(int uid, const TalkGroupViewPtr &groupview, const Ice::Current& = Ice::Current());
	virtual	bool Delete(int uid, long sessionid, int type, const Ice::Current& = Ice::Current());
	virtual	bool UpdateMsgid(int uid, long sessionid, int type, long msgid, const Ice::Current& = Ice::Current());
  virtual bool UpdateItem(int userid, long sid, const Str2StrMap &props, const Ice::Current& = Ice::Current());

  virtual void UpdateItemAsyn(int userid, long sid, const Str2StrMap &props, const Ice::Current& = Ice::Current());
	virtual	void UpdateAsyn(int uid, const TalkGroupViewPtr &groupview, const Ice::Current& = Ice::Current());
	virtual	void DeleteAsyn(int uid, long sessionid, int type, const Ice::Current& = Ice::Current());
  virtual MyUtil::LongSeq GetCreatSessions(int creater, const Ice::Current& = Ice::Current());
  //	virtual	TalkGroupViewSeq LoadById(int uid, int type, const Ice::Current& = Ice::Current());
	virtual	TalkGroupViewSeqMap LoadBySeq(const MyUtil::IntSeq &owners, int type, const Ice::Current& = Ice::Current());

 private:
  enum {
    TABLE_COUNT = 100,
  };


	TalkGroupViewPtr GenInfo(int uid, long sessionid,int isvisible, int hasread, int sessiontype, const string &updatetime, int affiliation, int saveonline, const string &notify, long msgid);
	bool LocateUser(const MyUtil::IntSeq &owners, int type, TalkGroupViewSeqMap &gviews);
	bool LocateUserOneMod(long mod, const MyUtil::IntSeq &owners, int type, TalkGroupViewSeqMap &gviews);
	TalkGroupViewPtr LocateOneItem(int uid, long sessionid);
	bool Update_(int uid, const TalkGroupViewPtr &groupview);
  bool Delete_(int uid, long sessionid, int type);
  bool UpdateMsgid_(int uid, long sessionid, int type, long msgid);

  bool UpdateItem_(int userid, long sid, const Str2StrMap &props);
  TalkGroupViewPtr MergeNewView(int userid, long sid, const Str2StrMap &props);

  //  void UpdateCache();
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

