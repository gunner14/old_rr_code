#ifndef COMMONTASK_H_
#define COMMONTASK_H_

#include "TalkCommon.h"
#include "Singleton.h"
#include "TaskManager.h"
#include "TalkFunStateManagerI.h"

namespace com{
namespace xiaonei{
namespace talk{
namespace fun{
namespace state{

using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::talk::common;
using namespace MyUtil;


const int GAMEOVERTIME = 600;


class CheckHeartBeatTask : virtual public MyUtil::Task{
public:
	CheckHeartBeatTask(const AvatarNSeqMap m):Task( m.size() % 11 ){
		avatarn_map_ = m;
	};
	CheckHeartBeatTask(const AvatarNSeq n):Task( n.size() % 11 ){
		if(!n.empty()){
			int userid = n.at(0)->jid->userId;
			avatarn_map_[userid] = n;
		}
	};
	virtual void handle();
private:
	AvatarNSeqMap avatarn_map_;

};

class DoingTimer : public MyUtil::Timer{
public:
	DoingTimer() : Timer(13,500){
	}
	virtual void handle(){
		TalkFunStateManagerI::instance().DealDoingCache();
	};
};

class DoingThread : public IceUtil::Thread ,virtual public Singleton<DoingThread>{
public:
	DoingThread(){
		start(65535).detach();
	}
	virtual void run(){
		for(;;){
			for(int index = 0; index < DOINGPOOLSIZE; ++index){
				TalkFunStateManagerI::instance().DealDoingCache(index);	
				usleep(100 * 1000);
			}
		}
	}
};


};
};
};
};
};


#endif
       
