#ifndef TALKFUNSTATEADAPTER_H_ 
#define TALKFUNSTATEADAPTER_H_ 

#include "AdapterI.h"
#include "TalkFunState.h"

#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace fun{
namespace state{
using namespace MyUtil;
using namespace com::xiaonei::talk::common;

class TalkFunStateAdapter : public MyUtil::ReplicatedClusterAdapterI<TalkFunStateManagerPrx>, public MyUtil::Singleton<TalkFunStateAdapter> {
public:
  TalkFunStateAdapter() : MyUtil::ReplicatedClusterAdapterI<TalkFunStateManagerPrx> ("ControllerTalkFunState",120,300,new XceFeedControllerChannel) {
		//_managersOneway.resize(cluster() > 0 ? cluster() : 1);
		//_managers.resize(cluster() > 0 ? cluster() : 1);
	}

	void HeartBeat(const JidSeq& jids);
	void HeartBeatNotSplit(const JidSeq& jids);
	
	AvatarNSeqMap GetUsersOnlineInfo(const MyUtil::IntSeq& uids, bool checkheartbeat = true);
	AvatarNSeq GetUserOnlineInfo(int userid, bool checkheartbeat = true);
	
	int GetUserOnlineType(int userid);
	int GetUserOnlineStat(int userid);
	void SetUserOnlineInfo(int userid, const AvatarNSeq& seq);
	void AvatarNOnline(const AvatarNPtr& avatarn);
	void AvatarNOffline(const JidPtr& jid);

	//兼容老接口
	JidSeqMap getUsersJids(const MyUtil::IntSeq& ids, int onlineType = 14);
	JidSeq getUserJids(int userId, int onlineType = 14);
	AvatarPtr getAvatar(const JidPtr& jid);
	AvatarSeq getAvatars(const JidSeq& jids);
	AvatarSeq getUserAvatars(int userId, int onlineType = 14);
	AvatarMap getUsersAvatars(const MyUtil::IntSeq& ids, int onlineType = 14);
	JidSeqMap getBuddyJidsAndSelf(int userId, int buddyOnlineType = 14, int selfOnlineType = 14);
	JidSeqMap getBuddyJids(int userId, int onlineType = 14);
	AvatarMap getBuddyAvatars(int userId, int onlineType = 14);
	AvatarMap getBuddyAvatarsAndSelf(int userId, int buddyOnlineType = 14, int selfOnlineType = 14);
  //UserOnlineTypeMap getOnlineUsers(const MyUtil::IntSeq& uids);
  UserOnlineTypeSeq getOnlineUsers(const MyUtil::IntSeq& uids);


	size_t Cluster(){
		return getCluster();
	}



protected:
	virtual string name() {
		return "TalkFunState";
	}
	virtual string endpoints() {
		return "@TalkFunState";
	}
	;
	virtual size_t cluster() {
		return getCluster();
	}
	
	TalkFunStateManagerPrx getManagerOneway(int id);
	TalkFunStateManagerPrx getManager(int id);
	TalkFunStateManagerPrx getManagerUdp(int id);

	vector<TalkFunStateManagerPrx> _managersOneway;
	vector<TalkFunStateManagerPrx> _managers;
	vector<TalkFunStateManagerPrx> _udpmanagers;
};

}
;
}
;
}
;
};
};

#endif /*TALKCACHEADAPTER_H_*/
