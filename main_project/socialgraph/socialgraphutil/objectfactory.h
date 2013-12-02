#ifndef OBJECT_FACTORY_H_
#define OBJECT_FACTORY_H_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace xce {
namespace socialgraph {

/*@brief interface of ObjectFactory*/
template <typename T>
class ObjectFactory {
 public:
	virtual ~ObjectFactory() {}

	/*@brief inteface of creating object*/
	virtual boost::shared_ptr<T> Create() const = 0;
};

//--------------------------------------

/*@brief initializion with no args*/
template <typename T>
class DefaultArgsFactory: public ObjectFactory<T> {
 public:
	DefaultArgsFactory() { }

	boost::shared_ptr<T> Create() const {
		return boost::make_shared<T>();
	}
};

//--------------------------------------

/*@brief initializion with one args*/
template <typename T, typename A>
class OneArgsFactory: public ObjectFactory<T> {
 public:
	OneArgsFactory(const A& a) {
		a_ = a;
	}

	boost::shared_ptr<T> Create() const {
		return boost::make_shared<T>(a_);
	}

 private:
	A a_;
};

//--------------------------------------

/*@brief initializion with two args*/
template <typename T, typename A, typename B>
class TwoArgsFactory: public ObjectFactory<T> {
 public:
	TwoArgsFactory(const A& a, const B& b) {
		a_ = a;
		b_ = b;
	}

	boost::shared_ptr<T> Create() const {
		return boost::make_shared<T>(a_, b_);
	}

 private:
	A a_;
	B b_;
};

//--------------------------------------

/*@brief initializion with three args*/
template <typename T, typename A, typename B, typename C>
class ThreeArgsFactory: public ObjectFactory<T> {
 public:
	ThreeArgsFactory(const A& a, const B& b, const C& c) {
		a_ = a;
		b_ = b;
		c_ = c;
	}

	boost::shared_ptr<T> Create() const {
		return boost::make_shared<T>(a_, b_, c_);
	}

 private:
	A a_;
	B b_;
	C c_;
};

}}

#endif
