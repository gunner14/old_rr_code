/*
 * =====================================================================================
 *
 *       Filename:  Timer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月26日 19时41分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_TIMER_H__
#define __INDEX_TIMER_H__

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

class IndexTimer : public IceUtil::Shared
{
	public:
		IndexTimer(void *instance = NULL) :instance_(instance)
	{
		gettimeofday(&last_time_, 0);
	}
		virtual void process() = 0;
		virtual int interval() const = 0;

		void setInstance(void *);
		timeval getLastProcessTime();
		void setLastProcessTime(const timeval &tv);

		bool operator >(const IndexTimer& t) const
		{
			return interval() > t.interval();
		}
		bool operator <(const IndexTimer& t) const
		{
			return interval() < t.interval();
		}
		bool operator ==(const IndexTimer& t) const
		{
			return interval() == t.interval();
		}
	protected:
		void *instance_;
		timeval last_time_;

};


typedef IceUtil::Handle<IndexTimer> IndexTimerPtr;

class IndexTimerManager : public IceUtil::Thread, public MyUtil::Singleton<IndexTimerManager>
{
	public:
		void add(IndexTimerPtr);
		virtual void run();
	public:
		IceUtil::RWRecMutex lock_;
		std::multiset <IndexTimerPtr> timers_;
};

#endif
