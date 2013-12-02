#include "UserScoreTripodAdapter.h"

using namespace xce::adapter;
using namespace xce::scorecache;
using namespace xce::tripoddata::userscore;

const std::string UserScoreTripodAdapter::kUserScoreNamespace = "xce_user_score";
const std::string UserScoreTripodAdapter::kBizScoreLogin = "user_score_login";
const std::string UserScoreTripodAdapter::kBizScore = "user_score";
const std::string UserScoreTripodAdapter::kBizScoreAwardCount = "user_score_award_count";
const std::string UserScoreTripodAdapter::kBizScoreSign = "user_score_sign";
const std::string UserScoreTripodAdapter::kBizScoreVip = "user_score_vip";


UserScoreTripodAdapter::UserScoreTripodAdapter() {
  score_login_client_.reset(new SingleTableTripodClient<PbScoreLogin, long>(kUserScoreNamespace, kBizScoreLogin));
  score_client_.reset(new SingleTableTripodClient<PbUserScore, long>(kUserScoreNamespace, kBizScore));
  score_award_count_client_.reset(new SingleTableTripodClient<PbScoreAwardCount,long>(kUserScoreNamespace, kBizScoreAwardCount));
  score_sign_client_.reset(new SingleTableTripodClient<PbScoreSign,long>(kUserScoreNamespace, kBizScoreSign));
  score_vip_client_.reset(new SingleTableTripodClient<PbScoreVip,long>(kUserScoreNamespace, kBizScoreVip));
  multi_biz_client_.reset(new MultiBizTripodClient(kUserScoreNamespace));
  biz_all_of_score.push_back(kBizScoreLogin);
  biz_all_of_score.push_back(kBizScore);
  biz_all_of_score.push_back(kBizScoreAwardCount);
  biz_all_of_score.push_back(kBizScoreSign);
  biz_all_of_score.push_back(kBizScoreVip);
}

ScoreDataN UserScoreTripodAdapter::GetScoreDataN(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::GetScoreDataN(), user_id = " << user_id);
  ScoreCacheDataNIPtr cache_data = new ScoreCacheDataNI();
  
  PbScoreLogin score_login;
  score_login_client_->Get(user_id, &score_login, 0);
  UserScoreUtil::parse(score_login, cache_data);
  
  PbUserScore score;
  score_client_->Get(user_id, &score, 0);
  UserScoreUtil::parse(score, cache_data);

  PbScoreAwardCount score_award_count;
  score_award_count_client_->Get(user_id, &score_award_count, 0);
  UserScoreUtil::parse(score_award_count, cache_data);

  PbScoreSign score_sign;
  score_sign_client_->Get(user_id, &score_sign, 0);
  UserScoreUtil::parse(score_sign, cache_data);

  PbScoreVip score_vip;
  score_vip_client_->Get(user_id, &score_vip, 0);
  UserScoreUtil::parse(score_vip, cache_data);

  long mask = 0;
  return cache_data->getScoreDataN(user_id, mask);
}

//// use MultiBizClient
ScoreDataN UserScoreTripodAdapter::GetScoreDataN2(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::GetScoreDataN2(), user_id = " << user_id);
  ScoreCacheDataNIPtr cache_data = new ScoreCacheDataNI();

  std::string string_key = boost::lexical_cast<string>(user_id);
  std::map<std::string, std::string> values_map;
  std::map<std::string, bool> result_map;
  try {
    result_map = multi_biz_client_->MultiBizGet(string_key, 
      biz_all_of_score, &values_map, 0);

    std::map<std::string, std::string>::const_iterator it = values_map.find(kBizScoreLogin);
    if (it != values_map.end()) {
      PbScoreLogin score_login;
      if (score_login.ParseFromString(it->second)) {
        UserScoreUtil::parse(score_login, cache_data);
      }
    }

    it = values_map.find(kBizScore);
    if (it != values_map.end()) {
      PbUserScore user_score;
      if (user_score.ParseFromString(it->second)) {
        UserScoreUtil::parse(user_score, cache_data);
      }
    }

    it = values_map.find(kBizScoreAwardCount);
    if (it != values_map.end()) {
      PbScoreAwardCount score_award_count;
      if (score_award_count.ParseFromString(it->second)) {
        UserScoreUtil::parse(score_award_count, cache_data);
      }
    }

    it = values_map.find(kBizScoreSign);
    if (it != values_map.end()) {
      PbScoreSign score_sign;
      if (score_sign.ParseFromString(it->second)) {
        UserScoreUtil::parse(score_sign, cache_data);
      }
    }

    it = values_map.find(kBizScoreVip);
    if (it != values_map.end()) {
      PbScoreVip score_vip;
      if (score_vip.ParseFromString(it->second)) {
        UserScoreUtil::parse(score_vip, cache_data);
      }
    }
  } catch (Ice::TimeoutException& ex) {
    MCE_WARN("UserScoreTripodAdapter::GetScoreDataN(use multi_biz_client), Timeout exception, user_id = " << user_id);
  } catch (...) {
    MCE_WARN("UserScoreTripodAdapter::GetScoreDataN(use multi_biz_client) ...exception, user_id = " << user_id);
  }

  long mask = 0;
  return cache_data->getScoreDataN(user_id, mask);
}

