#include "gtest/gtest.h"
#include <iostream>
#include "../UserLogicAdapter.h"

using namespace mop::hi::svc::adapter;

namespace xce {
namespace adapter {

/*
TEST(UserLogicTest, insertUserBorn) {
  try {
    UserLogicAdapter::instance().insertUserBorn(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserBorn) {
  try {
    UserLogicAdapter::instance().updateUserBorn(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserDoing) {
  try {
    UserLogicAdapter::instance().insertUserDoing(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserDoing) {
  try {
    UserLogicAdapter::instance().updateUserDoing(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserLoginCount) {
  try {
    UserLogicAdapter::instance().insertUserLoginCount(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserLoginCount) {
  try {
    UserLogicAdapter::instance().updateUserLoginCount(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserNames) {
  try {
    UserLogicAdapter::instance().insertUserNames(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserNames) {
  try {
    UserLogicAdapter::instance().updateUserNames(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserPassport) {
  try {
    UserLogicAdapter::instance().insertUserPassport(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserPassport) {
  try {
    UserLogicAdapter::instance().updateUserPassport(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserRight) {
  try {
    UserLogicAdapter::instance().insertUserRight(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserRight) {
  try {
    UserLogicAdapter::instance().updateUserRight(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserStage) {
  try {
    UserLogicAdapter::instance().insertUserStage(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserStage) {
  try {
    UserLogicAdapter::instance().updateUserStage(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserState) {
  try {
    UserLogicAdapter::instance().insertUserState(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserState) {
  try {
    UserLogicAdapter::instance().updateUserState(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, insertUserUrl) {
  try {
    UserLogicAdapter::instance().insertUserUrl(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserUrl) {
  try {
    UserLogicAdapter::instance().updateUserUrl(userId_, props_, ctx_);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserLogicTest, updateUserConfig) {

	Str2StrMap props;
	Str2StrMap ctx;

	for(int userId = 10000; userId < 20000; ++userId) {

		try {
			UserLogicAdapter::instance().updateUserConfig(userId, props, ctx);
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		usleep(1000);
	}
}


TEST(UserLogicTest, insertUserConfig) {
	Str2StrMap props;
	Str2StrMap ctx;
	for(int userId = 20000; userId < 30000; ++userId) {
		try {
			UserLogicAdapter::instance().insertUserConfig(userId, props, ctx);
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		usleep(1000);
	}
}
*/


TEST(UserLogicTest, updateUserConfig) {

	Str2StrMap props;
	Str2StrMap ctx;


		try {
			//UserLogicAdapter::instance().updateUserNetwork(35178, props);
			//UserLogicAdapter::instance().updateUserRegion(35178, props);
			//UserLogicAdapter::instance().updateUserWorkplace(35178, props);
			//UserLogicAdapter::instance().updateUserElementarySchool(1115, props);
			//UserLogicAdapter::instance().updateUserJuniorHighSchool(1011, props);
			UserLogicAdapter::instance().updateUserHighSchool(373420895, props);
			UserLogicAdapter::instance().updateUserHighSchool(373420895, props);
			UserLogicAdapter::instance().updateUserHighSchool(373420895, props);
			//UserLogicAdapter::instance().updateUserCollege(247322621, props);
			//UserLogicAdapter::instance().updateUserUniversity(35178, props);
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
}





}} //xce::user::adapter


