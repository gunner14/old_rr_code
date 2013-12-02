#include <iostream>

#include "gtest/gtest.h"

#include "SocialGraphCoreLogicAdapter.h"

TEST(SocialGraphCoreLogicTest, initiliaze) {
	using namespace xce::socialgraph;
	
	int userid = 238111132;
	int friendid = 40347688;
	SocialGraphCoreLogicAdapter::instance().ReportBuddyApply(userid, friendid);
	//SocialGraphCoreLogicAdapter::instance().ReportUserLogin(238111132);
  //ASSERT_EQ(3, 3);
}
