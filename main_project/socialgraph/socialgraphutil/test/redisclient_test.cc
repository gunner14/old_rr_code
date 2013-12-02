#ifndef REDIS_CLIENT_TEST_H_
#define REDIS_CLIENT_TEST_H_

#include <iostream>

#include <boost/assign/list_of.hpp>

#include "socialgraph/socialgraphutil/redisclient.h"
#include "socialgraph/socialgraphutil/clogging.h"

#include "gtest/gtest.h"

namespace xce {
namespace socialgraph {

class RedisClientTest: public ::testing::Test {
 protected:
	virtual void SetUp() {
		std::string ip = "10.3.17.94";	//sg8
		int port = 6379;
		ptr_ = RedisClientPtr(new RedisClient(ip, port));
	}

	virtual void TearDown() {
	}

	RedisClientPtr ptr_;
};

TEST_F(RedisClientTest, Set) {
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	ptr_->Remove(key);
	EXPECT_TRUE(ptr_->SetList(key, idlist));

	StrList result;
	ptr_->GetList(key, result);
	EXPECT_TRUE(result == idlist);
}

TEST_F(RedisClientTest, SetWithExpire) {
	int expiretime = 2;
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	ptr_->Remove(key);
	ptr_->SetList(key, idlist, expiretime);

	sleep(3);
	StrList result;
	ptr_->GetList(key, result);

	ASSERT_EQ(result.size(), 0);
}

TEST_F(RedisClientTest, GetListSize) {
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	ptr_->Remove(key);
	ptr_->SetList(key, idlist);

	int length = ptr_->GetListSize(key);
	ASSERT_EQ(length, 3);
}

TEST_F(RedisClientTest, Remove) {
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	ptr_->SetList(key, idlist);

	ptr_->Remove(key);
	StrList result;
	ptr_->GetList(key, result);

	ASSERT_EQ(result.size(), 0);
}

}}

#endif