ScoreDataNMap UserScoreTripodAdapter::GetScoreDataNMap(const std::list<long>& user_ids) {
  std::map<long, ScoreCacheDataNIPtr> cache_data_map;

  std::map<long, PbScoreLogin> score_login_map;
  score_login_client_->MultiGet(user_ids, &score_login_map, 0);
  for (std::map<long, PbScoreLogin>::const_iterator iter = score_login_map.begin(); iter != score_login_map.end(); ++iter) {
    std::map<long, ScoreCacheDataNIPtr>::iterator data_iter = cache_data_map.find(iter->first);
    if (data_iter == cache_data_map.end()) {
      ScoreCacheDataNIPtr cache_data = new ScoreCacheDataNI();
      UserScoreUtil::parse(iter->second, cache_data);
      cache_data_map.insert(std::make_pair<long, ScoreCacheDataNIPtr>(iter->first, cache_data));
    } else {
      UserScoreUtil::parse(iter->second, data_iter->second);
    }
  }

  std::map<long, PbUserScore> score_map;
  score_client_->MultiGet(user_ids, &score_map, 0);
  for (std::map<long, PbUserScore>::const_iterator iter = score_map.begin(); iter != score_map.end(); ++iter) {
    std::map<long, ScoreCacheDataNIPtr>::iterator data_iter = cache_data_map.find(iter->first);
    if (data_iter == cache_data_map.end()) {
      ScoreCacheDataNIPtr cache_data = new ScoreCacheDataNI();
      UserScoreUtil::parse(iter->second, cache_data);
      cache_data_map.insert(std::make_pair<long, ScoreCacheDataNIPtr>(iter->first, cache_data));
    } else {
      UserScoreUtil::parse(iter->second, data_iter->second);
    }
  }

  std::map<long, PbScoreAwardCount> score_award_map;
  score_award_count_client_->MultiGet(user_ids, &score_award_map, 0);
  for (std::map<long, PbScoreAwardCount>::const_iterator iter = score_award_map.begin(); iter != score_award_map.end(); ++iter) {
    std::map<long, ScoreCacheDataNIPtr>::iterator data_iter = cache_data_map.find(iter->first);
    if (data_iter == cache_data_map.end()) {
      ScoreCacheDataNIPtr cache_data = new ScoreCacheDataNI();
      UserScoreUtil::parse(iter->second, cache_data);
      cache_data_map.insert(std::make_pair<long, ScoreCacheDataNIPtr>(iter->first, cache_data));
    } else {
      UserScoreUtil::parse(iter->second, data_iter->second);
    }
  }
  
  std::map<long, PbScoreSign> score_sign_map;
  score_sign_client_->MultiGet(user_ids, &score_sign_map, 0);
  for (std::map<long, PbScoreSign>::const_iterator iter = score_sign_map.begin(); iter != score_sign_map.end(); ++iter) {
    std::map<long, ScoreCacheDataNIPtr>::iterator data_iter = cache_data_map.find(iter->first);
    if (data_iter == cache_data_map.end()) {
      ScoreCacheDataNIPtr cache_data = new ScoreCacheDataNI();
      UserScoreUtil::parse(iter->second, cache_data);
      cache_data_map.insert(std::make_pair<long, ScoreCacheDataNIPtr>(iter->first, cache_data));
    } else {
      UserScoreUtil::parse(iter->second, data_iter->second);
    }
  }

  std::map<long, PbScoreVip> score_vip_map;
  score_vip_client_->MultiGet(user_ids, &score_vip_map, 0);
  for (std::map<long, PbScoreVip>::const_iterator iter = score_vip_map.begin(); iter != score_vip_map.end(); ++iter) {
    std::map<long, ScoreCacheDataNIPtr>::iterator data_iter = cache_data_map.find(iter->first);
    if (data_iter == cache_data_map.end()) {
      ScoreCacheDataNIPtr cache_data = new ScoreCacheDataNI();
      UserScoreUtil::parse(iter->second, cache_data);
      cache_data_map.insert(std::make_pair<long, ScoreCacheDataNIPtr>(iter->first, cache_data));
    } else {
      UserScoreUtil::parse(iter->second, data_iter->second);
    }
  }

  std::map<int, ScoreDataN> result;
  long mask = 0;
  for (std::map<long, ScoreCacheDataNIPtr>::const_iterator iter = cache_data_map.begin(); iter != cache_data_map.end(); ++iter) {
    result.insert(std::make_pair<int, ScoreDataN>(iter->first, iter->second->getScoreDataN(iter->first, mask)));
  }
  return result;

}
  
