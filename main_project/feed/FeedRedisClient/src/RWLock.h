#ifndef  __XCE_FEED_RWLOCK__
#define __XCE_FEED_RWLOCK__

#include <pthread.h>
#include <errno.h>
#include "iostream"
#include "LogWrapper.h"

namespace xce {
namespace feed {

class WriteLock {
public:
	WriteLock(pthread_rwlock_t* rwlock):rwlock_(rwlock) {
		if (!rwlock_) {
#ifdef STDOUT
			std::cout << "WriteLock::WriteLock --> rwlock is NULL!" << std::endl;
#else
			MCE_WARN("WriteLock::WriteLock --> rwlock is NULL!");
#endif

			pthread_exit(NULL);
		}

		if (int err = pthread_rwlock_wrlock(rwlock_)) {
#ifdef STDOUT
			std::cout << "WriteLock::WriteLock --> pthread_rwlock_wrlock error:" << err << std::endl;	
#else
			MCE_WARN("WriteLock::WriteLock --> pthread_rwlock_wrlock error:" << err << " " << strerror(err));	
#endif
			pthread_exit(NULL);
		}
	}

	~WriteLock() {
		if (int err = pthread_rwlock_unlock(rwlock_)) {
#ifdef STDOUT
			std::cout << "WriteLock::~WriteLock --> pthread_rwlock_unlock error:" << err << std::endl;	
#else
			MCE_WARN("WriteLock::~WriteLock --> pthread_rwlock_unlock error:" << err << " " << strerror(err));	
#endif
			pthread_exit(NULL);
		}
	}

private:
	pthread_rwlock_t* rwlock_;

};

class ReadLock {
public:
	ReadLock(pthread_rwlock_t* rwlock):rwlock_(rwlock) {

		if (!rwlock_) {
#ifdef STDOUT
			std::cout << "WriteLock::ReadLock --> rwlock is NULL!" << std::endl;
#else
			MCE_WARN("WriteLock::ReadLock --> rwlock is NULL!");
#endif
			pthread_exit(NULL);

		}

		if (int err = pthread_rwlock_rdlock(rwlock_)) {
#ifdef STDOUT
			cout << "WriteLock::ReadLock --> pthread_rwlock_rdlock error:" << err << endl;	
#else
			MCE_WARN("WriteLock::ReadLock --> pthread_rwlock_rdlock error:" << err << " " << strerror(err));	
#endif
			pthread_exit(NULL);
		}
	}

	~ReadLock() {
		if (int err = pthread_rwlock_unlock(rwlock_)) {
#ifdef STDOUT
			cout << "WriteLock::ReadLock --> pthread_rwlock_unlock error:" << err << endl;	
#else
			MCE_WARN("WriteLock::ReadLock --> pthread_rwlock_unlock error:" << err << " " << strerror(err));	
#endif
			pthread_exit(NULL);
		}
	}

private:
	mutable pthread_rwlock_t* rwlock_;

};

}}
#endif
