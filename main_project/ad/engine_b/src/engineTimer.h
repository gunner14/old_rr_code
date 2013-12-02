#ifndef __ENGINE_TIMER_H__
#define __ENGINE_TIMER_H__

#include <set>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "Singleton.h"

class EngineTimer : public IceUtil::Shared{
public:
		EngineTimer(void *instance = NULL) :instance_(instance){
			gettimeofday(&last_time_, 0);
		}
		virtual void process() = 0;
		virtual int interval() const = 0;

		void setInstance(void *);
		timeval getLastProcessTime();
		void setLastProcessTime(const timeval &tv);

		bool operator >(const EngineTimer& t) const{
			return interval() > t.interval();
		}
		bool operator <(const EngineTimer& t) const{
			return interval() < t.interval();
		}
		bool operator ==(const EngineTimer& t) const{
			return interval() == t.interval();
		}
	protected:
		void *instance_;
		timeval last_time_;
};


typedef IceUtil::Handle<EngineTimer> EngineTimerPtr;

class EngineTimerManager : public IceUtil::Thread, public MyUtil::Singleton<EngineTimerManager>
{
	public:
		void add(EngineTimerPtr);
		virtual void run();
	public:
		IceUtil::RWRecMutex lock_;
		std::multiset <EngineTimerPtr> timers_;
};

#endif
