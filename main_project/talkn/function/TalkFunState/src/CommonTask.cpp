#include "CommonTask.h"
#include "TalkFunStateManagerI.h"
#include "XmppTools.h"
using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::talk::util;



void CheckHeartBeatTask::handle(){
	for(AvatarNSeqMap::iterator it = avatarn_map_.begin(); it != avatarn_map_.end(); ++it){
		int userid = it->first;
		int now = time(NULL);
		for(AvatarNSeq::iterator sit = it->second.begin(); sit != it->second.end(); ++sit){
			if(now - (*sit)->hearttimestamp >= GAMEOVERTIME){
				MCE_DEBUG("CheckHeartBeatTask::handle--> heart beat check jid:" << jidToString((*sit)->jid) << " now:" << now << " timestamp:" << (*sit)->hearttimestamp << " GAMEOVER:" << GAMEOVERTIME);
				TalkFunStateManagerI::instance().AvatarNOffline((*sit)->jid);
			}
		}
	}
}


