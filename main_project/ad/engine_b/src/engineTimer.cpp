#include "engineTimer.h"
#include "IceLogger.h"

void EngineTimer::setInstance(void *arg){
	instance_ = arg;
}

void EngineTimer::setLastProcessTime(const timeval &tv){
	last_time_ = tv;
}

timeval EngineTimer::getLastProcessTime(){
	return last_time_;
}

void EngineTimerManager::add(EngineTimerPtr t){
	IceUtil::RWRecMutex::WLock wlock(lock_);
	timers_.insert(t);
}

void EngineTimerManager::run(){
	struct timeval tv;
	struct timeval elapse;
	while(true){
		{
			IceUtil::RWRecMutex::RLock wlock(lock_);
			if(timers_.empty()){
				MCE_WARN("EngineTimerManager has no timer");
				return;
			}
			tv.tv_sec = (*(timers_.begin()))->interval();
			tv.tv_usec = 0;
		}

		while(select(0,NULL,NULL,NULL,&tv) < 0 && errno == EINTR);

		gettimeofday(&elapse, 0);

		for(std::multiset <EngineTimerPtr>::iterator it = timers_.begin(); it != timers_.end(); ++it){
			EngineTimerPtr timer = *it;
			if(!timer){
				MCE_WARN("EngineTimerManager::run get a NULL timer");
				return;
			}

			if(elapse.tv_sec - timer->getLastProcessTime().tv_sec >= timer->interval()){
				timer->process();
				gettimeofday(&elapse, 0); 
				timer->setLastProcessTime(elapse);
			}
		}
	}
}
