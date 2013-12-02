/*
 * FeedLogger.cpp
 *
 *  Created on: 2012-11-13
 *      Author:rui.qiu
 */

#include <IceUtil/Mutex.h>
#include "FeedLogger.h"

using namespace xce::feed;

void FeedLogger::FlushLogTask::handle(){
  try {
    while ( true ) {
	    parent_->Execute();
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedLogger::FlushLogTask::handle --> logger:" << parent_->name() << " handle Execute err:" << e );
  } catch (...) {
    MCE_WARN("FeedLogger::FlushLogTask::handle --> logger:" << parent_->name()<< " handle Execute ");
  }
}

void FeedLogger::PushBack( const string& log ) {  
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  queue_.push( log );
  if ( queue_.size() >= buffer_size_ ) { // local buffer is full
	  notify();
  }
}

void FeedLogger::PushBack( const StrSeq& logs ) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  for (StrSeq::const_iterator it = logs.begin(); it != logs.end(); ++it) {
    queue_.push(*it);
  }
  if ( queue_.size() >= buffer_size_ ) {
	  notify();
  }
}

void FeedLogger::Execute() {
	ostringstream log;
	{
		IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
		if ( queue_.empty() || queue_.size() < buffer_size_ ) {
			IceUtil::Time time = IceUtil::Time::seconds( wait_seconds_ );
			timedWait(time);
		}
		if (!queue_.empty() ) {
			int count = 0;
			size_t queue_size = queue_.size();
			for (size_t i = 0; i < queue_size; i++) {
				log << queue_.front();
				queue_.pop();
				count++;
				if (queue_.empty()) {
					break;
				}
				if (i < queue_size - 1) {
					log << "\n";
				}
			}
			LOG_INFO( name_, log.str() );
		} else {
      LOG_INFO(name_, "Empty");
    }
	  usleep(10);
  }
}

void FeedLogger::Flush() {
  ostringstream log;
	int count = 0;
	size_t queue_size = queue_.size();
	for (size_t i = 0; i < queue_size; i++) {
	  log << queue_.front();
	  queue_.pop();
	  count++;
	  if (queue_.empty()) {
		  break;
	  }
	  if (i < queue_size - 1) {
		  log << "\n";
	  }
  }
	LOG_INFO( name_, log.str() );
}

