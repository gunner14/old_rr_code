#include "gtest/gtest.h"
#include <iostream>
#include "../UserScoreTripodAdapter.h"

namespace xce {
namespace adapter {

TEST(UserScoreTripodAdapter, GetScoreDataN2) {
  int userId = 200308413;
  try {
    int start_id = 200308493;
    for (int i = 0; i < 15; ++i) {
      int id = start_id + i;
      UserScoreTripodAdapter::instance().LoadAll(id);
      ScoreDataN data = UserScoreTripodAdapter::instance().GetScoreDataN(id);
      std::cout << "[id=" << id << ", historyLoginDays=" << data.historyLoginDays << ", continueLoginDays=" << data.continueLoginDays << ", activeDays=" << data.activeDays << ", lastLoginTime=" << data.lastLoginTime << ", awardCount=" << data.awardCount << ", loginType=" << data.loginType << ", score=" << data.score << ", level=" << data.level << ", nextLevelScore=" << data.nextLevelScore << ", vip=" << data.vip << ", sign=" << data.sign << "\n";
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserScoreTripodAdapter, LoadScoreLogin) {
  int userId = 200308413;
  try {
    UserScoreTripodAdapter::instance().LoadScoreLogin(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserScoreTripodAdapter, LoadScore) {
  int userId = 200308413;
  try {
    UserScoreTripodAdapter::instance().LoadScore(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserScoreTripodAdapter, LoadAward) {
  int userId = 200308413;
  try {
    UserScoreTripodAdapter::instance().LoadScoreAwardCount(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserScoreTripodAdapter, LoadSign) {
  int userId = 200308413;
  try {
    UserScoreTripodAdapter::instance().LoadScoreSign(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserScoreTripodAdapter, LoadVip) {
  int userId = 200308413;
  try {
    UserScoreTripodAdapter::instance().LoadScoreVip(userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(UserScoreTripodAdapter, GetScoreDataNMap) {
  int userId = 200308413;
  std::list<long> user_ids;
  int start_id = 35178;
  for (int i = 0; i < 15; ++i) {
    user_ids.push_back(start_id + i);
    UserScoreTripodAdapter::instance().LoadAll(start_id + i);
  }

  try {
    std::map<int, ScoreDataN> data = UserScoreTripodAdapter::instance().GetScoreDataNMap(user_ids);
    for (std::map<int, ScoreDataN>::iterator iter = data.begin(); iter != data.end(); ++iter) {
      std::cout << "[id=" << iter->first << ", historyLoginDays=" << iter->second.historyLoginDays << ", continueLoginDays=" << iter->second.continueLoginDays << ", activeDays=" << iter->second.activeDays << ", lastLoginTime=" << iter->second.lastLoginTime << ", awardCount=" << iter->second.awardCount << ", loginType=" << iter->second.loginType << ", score=" << iter->second.score << ", level=" << iter->second.level << ", nextLevelScore=" << iter->second.nextLevelScore << ", vip=" << iter->second.vip << ", sign=" << iter->second.sign << "\n";
     }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}} //xce::user::adapter



