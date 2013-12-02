#include "AdMobLogMaker.h"
#include <IceUtil/Mutex.h>

using namespace xce::ad;

void AdMobLogMaker::FlushLogTask::handle(){
  try {
    while ( true ) {
	  parent_->Execute();
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("AdMobLogMaker::FlushLogTask::handle --> handle Execute err:" << e );
  } catch (...) {
    MCE_WARN("AdMobLogMaker::FlushLogTask::handle --> handle Execute ");
  }
}

void AdMobLogMaker::PushBack( const string& log ) {  
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  queue_.push( log );
  if ( queue_.size() >= buffer_size_ ) {
	notify();
  }
}

void AdMobLogMaker::PushBack( const StrSeq& logs ) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  for (StrSeq::const_iterator it = logs.begin(); it != logs.end(); ++it) {
    queue_.push(*it);
  }
  if ( queue_.size() >= buffer_size_ ) {
	notify();
  }
}

void AdMobLogMaker::Execute() {
	ostringstream log;
	{
		IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
		MCE_DEBUG("AdMobLogMaker::Execute --> queue size = " << queue_.size());
		if ( queue_.empty() || queue_.size() < buffer_size_ ) {
			IceUtil::Time time = IceUtil::Time::seconds( WAIT_SECONDS );
			timedWait(time);
		}

		if ( queue_.empty() ) {
			LOG_INFO( name_, "Empty" );
		} else {
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
			MCE_DEBUG("AdMobLogMaker::Execute --> queue size = " << queue_.size() << ", this time count = " << count);
			LOG_INFO( name_, log.str() );
		}
	}
}

