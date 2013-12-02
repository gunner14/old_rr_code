#ifndef __TALK_GROUP_LOADER_I_H__
#define __TALK_GROUP_LOADER_I_H__

#include "ServiceI.h"
#include "TalkGroupLoader.h"
#include "FeedMemcProxy/client/talk_group_client.h"
#include<set>
namespace com {
namespace xiaonei {
namespace talk {
namespace group {
namespace loader{
using namespace std;
using namespace MyUtil;
//using namespace xce::talk;
using namespace com::xiaonei::talk::common;
const char *DB_INSTANCE = "im_groupchat";
const char *DB_ROUTE = "session_info";
const char *DB_PREF = "session_info";



class TalkGroupLoaderManagerI : public TalkGroupLoaderManager, public Singleton<TalkGroupLoaderManagerI> {
	public:

    TalkGroupLoaderManagerI();
		virtual GroupInfoDict LoadGroupInfo(const MyUtil::LongSeq& idSeq, const Ice::Current& = Ice::Current());
		//virtual MyUtil::LongSeq GetCreatSessions(int creater, const Ice::Current& = Ice::Current());
		virtual bool AddGroupInfo(const GroupInfoPtr& info, const Ice::Current& = Ice::Current());
		virtual bool SetGroupInfo(const GroupInfoPtr& info, const Ice::Current& = Ice::Current());
		virtual bool DeleteGroupInfo(const MyUtil::LongSeq& idSeq, const Ice::Current& = Ice::Current());
		virtual bool UpdateGroupInfo(long sid, const MyUtil::Str2StrMap& props, const Ice::Current& = Ice::Current());


		virtual void AddGroupInfoAsyn(const GroupInfoPtr& info, const Ice::Current& = Ice::Current());
		virtual void SetGroupInfoAsyn(const GroupInfoPtr& info, const Ice::Current& = Ice::Current());
		virtual void DeleteGroupInfoAsyn(const MyUtil::LongSeq& idSeq, const Ice::Current& = Ice::Current());
		virtual void UpdateGroupInfoAsyn(long sid, const MyUtil::Str2StrMap& props, const Ice::Current& = Ice::Current());
    virtual bool UpdateLastContent(long sid, const string &lastcontent, const string &lasttime, const Ice::Current& = Ice::Current());

 private:
    enum {
      TABLE_COUNT = 100,
    };

    bool AddGroupInfo_(const GroupInfoPtr& info);
    bool SetGroupInfo_(const GroupInfoPtr& info);
    bool DeleteGroupInfo_(const MyUtil::LongSeq& idSeq);
    bool DeleteGroupInfoOneMod(long mod, const MyUtil::LongSeq& idSeq);
    bool UpdateGroupInfo_(long sid, const MyUtil::Str2StrMap& props);
    bool UpdateLastContent_(long sid, const string &lastcontent, const string &lasttime);

    GroupInfoPtr GenInfo(const Ice::Long session_id, const string& session_name,  
		const string& session_head, int session_creater,  const string& create_time, 
		const string& session_intro, int session_type, const string& session_last_content, const  string& session_last_time,
		const string& announcement, const string& authentication, const string& notify,const int level,
    const int toplimit, const int saveonline); 
		bool getInfoFromDbOneMod(long mod, const MyUtil::LongSeq& idSeq, GroupInfoDict& infos);
		bool getInfoFromDb(const MyUtil::LongSeq& idSeq, GroupInfoDict& infos);
		Ice::Long identifyGroupInfo(MyUtil::Str2StrMap props);
    string EscapeString(const string &raw_string, const bool mode=0); 
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

