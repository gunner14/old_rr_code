#ifndef TALKFUNTASK_H_
#define TALKFUNTASK_H_

#include "Util.h"
#include "TaskManager.h"
namespace com{
namespace xiaonei{
namespace talk{
namespace function{
namespace muc{

const int kVipMaxSizeOfSolidGroups = 60;
const int kUserMaxSizeOfSolidGroups = 40;
const int kVipMaxSizeOfSolidGroupMembers = 500;
const int kUserMaxSizeOfSolidGroupMembers = 200;
const int kVipMaxSizeOfDiscussGroups = 1000;
const int kUserMaxSizeOfDiscussGroups = 1000;
const int kVipMaxSizeOfDiscussGroupMembers = 20;
const int kUserMaxSizeOfDiscussGroupMembers = 20;
const int kSolidGroup = 0;
const int kDiscussGroup = 1;
const int kConversationGroup = 2;

class TimeStat{
	timeval _begin, _end;
	public:
	TimeStat(){
		reset();
	}
	void reset(){
		gettimeofday(&_begin, NULL);
	}
	float getTime(){
		gettimeofday(&_end, NULL);
		float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
			-_begin.tv_usec;
		timeuse/=1000;
		return timeuse;
	}
};

class TalkFunTask : public MyUtil::Task {
public:
  TalkFunTask(const MyUtil::Str2StrMap &paras, const std::string tpl_type, const std::string source_task): paras_(paras),
                      is_correct_(true), source_task_(source_task), config_type_(tpl_type) {
    common_data_["config_type"] = tpl_type;
    MyUtil::Str2StrMap::const_iterator iter;
    if ((iter=paras.find("actor_jid")) != paras.end())
      from_jid_ = common_data_["from_jid"] = iter->second;
  }
  void PrintResults(const MyUtil::Str2StrMap &print_map);
  void SendData();
  void SendOfflineAdminMsgs(MyUtil::Str2StrMap message,const MyUtil::IntSeq &receivers);
	virtual void handle();
  virtual void CreateResults() = 0;
protected:
  const std::string source_task_;
  const std::string config_type_;
  const MyUtil::Str2StrMap paras_;
  MyUtil::Str2StrMap common_data_;
//  MyUtil::Str2StrMap presence_data_;
  std::vector<MyUtil::Str2StrMap> presence_data_seq_;
  std::string code_;
  std::string description_;
  std::string from_jid_;
  bool is_correct_;
};

class CreateGroupTask : public TalkFunTask {
public:
  CreateGroupTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10000", "CreateGroupTask"){}
	virtual void CreateResults();
};

class SerachGroupTask : public TalkFunTask {
public:
  SerachGroupTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10001", "SerachGroupTask"){}
	virtual void CreateResults();
};

class JoinGroupTask : public TalkFunTask{
public:
  JoinGroupTask(const MyUtil::Str2StrMap &paras, const std::string tpl_type, const std::string source_task): TalkFunTask(paras, tpl_type, source_task) {}
protected:
  bool CheckGroupFull(const int owner_id, const int group_size);
  void JoinGroup(const Ice::Long room_id, const int userid);
  void SendPresence(const Ice::Long room_id, const int userid, const std::string &to_id);
};

class InviteToJoinGroupTask : public JoinGroupTask {
public:
  InviteToJoinGroupTask(const MyUtil::Str2StrMap &paras): JoinGroupTask(paras, "10014", "InviteToJoinGroupTask"){}
	virtual void CreateResults();
};

class ApplyToJoinGroupTask : public JoinGroupTask {
public:
  ApplyToJoinGroupTask(const MyUtil::Str2StrMap &paras): JoinGroupTask(paras, "10004", " ApplyToJoinGroupTask"){}
	virtual void CreateResults();
};

class SetGroupConfigTask : public TalkFunTask {
public:
  SetGroupConfigTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10005","SetGroupConfigTask"){}
	virtual void CreateResults();
};

class GetGroupConfigTask : public TalkFunTask {
public:
  GetGroupConfigTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10006", " GetGroupConfigTask"){}
	virtual void CreateResults();
};

class GetGroupLimitInfoTask : public TalkFunTask {
public:
  GetGroupLimitInfoTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10009", "GetGroupLimitInfoTask"){}
	virtual void CreateResults();
};

class GetMembersTask : public TalkFunTask {
public:
  GetMembersTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10002", "GetMembersTask"){}
	virtual void CreateResults();
};

class TransferGroupTask : public TalkFunTask {
public:
  TransferGroupTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10010", "TransferGroupTask"){}
	virtual void CreateResults();
};

class DestroyGroupTask : public TalkFunTask {
public:
  DestroyGroupTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10003", "DestroyGroupTask"){}
	virtual void CreateResults();
};

class QuitGroupTask : public TalkFunTask {
public:
  QuitGroupTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10012", "QuitGroupTask"){}
	virtual void CreateResults();
};

class ChangePrivilegeTask : public TalkFunTask {
public:
  ChangePrivilegeTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10013", "ChangePrivilegeTask"){}
	virtual void CreateResults();
};

class KickUsersTask : public TalkFunTask {
public:
  KickUsersTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10015", "KickUsersTask"){}
	virtual void CreateResults();
};

class SendMessageTask : public TalkFunTask {
public:
  SendMessageTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10020", "SendMessageTask"){}
	virtual void CreateResults();
};

class ChangeStatusPresenceTask : public TalkFunTask {
public:
  ChangeStatusPresenceTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10021", "ChangeStatusPresenceTask"){}
	virtual void CreateResults();
private:
  void SendPresence(const int userid, const string &online_type,const string &online_stat, const bool is_login);
};

class OfflineMessageTask : public TalkFunTask {
public:
  OfflineMessageTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10022", "OfflineMessageTask"){}
	virtual void CreateResults();
private:
  std::string FormatTimeStamp(std::string &time);
};

class AttachmentMessageTask : public TalkFunTask {
public:
  AttachmentMessageTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10023", "AttachmentMessageTask"){}
	virtual void CreateResults();
};

class GetGroupListTask : public TalkFunTask {
public:
  GetGroupListTask(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "10011", "GetGroupListTask"){}
	virtual void CreateResults();
private:
  struct GroupItemInfo {
    Ice::Long id;
    std::string name;
    std::string photo;
    std::string is_fixed;
    std::string affiliation;
    std::string announcement;
    Ice::Int owner;
    std::string introduction;
    std::string authentication;
    std::string notify;
    Ice::Int saveonline;
  };
  typedef std::map<Ice::Long,GroupItemInfo> GroupItemInfoMap;
  typedef std::vector<GroupItemInfo> GroupItemInfoSeq;
};

class Tester1Task : public TalkFunTask {
public:
  Tester1Task(const MyUtil::Str2StrMap &paras): TalkFunTask(paras, "11000", "Tester1Task"){}
	virtual void CreateResults();
};
};
};
};
};
};
#endif

