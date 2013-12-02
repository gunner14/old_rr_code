#ifndef OBJECT_POOL_TEST_H_
#define OBJECT_POOL_TEST_H_

#include "gtest/gtest.h"

#include "socialgraph/socialgraphutil/objectpool.h"

namespace xce {
namespace socialgraph {

struct Item {
	Item(const std::string& ip, int port, int timeout) {
		ip_ = ip;
		port_ = port;
		timeout_ = timeout;
	}

	std::string ip_;
	int port_;
	int timeout_;
};

typedef boost::shared_ptr<Item> ItemPtr;

class ObjectPoolTest: public ::testing::Test {
 protected:
	virtual void SetUp() {
		std::string ip = "10.3.17.94";
		int port = 6379;
		int timeout = 500;
		pool_ptr_ = new ObjectPool<Item>;
		ObjectFactory<Item> *f = new ThreeArgsFactory<Item, std::string, int, int>(ip, port, timeout);
		pool_ptr_->SetFactory(f);
	}

	virtual void TearDown() {
		if (pool_ptr_) {
			delete pool_ptr_;
		}
	}

	ObjectPool<Item> *pool_ptr_;
};

TEST_F(ObjectPoolTest, BorrowObject) {
	ItemPtr ptr = pool_ptr_->BorrowObject();
	ASSERT_STREQ("10.3.17.94", ptr->ip_.c_str());
	ASSERT_EQ(6379, ptr->port_);
	ASSERT_EQ(500, ptr->timeout_);
}

TEST_F(ObjectPoolTest, GetNumActive) {
	ItemPtr ptr1 = pool_ptr_->BorrowObject();
	ItemPtr ptr2 = pool_ptr_->BorrowObject();
	ItemPtr ptr3 = pool_ptr_->BorrowObject();

	ASSERT_EQ(3, pool_ptr_->GetNumActive());
}

TEST_F(ObjectPoolTest, GetNumIdle) {
	ItemPtr ptr1 = pool_ptr_->BorrowObject();
	ItemPtr ptr2 = pool_ptr_->BorrowObject();
	ItemPtr ptr3 = pool_ptr_->BorrowObject();
	pool_ptr_->ReturnObject(ptr1);
	pool_ptr_->ReturnObject(ptr2);
	pool_ptr_->ReturnObject(ptr3);

	ASSERT_EQ(3, pool_ptr_->GetNumIdle());
}

TEST_F(ObjectPoolTest, Clear) {
	ItemPtr ptr1 = pool_ptr_->BorrowObject();
	ItemPtr ptr2 = pool_ptr_->BorrowObject();
	ItemPtr ptr3 = pool_ptr_->BorrowObject();

	pool_ptr_->ReturnObject(ptr1);
	pool_ptr_->ReturnObject(ptr2);
	pool_ptr_->ReturnObject(ptr3);

	pool_ptr_->Clear();
	ASSERT_EQ(0, pool_ptr_->GetNumIdle());
}

}}

#endif
