#ifndef REDIS_ADAPTER_TEST_H_
#define REDIS_ADAPTER_TEST_H_

#include <boost/assign/list_of.hpp>

#include "gtest/gtest.h"

#include "socialgraph/socialgraphutil/redis_adapter.h"

namespace xce {
namespace socialgraph {

class RedisAdapterTest: public ::testing::Test {
 protected:
	virtual void SetUp() {
		//std::string ip = "10.3.17.94";
		std::string ip = "10.3.16.85";
		int port = 6379;
		int timeout = 5000;
		int core_size = 10;
		int max_size = 100;
		RedisAdapter::instance().Init(ip, port, timeout, core_size, max_size);
	}

	virtual void TearDown() {
	}

};

TEST_F(RedisAdapterTest, Set) {
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	RedisAdapter::instance().Remove(key);
	EXPECT_TRUE(RedisAdapter::instance().SetList(key, idlist));

	StrList result;
	RedisAdapter::instance().GetList(key, result);
	EXPECT_TRUE(result == idlist);
}

TEST_F(RedisAdapterTest, SetWithExpire) {
	int expiretime = 2;
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	RedisAdapter::instance().Remove(key);
	RedisAdapter::instance().SetList(key, idlist, expiretime);

	sleep(3);
	StrList result;
	RedisAdapter::instance().GetList(key, result);

	ASSERT_EQ(result.size(), 0);
}

TEST_F(RedisAdapterTest, GetListSize) {
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	RedisAdapter::instance().Remove(key);
	RedisAdapter::instance().SetList(key, idlist);

	int length = RedisAdapter::instance().GetListSize(key);
	ASSERT_EQ(length, 3);
}

TEST_F(RedisAdapterTest, Remove) {
	std::string key = "238";
	StrList idlist = boost::assign::list_of("22")("33")("55");
	RedisAdapter::instance().SetList(key, idlist);

	RedisAdapter::instance().Remove(key);
	StrList result;
	RedisAdapter::instance().GetList(key, result);

	ASSERT_EQ(result.size(), 0);
}

TEST_F(RedisAdapterTest, RemoveAndGetListSize) {
	std::string key = "90";
	RedisAdapter::instance().Remove(key);

	int length = RedisAdapter::instance().GetListSize(key);
	ASSERT_EQ(length, -1);
}

}
}

#endif
