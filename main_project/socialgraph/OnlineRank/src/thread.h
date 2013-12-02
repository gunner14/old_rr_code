#ifndef THREAD_H_
#define THREAD_H_
#include <stdio.h>

#include <pthread.h>

class Thread{
	public:
		static void* wrapper(void * ptr){
			Thread* p=static_cast<Thread*>(ptr);
			p->run();
			return 0;
		}
		virtual void run(){}
		void start(){
			pthread_create(&hnd, 0, &Thread::wrapper, static_cast<void*>(this) );
		}
		void join(){
			pthread_join(hnd, 0);
		}
		virtual ~Thread(){}

	private:
		pthread_t hnd;
};

#endif
