#ifndef __TALK_GROUP_MEMBER_LOADER_ICE__
#define __TALK_GROUP_MEMBER_LOADER_ICE__

#include <TalkCommon.ice>

module com
{
module xiaonei
{
module talk
{
module group 
{
module memberloader
{
  class GroupMemberInfo {
    int uid;
    string name;
    string affiliation;
    string network;
    string gender;
    string state;
    string tinyurl;
    string equipment;
  };
	sequence<GroupMemberInfo> GroupMemberInfoSeq;
  dictionary<int,GroupMemberInfo> GroupMemberInfoDict;	
  
  enum PRIVILEGE {
    OWNER, ADMIN, MEMBER
  }; 
	class GroupMember{
		int uid;
		int level;
	};
	sequence<GroupMember> GroupMemberSeq;
  dictionary<long,GroupMemberSeq> GroupMemberDict;	

	class TalkGroupMemberLoaderManager{
		  GroupMemberDict LoadGroupMember(MyUtil::LongSeq idSeq);
      // 同步接口
		  bool AddNewGroup(GroupMemberDict memberdict);
			bool DeleteGroup(MyUtil::LongSeq idSeq);
			bool AddNewMember(long sid, GroupMemberSeq members);
			bool RemoveMember(long sid, MyUtil::IntSeq idSeq);
      bool SetMember(GroupMemberDict memberdict);
      // 异步接口
		  void AddNewGroupAsyn(GroupMemberDict memberdict);
			void DeleteGroupAsyn(MyUtil::LongSeq idSeq);
			void AddNewMemberAsyn(long sid, GroupMemberSeq members);
			void RemoveMemberAsyn(long sid, MyUtil::IntSeq idSeq);
      void SetMemberAsyn(GroupMemberDict memberdict);

	};
};
};
};
};
};

#endif