//// use MultiBizClient
ScoreDataNMap UserScoreTripodAdapter::GetScoreDataNMap2(const std::list<long>& user_ids) {
  MCE_DEBUG("UserScoreTripodAdapter::GetScoreDataNMap2(), user_ids.size() = " << user_ids.size()); 
  ScoreDataNMap result_map;
  for (std::list<long>::const_iterator it = user_ids.begin(); it != user_ids.end(); ++it) {
    ScoreDataN result = GetScoreDataN2(*it);
    result_map.insert(std::make_pair<int, ScoreDataN>(*it, result));
  }
  return result_map;
}

UserScoreTripodAdapter::~UserScoreTripodAdapter() {
  score_login_client_.reset();
  score_client_.reset();
  score_award_count_client_.reset();
  score_sign_client_.reset();
  score_vip_client_.reset();
  multi_biz_client_.reset();
}

void UserScoreTripodAdapter::PrintScoreDataN(const ScoreDataN& data) {
//  std::cout << data.his
}

bool UserScoreTripodAdapter::LoadScoreLogin(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::LoadScoreLogin(), user_id = " << user_id);
  return score_login_client_->Load(user_id, 0);
}

bool UserScoreTripodAdapter::LoadScore(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::LoadScore(), user_id = " << user_id);
  return score_client_->Load(user_id, 0);
}

bool UserScoreTripodAdapter::LoadScoreAwardCount(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::LoadScoreAwardCount(), user_id = " << user_id);
  return score_award_count_client_->Load(user_id, 0);
}

bool UserScoreTripodAdapter::LoadScoreSign(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::LoadScoreSign(), user_id = " << user_id);
  return score_sign_client_->Load(user_id, 0);
}

bool UserScoreTripodAdapter::LoadScoreVip(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::LoadScoreVip(), user_id = " << user_id);
  return score_vip_client_->Load(user_id, 0);
}

bool UserScoreTripodAdapter::LoadAll(long user_id) {
  MCE_DEBUG("UserScoreTripodAdapter::LoadAll(), user_id = " << user_id);
  bool result = true;
  if (!score_login_client_->Load(user_id, 0)) {
    result = false;
  }
  if (!score_client_->Load(user_id, 0)) {
    result = false;
  }
  if (!score_award_count_client_->Load(user_id, 0)) {
    result = false;
  }
  if (!score_sign_client_->Load(user_id, 0)) {
    result = false;
  }
  if (!score_vip_client_->Load(user_id, 0)) {
    result = false;
  }
  return result;
}

void UserScoreUtil::parse(const PbScoreLogin& score_login, ScoreCacheDataNIPtr& cache_data) {
  cache_data->historyLoginDays = score_login.history_days();
  cache_data->continueLoginDays = score_login.continue_days();
  cache_data->activeDays = score_login.active_days();
  cache_data->lastLoginTime = score_login.last_time();
  cache_data->loginType = score_login.login_type();
}

void UserScoreUtil::parse(const PbUserScore& score, ScoreCacheDataNIPtr& cache_data) {
  cache_data->score = score.score();
}

void UserScoreUtil::parse(const PbScoreSign& score_sign, ScoreCacheDataNIPtr& cache_data) {
  cache_data->sign = score_sign.sign();
}

void UserScoreUtil::parse(const PbScoreAwardCount& score_award, ScoreCacheDataNIPtr& cache_data) {
  cache_data->awardCount = score_award.award_count();
}

void UserScoreUtil::parse(const PbScoreVip& score_vip, ScoreCacheDataNIPtr& cache_data) {
  cache_data->vip = score_vip.vip();
}

