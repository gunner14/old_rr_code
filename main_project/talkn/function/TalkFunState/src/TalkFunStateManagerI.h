#ifndef __TALK_FUN_STATE_MANAGERI__
#define __TALK_FUN_STATE_MANAGERI__


#include "ServiceI.h"
#include "TalkFunState.h"
#include "Cache.h"
namespace com{
namespace xiaonei{
namespace talk{
namespace fun{
namespace state{

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;

const float TIMEOUT = 200.0;
//const int DOINGPOOLSIZE = 49999;
const int DOINGPOOLSIZE = 3089;

enum OnlineType {
  UNKNOWTYPE = 0,
  WEB_ONLINE = 1,
  WP_ONLINE = 2,
  IM_ONLINE = 4,
  PHONE_ONLINE = 8
};


class TalkFunStateManagerI: public TalkFunStateManager, public Singleton<TalkFunStateManagerI>{
public:

 TalkFunStateManagerI() : heart_stamp_(time(NULL)) {
  }

	void InitDoingPool(){
		for(int i = 0; i <= DOINGPOOLSIZE; ++i){
			DoingCachePtr ptr = new DoingCache();
			doingpool_.push_back(ptr);
		}
	}

	void DealDoingCache(){
		for(vector<DoingCachePtr>::iterator it = doingpool_.begin(); it != doingpool_.end(); ++it){
			(*it)->DealDelay();
		}
	}
	void DealDoingCache(int index){
		if(index <0 || index >= DOINGPOOLSIZE){
			return;
		}
		doingpool_[index]->DealDelay();
	}

	virtual void HeartBeat(const JidSeq& seq, const Ice::Current& = Ice::Current());
	virtual void HeartBeatNotSplit(const JidSeq &seq, const Ice::Current& = Ice::Current());

	virtual AvatarNSeqMap GetUsersOnlineInfo(const MyUtil::IntSeq& uids, bool checkheartbeat, const Ice::Current& = Ice::Current());
	virtual AvatarNSeq GetUserOnlineInfo(int userid, bool checkheartbeat, const Ice::Current& = Ice::Current());
	virtual int GetUserOnlineType(int userid, const Ice::Current& = Ice::Current());
	virtual int GetUserOnlineStat(int userid, const Ice::Current& = Ice::Current());
	virtual void SetUserOnlineInfo(int userid, const AvatarNSeq& seq, const Ice::Current& = Ice::Current());
	virtual void AvatarNOnline(const AvatarNPtr& avatarn, const Ice::Current& = Ice::Current());
	virtual void AvatarNOffline(const JidPtr& jid, const Ice::Current& = Ice::Current());
	virtual void ClearUserOnlineInfo(int userid, const Ice::Current& = Ice::Current());

	//bool GetRightHeartUser(AvatarNSeq& ans);

	//兼容老接口
  virtual UserOnlineTypeSeq getOnlineUsers(const MyUtil::IntSeq& uids,const Ice::Current& = Ice::Current()); 
  virtual JidSeqMap getUsersJids(const MyUtil::IntSeq& ids, int onlineType, const Ice::Current& = Ice::Current());
	virtual JidSeq getUserJids(int userId, int onlineType, const Ice::Current& = Ice::Current());
	virtual AvatarPtr getAvatar(const JidPtr& jid, const Ice::Current& = Ice::Current());
	virtual AvatarSeq getAvatars(const JidSeq& jids, const Ice::Current& = Ice::Current());
	virtual AvatarSeq getUserAvatars(int userId, int onlineType, const Ice::Current& = Ice::Current());
  virtual AvatarMap getUsersAvatars(const MyUtil::IntSeq& ids, int onlineType, const Ice::Current& = Ice::Current());
  virtual JidSeqMap getBuddyJidsAndSelf(int userId, int buddyOnlineType, int selfOnlineType, const Ice::Current& = Ice::Current());
	virtual JidSeqMap getBuddyJids(int userId, int onlineType, const Ice::Current& = Ice::Current());
  virtual AvatarMap getBuddyAvatars(int userId, int onlineType, const Ice::Current& = Ice::Current());
  virtual AvatarMap getBuddyAvatarsAndSelf(int userId, int buddyOnlineType, int selfOnlineType, const Ice::Current& = Ice::Current());

	virtual void UseBackup(bool use, const Ice::Current& = Ice::Current()){
		usebackup_ = use;
		MCE_INFO("UseBackup--> usebackup_:" << usebackup_);
	}

	bool usebackup_;
	
	vector<DoingCachePtr> doingpool_;

 private:
	IceUtil::Mutex heart_mutex_;
  time_t heart_stamp_;
	JidSeq heart_seq_;
  bool addHeartBeat(const JidSeq &seq, JidSeq &cache);
  bool getUserAvatars_(int userId, int onlineType, AvatarSeq &ans);
  bool getUsersAvatars_(const MyUtil::IntSeq& ids, int onlineType, AvatarMap &ans);

  bool getUsersJids_(const MyUtil::IntSeq& ids, int onlineType, JidSeqMap &ans);
  bool getUserJids_(int userId, int onlineType, JidSeq &ans);

	void DeleteUser(int userid);
	void DeleteUserJid(const JidPtr &jid);
  void StatEndpoint(const char *fun, const JidPtr &jid);
  void heartInfo(const JidSeq& jidseq, const Ice::Current& current);
  void get3GAvatars(const MyUtil::IntSeq& uids, AvatarSeq &ans);
  void get3GAvatarNs(const MyUtil::IntSeq& uids, AvatarNSeq &ans);
  void get3GJids(const MyUtil::IntSeq& uids, JidSeq &ans);
};
	

};
};
};
};
};

#endif

