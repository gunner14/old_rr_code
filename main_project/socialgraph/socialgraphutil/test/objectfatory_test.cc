#ifndef OBJECT_FACTORY_TEST_H_
#define OBJECT_FACTORY_TEST_H_

#include <iostream>

#include "gtest/gtest.h"

#include "socialgraph/socialgraphutil/objectfactory.h"

namespace xce {
namespace socialgraph {

struct TT {
	TT(int a, int b, int c) {
		a_ = a;
		b_ = b;
		c_ = c;		
	}

	int a_;
	int b_;
	int c_;
};

typedef boost::shared_ptr<TT> TTPtr;

TEST(ObjectFactory, ThreeArgsFactory) {
	ObjectFactory<TT> *f = new ThreeArgsFactory<TT, int, int, int>(3, 5, 6);
	TTPtr p = f->Create();	

	ASSERT_EQ(p->a_, 3);
	ASSERT_EQ(p->b_, 5);
	ASSERT_EQ(p->c_, 6);
	delete f;
}

}
}

#endif
