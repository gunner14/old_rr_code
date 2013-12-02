#include "TalkFunStateAdapter.h"
#include "IceLogger.h"

using namespace com::xiaonei::talk::fun::state;
using namespace MyUtil;


TalkFunStateManagerPrx TalkFunStateAdapter::getManager(int id) {
  //return locate<TalkFunStateManagerPrx>(_managers, "M", id, TWO_WAY);
	return getProxy(id);
}

TalkFunStateManagerPrx TalkFunStateAdapter::getManagerOneway(int id) {
	//return locate<TalkFunStateManagerPrx>(_managersOneway, "M", id, ONE_WAY);
	return getProxyOneway(id);
}

TalkFunStateManagerPrx TalkFunStateAdapter::getManagerUdp(int id){
	//return locate<TalkFunStateManagerPrx>(_udpmanagers, "M", id, DATAGRAM);
}

void TalkFunStateAdapter::HeartBeat(const JidSeq& jids){
	if(jids.empty()){
		return;
	}
	getManagerOneway(jids.at(0)->userId)->HeartBeat(jids);
}

void TalkFunStateAdapter::HeartBeatNotSplit(const JidSeq& jids){
	if(jids.empty()){
		return;
	}
	getManagerOneway(jids.at(0)->userId)->HeartBeatNotSplit(jids);
}




AvatarNSeqMap TalkFunStateAdapter::GetUsersOnlineInfo(const MyUtil::IntSeq& uids, bool checkheartbeat){
	if(uids.empty()){
		return AvatarNSeqMap();
	}
	return getManager(uids.size())->GetUsersOnlineInfo(uids, checkheartbeat);
}
AvatarNSeq TalkFunStateAdapter::GetUserOnlineInfo(int userid, bool checkheartbeat){
	return getManager(userid)->GetUserOnlineInfo(userid, checkheartbeat);
}
int TalkFunStateAdapter::GetUserOnlineType(int userid){
	return getManager(userid)->GetUserOnlineType(userid);
}
int TalkFunStateAdapter::GetUserOnlineStat(int userid){
	return getManager(userid)->GetUserOnlineStat(userid);
}
void TalkFunStateAdapter::SetUserOnlineInfo(int userid, const AvatarNSeq& seq){
	getManagerOneway(userid)->SetUserOnlineInfo(userid, seq);
}

void TalkFunStateAdapter::AvatarNOnline(const AvatarNPtr& avatarn){
	getManagerOneway(avatarn->jid->userId)->AvatarNOnline(avatarn);
}
void TalkFunStateAdapter::AvatarNOffline(const JidPtr& jid){
	if(jid){
		getManagerOneway(jid->userId)->AvatarNOffline(jid);
	}
}

//UserOnlineTypeMap TalkFunStateAdapter::getOnlineUsers(const MyUtil::IntSeq& uids) {
UserOnlineTypeSeq TalkFunStateAdapter::getOnlineUsers(const MyUtil::IntSeq& uids) {
  return getManager(uids.at(0))->getOnlineUsers(uids);
}

JidSeqMap TalkFunStateAdapter::getUsersJids(const MyUtil::IntSeq& ids, int onlineType){
	if(ids.empty()){
		return JidSeqMap();
	}
	return getManager(ids.at(0))->getUsersJids(ids, onlineType);
}
JidSeq TalkFunStateAdapter::getUserJids(int userId, int onlineType){
	return getManager(userId)->getUserJids(userId, onlineType);
}
AvatarPtr TalkFunStateAdapter::getAvatar(const JidPtr& jid){
	if(!jid){
		return NULL;
	}
	return getManager(jid->userId)->getAvatar(jid);
}
AvatarSeq TalkFunStateAdapter::getAvatars(const JidSeq& jids){
	if(jids.empty()){
		return AvatarSeq();
	}
	return getManager(jids.at(0)->userId)->getAvatars(jids);
}
AvatarSeq TalkFunStateAdapter::getUserAvatars(int userId, int onlineType){
	return getManager(userId)->getUserAvatars(userId, onlineType);
}
AvatarMap TalkFunStateAdapter::getUsersAvatars(const MyUtil::IntSeq& ids, int onlineType){
	if(ids.empty()){
		return AvatarMap();
	}
	return getManager(ids.at(0))->getUsersAvatars(ids, onlineType);
}
JidSeqMap TalkFunStateAdapter::getBuddyJidsAndSelf(int userId, int buddyOnlineType, int selfOnlineType){
	return getManager(userId)->getBuddyJidsAndSelf(userId, buddyOnlineType, selfOnlineType);
}
JidSeqMap TalkFunStateAdapter::getBuddyJids(int userId, int onlineType){
	return getManager(userId)->getBuddyJids(userId, onlineType);
}
AvatarMap TalkFunStateAdapter::getBuddyAvatars(int userId, int onlineType){
	return getManager(userId)->getBuddyAvatars(userId, onlineType);;
}
AvatarMap TalkFunStateAdapter::getBuddyAvatarsAndSelf(int userId, int buddyOnlineType, int selfOnlineType){
	return getManager(userId)->getBuddyAvatarsAndSelf(userId, buddyOnlineType, selfOnlineType);
}







