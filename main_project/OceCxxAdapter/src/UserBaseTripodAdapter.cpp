#include "UserBaseTripodAdapter.h"
#include <sstream>

namespace xce {
namespace adapter {
namespace userbase {

const int UserBaseTripodAdapter::expiration = 3600 * 24 * 30;

UserBaseCommonViewPtr UserBaseTripodAdapter::GetCommon(int user_id) {
  UserBaseCommonViewPtr result = new UserBaseCommonView;

  xce::tripod::StringMapType data;
  std::string key = boost::lexical_cast<string>(user_id);
  std::map<std::string, bool> success = multi_biz_client_->MultiBizGet(key, bizs_common_, &data, expiration);
  if(success[kBizUserPassport]) {
    std::istringstream in(data[kBizUserPassport]); 
    PbUserPassport passport;
    passport.ParseFromIstream(&in);
    UserBaseUtil::parse(passport, result);
  }
  if(success[kBizUserName]) {
    std::istringstream in(data[kBizUserName]); 
    PbUserName name;
    name.ParseFromIstream(&in);
    UserBaseUtil::parse(name, result);
  }
  if(success[kBizUserRight]) {
    std::istringstream in(data[kBizUserRight]); 
    PbUserRight right;
    right.ParseFromIstream(&in);
    UserBaseUtil::parse(right, result);
  }
  if(success[kBizUserState]) {
    std::istringstream in(data[kBizUserState]); 
    PbUserState state;
    state.ParseFromIstream(&in);
    UserBaseUtil::parse(state, result);
  }
  if(success[kBizUserLoginCount]) {
    UserBaseUtil::parse(boost::lexical_cast<int>(data[kBizUserLoginCount]), result); 
  }
  if(success[kBizUserBorn]) {
    std::istringstream in(data[kBizUserBorn]); 
    PbUserBorn born;
    born.ParseFromIstream(&in);
    UserBaseUtil::parse(born, result);
  }
  if(success[kBizUserStage]) {
    std::istringstream in(data[kBizUserStage]); 
    PbUserStage stage;
    stage.ParseFromIstream(&in);
    UserBaseUtil::parse(stage, result);
  }
  if(success[kBizUserUrl]) {
    std::istringstream in(data[kBizUserUrl]); 
    PbUserUrl url;
    url.ParseFromIstream(&in);
    UserBaseUtil::parse(url, result);
  }

  return result;
}

UserBaseFullViewPtr UserBaseTripodAdapter::GetFull(int user_id) {
  UserBaseFullViewPtr result = new UserBaseFullView;

  xce::tripod::StringMapType data;
  std::string key = boost::lexical_cast<string>(user_id);
  std::map<std::string, bool> success = multi_biz_client_->MultiBizGet(key, bizs_full_, &data, expiration);
  if(success[kBizUserPassport]) {
    std::istringstream in(data[kBizUserPassport]); 
    PbUserPassport passport;
    passport.ParseFromIstream(&in);
    UserBaseUtil::parse(passport, result);
  }
  if(success[kBizUserName]) {
    std::istringstream in(data[kBizUserName]); 
    PbUserName name;
    name.ParseFromIstream(&in);
    UserBaseUtil::parse(name, result);
  }
  if(success[kBizUserRight]) {
    std::istringstream in(data[kBizUserRight]); 
    PbUserRight right;
    right.ParseFromIstream(&in);
    UserBaseUtil::parse(right, result);
  }
  if(success[kBizUserState]) {
    std::istringstream in(data[kBizUserState]); 
    PbUserState state;
    state.ParseFromIstream(&in);
    UserBaseUtil::parse(state, result);
  }
  if(success[kBizUserLoginCount]) {
    UserBaseUtil::parse(boost::lexical_cast<int>(data[kBizUserLoginCount]), result); 
  }
  if(success[kBizUserBorn]) {
    std::istringstream in(data[kBizUserBorn]); 
    PbUserBorn born;
    born.ParseFromIstream(&in);
    UserBaseUtil::parse(born, result);
  }
  if(success[kBizUserStage]) {
    std::istringstream in(data[kBizUserStage]); 
    PbUserStage stage;
    stage.ParseFromIstream(&in);
    UserBaseUtil::parse(stage, result);
  }
  if(success[kBizUserUrl]) {
    std::istringstream in(data[kBizUserUrl]); 
    PbUserUrl url;
    url.ParseFromIstream(&in);
    UserBaseUtil::parse(url, result);
  }
  if(success[kBizUserDoing]) {
    std::istringstream in(data[kBizUserDoing]); 
    PbUserDoing doing;
    doing.ParseFromIstream(&in);
    UserBaseUtil::parse(doing, result);
  }

  return result;
}

UserBaseTripodAdapter::UserBaseTripodAdapter() {
  bizs_common_.push_back(kBizUserPassport);
  bizs_common_.push_back(kBizUserName);
  bizs_common_.push_back(kBizUserRight);
  bizs_common_.push_back(kBizUserState);
  bizs_common_.push_back(kBizUserLoginCount);
  bizs_common_.push_back(kBizUserBorn);
  bizs_common_.push_back(kBizUserStage);
  bizs_common_.push_back(kBizUserUrl);
  bizs_full_.insert(bizs_full_.begin(), bizs_common_.begin(), bizs_common_.end());
  bizs_full_.push_back(kBizUserDoing);
  multi_biz_client_.reset(new xce::tripod::MultiBizTripodClient(kNameSpace));
}

UserBaseTripodAdapter::~UserBaseTripodAdapter() {
  multi_biz_client_.reset();
}

//UserBaseCommonView
void UserBaseUtil::parse(const PbUserPassport &passport, UserBaseCommonViewPtr &result) {
  result->id_ = passport.id();
  result->status_ = passport.status();
  result->safestatus_ = passport.safe_status();
  result->account_ = passport.account();
  result->passwordmd5_ = passport.password_md5();
  result->domain_ = passport.domain();
}

void UserBaseUtil::parse(const PbUserName &name, UserBaseCommonViewPtr &result) {
  result->name_ = name.name();
  result->nickname_ = name.nickname();
}

void UserBaseUtil::parse(const PbUserRight &right, UserBaseCommonViewPtr &result) {
  result->auth_ = right.auth();
  result->rights_ = right.common_rights();
}

void UserBaseUtil::parse(const PbUserState &state, UserBaseCommonViewPtr &result) {
  result->state_ = state.state();
  result->star_ = state.star();
  result->level_ = state.level();
}

void UserBaseUtil::parse(const PbUserLoginCount &logincount, UserBaseCommonViewPtr &result) {
  result->logincount_ = logincount.logincount();
}

void UserBaseUtil::parse(const int logincount, UserBaseCommonViewPtr &result) {
  result->logincount_ = logincount;
}

void UserBaseUtil::parse(const PbUserBorn &born, UserBaseCommonViewPtr &result) {
  result->gender_ = born.gender();
}

void UserBaseUtil::parse(const PbUserStage &stage, UserBaseCommonViewPtr &result) {
  result->univ_ = stage.univ();
  result->stage_ = stage.stage();
}

void UserBaseUtil::parse(const PbUserUrl &url, UserBaseCommonViewPtr &result) {
  result->largeurl_ = url.largeurl();
  result->tinyurl_ = url.tinyurl();
  result->headurl_ = url.headurl();
  result->mainurl_ = url.mainurl();
  result->flexurl_ = url.flexurl();
}

//UserBaseFullView
void UserBaseUtil::parse(const PbUserPassport &passport, UserBaseFullViewPtr &result) {
  result->id_ = passport.id();
  result->status_ = passport.status();
  result->safestatus_ = passport.safe_status();
  result->account_ = passport.account();
  result->passwordmd5_ = passport.password_md5();
  result->domain_ = passport.domain();
}

void UserBaseUtil::parse(const PbUserName &name, UserBaseFullViewPtr &result) {
  result->name_ = name.name();
  result->nickname_ = name.nickname();
}

void UserBaseUtil::parse(const PbUserRight &right, UserBaseFullViewPtr &result) {
  result->auth_ = right.auth();
  result->rights_ = right.common_rights();
}

void UserBaseUtil::parse(const PbUserState &state, UserBaseFullViewPtr &result) {
  result->state_ = state.state();
  result->star_ = state.star();
  result->level_ = state.level();
}

void UserBaseUtil::parse(const PbUserLoginCount &logincount, UserBaseFullViewPtr &result) {
  result->logincount_ = logincount.logincount();
}

void UserBaseUtil::parse(const int logincount, UserBaseFullViewPtr &result) {
  result->logincount_ = logincount;
}

void UserBaseUtil::parse(const PbUserBorn &born, UserBaseFullViewPtr &result) {
  result->gender_ = born.gender();
  result->birthday_ = born.birth_day();
  result->birthmonth_ = born.birth_month();
  result->birthyear_ = born.birth_year();
  result->homeprovince_ = born.home_province();
  result->homecity_ = born.home_city();
  result->citycode_ = born.city_code();
  result->astrology_ = born.astrology();
}

void UserBaseUtil::parse(const PbUserStage &stage, UserBaseFullViewPtr &result) {
  result->univ_ = stage.univ();
  result->stage_ = stage.stage();
}

void UserBaseUtil::parse(const PbUserDoing &doing, UserBaseFullViewPtr &result) {
  result->doing_ = doing.doing();
  result->doingtime_ = doing.doingtime();
}

void UserBaseUtil::parse(const PbUserUrl &url, UserBaseFullViewPtr &result) {
  result->largeurl_ = url.largeurl();
  result->tinyurl_ = url.tinyurl();
  result->headurl_ = url.headurl();
  result->mainurl_ = url.mainurl();
  result->flexurl_ = url.flexurl();
}

}}} //namespace xce::adapter::userbase
