#include "MemcOnlineLogicAdapter.h"
#include "IceLogger.h"

using namespace com::xiaonei::talk::memc::online;
using namespace MyUtil;


MemcOnlineLogicManagerPrx MemcOnlineLogicAdapter::getManager(int id) {
  return locate<MemcOnlineLogicManagerPrx>(_managers, "M", id, TWO_WAY);
}

MemcOnlineLogicManagerPrx MemcOnlineLogicAdapter::getManagerOneway(int id) {
	return locate<MemcOnlineLogicManagerPrx>(_managersOneway, "M", id, ONE_WAY);
}

MemcOnlineLogicManagerPrx MemcOnlineLogicAdapter::getManagerUdp(int id){
	return locate<MemcOnlineLogicManagerPrx>(_udpmanagers, "M", id, DATAGRAM);
}



AvatarNMap MemcOnlineLogicAdapter::GetUsersOnlineInfo(const MyUtil::IntSeq& uids){
	if(uids.empty()){
		return AvatarNMap();
	}
	return getManager(uids.size())->GetUsersOnlineInfo(uids);
}
AvatarNSeq MemcOnlineLogicAdapter::GetUserOnlineInfo(int userid){
	return getManager(userid)->GetUserOnlineInfo(userid);
}
int MemcOnlineLogicAdapter::GetUserOnlineType(int userid){
	return getManager(userid)->GetUserOnlineType(userid);
}
int MemcOnlineLogicAdapter::GetUserOnlineStat(int userid){
	return getManager(userid)->GetUserOnlineStat(userid);
}
void MemcOnlineLogicAdapter::SetUserOnlineInfo(int userid, const AvatarNSeq& seq){
	getManagerOneway(userid)->SetUserOnlineInfo(userid, seq);
}

/*
void MemcOnlineLogicAdapter::AvatarOnline(const JidPtr& jid, const string& ver, const string& ext){
	getManagerOneway(jid->userId)->AvatarOnline(jid, ver, ext);
}
*/

void MemcOnlineLogicAdapter::UDP_SetUserOnlineInfo(int userid, const AvatarNSeq& seq){
	getManagerUdp(userid)->SetUserOnlineInfo(userid, seq);
}



void MemcOnlineLogicAdapter::AvatarNOnline(const AvatarNPtr& avatarn){
	getManagerOneway(avatarn->jid->userId)->AvatarNOnline(avatarn);
}
void MemcOnlineLogicAdapter::AvatarNOffline(const JidPtr& jid){
	if(jid){
		getManagerOneway(jid->userId)->AvatarNOffline(jid);
	}
}


