#ifndef TRIPOD_SINGLETON_H_
#define TRIPOD_SINGLETON_H_

#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

namespace xce {
namespace tripod {

template<class T>
class Singleton {
 public:
  static T& GetInstance() {
    if( instance_ == NULL ) {
      boost::lock_guard<boost::mutex> lock(mutex_);
      if( instance_ == NULL ) {
        instance_ = new T;
      }
    }
    return *instance_;
  }

 protected:
  Singleton() { }
  ~Singleton() {
   if(instance_) {
     T* t = instance_;
     instance_ = NULL;
     delete t;
   }
  }
 private:
  static T* instance_;
  static boost::mutex mutex_;
};

template<class T> T* Singleton<T>::instance_ = NULL;
template<class T> boost::mutex Singleton<T>::mutex_;

}} // xce::tripod

#endif
