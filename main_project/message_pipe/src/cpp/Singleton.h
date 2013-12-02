#ifndef _MP_SINGLETON_H__
#define _MP_SINGLETON_H__

#include <boost/thread/locks.hpp>
#include <boost/thread/pthread/shared_mutex.hpp>

namespace xce{
namespace messagepipe{

template<class T>
    class Singleton {
        public:
            static T* getInstance() {
                if( instance__ == NULL ) {
                    boost::unique_lock<boost::shared_mutex> lock(lock__);
                    if( instance__ == NULL ) {
                        instance__ = new T;
                    }
                }
                return instance__;
            }

        protected:
            Singleton(){}
            ~Singleton() {
                if(instance__) {
                    T* t = instance__;
                    instance__ = NULL;
                    free(t);
                }
            }

        private:
            static T* instance__;

            static boost::shared_mutex lock__;
    };

template<class T> T* Singleton<T>::instance__ = NULL;
template<class T> boost::shared_mutex Singleton<T>::lock__;

}
}
#endif
