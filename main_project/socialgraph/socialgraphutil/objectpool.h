#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include <deque>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/mutex.hpp>

#include "socialgraph/socialgraphutil/objectfactory.h"

namespace xce {
namespace socialgraph {

/*@brief Common Object pool*/
template <class T>
class ObjectPool {
 public:
	ObjectPool(size_t core_size = 10, size_t max_size = 100) { 
		core_size_ = core_size;	
		max_size_ = max_size;
		used_ = 0;
		pfactory_ = 0;
	}

	virtual ~ObjectPool() {
		if (0 != pfactory_) {
			delete pfactory_;
		}
		pfactory_ = 0;
	}

	/*@brief Borrow pointer of object in the pool*/
	boost::shared_ptr<T> BorrowObject() {
		boost::shared_ptr<T> ptr = boost::shared_ptr<T>();		//NULL
		bool add = false;
		{
			boost::mutex::scoped_lock lock(mutex_);
			if (obj_deque_.empty() && (used_ < max_size_)) {
				add = true;	
			} 
			else {
				ptr = obj_deque_.front();
				obj_deque_.pop_front();
			}
		}
		if (add) {
			ptr = AddObject();
		}
		if (ptr != NULL) {
			boost::mutex::scoped_lock lock(mutex_);
			used_++;
		}
		return ptr;
	}

	/*@brief return the object*/
	void ReturnObject(boost::shared_ptr<T> obj) {
		boost::mutex::scoped_lock lock(mutex_);
		obj_deque_.push_back(obj);
		used_--;
	}

	/*@brief Set factory which create object*/
	void SetFactory(ObjectFactory<T> *pfactory) {
		pfactory_ = pfactory;
	}

	/*@brief clear pool of object*/
	void Clear() {
		boost::mutex::scoped_lock lock(mutex_);
		if (!obj_deque_.empty()) {
			obj_deque_.clear();
		}
	}

	/*@brief detach object from deque*/
	void Detach() {
		boost::mutex::scoped_lock lock(mutex_);
		used_--;
	}

	/*@brief get number of object used*/
	int GetNumActive() {
		return used_;
	}

	/*@brief get number of object remained in the pool*/
	int GetNumIdle() {
		boost::mutex::scoped_lock lock(mutex_);
		return obj_deque_.size();
	}

 private:
	boost::shared_ptr<T> AddObject() {
		if (NULL == pfactory_) {
			return boost::shared_ptr<T>();
		}
		return pfactory_->Create();
	}

 private:
	size_t core_size_;
	size_t max_size_;			// max size of pool
	size_t used_;			// number of object used
	ObjectFactory<T> *pfactory_;		// factory used to create object
	std::deque< boost::shared_ptr<T> > obj_deque_;		// pool
	boost::mutex mutex_;
};

}}

#endif
