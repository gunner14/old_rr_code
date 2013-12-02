#ifndef __TALK_GROUP_LOADER_ICE__
#define __TALK_GROUP_LOADER_ICE__

#include <TalkCommon.ice>

module com
{
module xiaonei
{
module talk
{
module group 
{
module loader
{
	
	class GroupInfo{
		long id;
		string name;
		string head;
		int creator;
		string createTime;
		string introduction;
		int type;
		string lastContent;
		string lastTime;
    string announcement;
    string authentication;
    string notify;
    int saveonline;
    int level;
    int toplimit;
	};
	sequence<GroupInfo> GroupInfoSeq;
	dictionary<long,GroupInfo> GroupInfoDict;
	

	class TalkGroupLoaderManager{
		  GroupInfoDict LoadGroupInfo(MyUtil::LongSeq idSeq);
			//MyUtil::LongSeq GetCreatSessions(int creater);
		  bool SetGroupInfo(GroupInfo gi);  
		  bool AddGroupInfo(GroupInfo gi);  
			bool DeleteGroupInfo(MyUtil::LongSeq idSeq);
			bool UpdateGroupInfo(long sid, MyUtil::Str2StrMap props);
      bool UpdateLastContent(long sid, string lastcontent, string lasttime);

		  void SetGroupInfoAsyn(GroupInfo gi);
		  void AddGroupInfoAsyn(GroupInfo gi);
			void DeleteGroupInfoAsyn(MyUtil::LongSeq idSeq);
			void UpdateGroupInfoAsyn(long sid, MyUtil::Str2StrMap props);

	};
};
};
};
};
};

#endif
