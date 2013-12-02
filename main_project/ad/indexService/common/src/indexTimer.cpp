/*
 * =====================================================================================
 *
 *       Filename:  IndexTimer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月26日 15时30分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "indexTimer.h"
#include "IceLogger.h"

void IndexTimer::setInstance(void *arg)
{
	instance_ = arg;
}

void IndexTimer::setLastProcessTime(const timeval &tv)
{
	last_time_ = tv;
}

timeval IndexTimer::getLastProcessTime()
{
	return last_time_;
}


void IndexTimerManager::add(IndexTimerPtr t)
{
	IceUtil::RWRecMutex::WLock wlock(lock_);
	timers_.insert(t);
}

void IndexTimerManager::run()
{

	struct timeval tv;
	struct timeval elapse;

	while(true)
	{

		{
			IceUtil::RWRecMutex::RLock wlock(lock_);
			if(timers_.empty())
			{
				MCE_WARN("IndexTimerManager has no timer");
				return;
			}

			tv.tv_sec = (*(timers_.begin()))->interval();
			tv.tv_usec = 0;
		}

		while(select(0,NULL,NULL,NULL,&tv) < 0 && errno == EINTR);

		gettimeofday(&elapse, 0);

		for(std::multiset <IndexTimerPtr>::iterator it = timers_.begin(); it != timers_.end(); ++it)
		{

			IndexTimerPtr timer = *it;
			if(!timer)
			{
				MCE_WARN("IndexTimerManager::run get a NULL timer");
				return;
			}

			if(elapse.tv_sec - timer->getLastProcessTime().tv_sec >= timer->interval())
			{
				timer->process();

				gettimeofday(&elapse, 0); 
				timer->setLastProcessTime(elapse);
			}
		}

	

	}

}
