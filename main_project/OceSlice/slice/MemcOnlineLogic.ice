#ifndef _MEMC_ONLINE_LOGIC_
#define _MEMC_ONLINE_LOGIC_

#include <TalkCommon.ice>

module com
{
module xiaonei
{
module talk
{
module memc
{
module online
{
	class MemcOnlineLogicManager{
		com::xiaonei::talk::common::AvatarNSeqMap GetUsersOnlineInfo(MyUtil::IntSeq uids);
		com::xiaonei::talk::common::AvatarNSeq GetUserOnlineInfo(int userid);
		int GetUserOnlineType(int userid);
		//MyUtil::IntSeq GetUsersOnlineType(MyUtil::IntSeq uids);
		int GetUserOnlineStat(int userid);
		//MyUtil::IntSeq GetUsersOnlineStat(MyUtil::IntSeq uids);
		void SetUserOnlineInfo(int userid, com::xiaonei::talk::common::AvatarNSeq seq);
		void AvatarNOnline(com::xiaonei::talk::common::AvatarN avatarn);
		void AvatarNOffline(com::xiaonei::talk::common::Jid jid);
		
		void ClearUserOnlineInfo(int userid);
		//void Deliver(com::xiaonei::talk::common::Jid from, int touserid, com::xiaonei);
	};
};
};
};
};
};

#endif
