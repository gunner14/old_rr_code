#include <iostream>

#include "gtest/gtest.h"

#include "socialgraph/friendrecommendmanager/common/id_filter.h"

using namespace xce::socialgraph;

class IdFilterTest : public ::testing::Test {
 public:
	void SetUp() {
		IdFilter::instance().Initialize("./");
	}

	virtual void TearDown() {
	}
};

//------------------------------------------------

TEST_F(IdFilterTest, IsExist) {
	ASSERT_EQ(true, IdFilter::instance().IsExist("zn_test_blacklist", 897));
	ASSERT_EQ(false, IdFilter::instance().IsExist("zn_test_whitelist", 66666666));
}

TEST_F(IdFilterTest, Reload) {
	IdFilter::instance().Reload("./");
	ASSERT_EQ(true, IdFilter::instance().IsExist("zn_test_blacklist", 897));
	ASSERT_EQ(false, IdFilter::instance().IsExist("zn_test_whitelist", 66666666));
}

TEST_F(IdFilterTest, badfile) {
	std::set<int> tmp = IdFilter::instance().GetIdSet("zn_badtest_whitelist");
	ASSERT_EQ(3, tmp.size());
}
