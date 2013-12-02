#include "UserBasicTripodAdapter.h"
#include <sstream>

namespace xce {
namespace adapter {
namespace userbasic {

const int UserBasicTripodAdapter::expiration = 3600 * 24 * 30;

UserBasicDataPtr UserBasicTripodAdapter::GetBasic(int user_id) {
  UserBasicDataPtr result = new UserBasicData;

  xce::tripod::StringMapType data;
  std::string key = boost::lexical_cast<string>(user_id);
  std::map<std::string, bool> success = multi_biz_client_->MultiBizGet(key, bizs_, &data, expiration);
  if(success[kBizUserPassport]) {
    std::istringstream in(data[kBizUserPassport]); 
    PbUserPassport passport;
    passport.ParseFromIstream(&in);
    UserBasicUtil::parse(passport, result);
  }
  if(success[kBizUserName]) {
    std::istringstream in(data[kBizUserName]); 
    PbUserName name;
    name.ParseFromIstream(&in);
    UserBasicUtil::parse(name, result);
  }
  if(success[kBizUserState]) {
    std::istringstream in(data[kBizUserState]); 
    PbUserState state;
    state.ParseFromIstream(&in);
    UserBasicUtil::parse(state, result);
  }
  if(success[kBizUserBorn]) {
    std::istringstream in(data[kBizUserBorn]); 
    PbUserBorn born;
    born.ParseFromIstream(&in);
    UserBasicUtil::parse(born, result);
  }
  if(success[kBizUserStage]) {
    std::istringstream in(data[kBizUserStage]); 
    PbUserStage stage;
    stage.ParseFromIstream(&in);
    UserBasicUtil::parse(stage, result);
  }
  if(success[kBizUserDoing]) {
    std::istringstream in(data[kBizUserDoing]); 
    PbUserDoing doing;
    doing.ParseFromIstream(&in);
    UserBasicUtil::parse(doing, result);
  }
  if(success[kBizUserUrl]) {
    std::istringstream in(data[kBizUserUrl]); 
    PbUserUrl url;
    url.ParseFromIstream(&in);
    UserBasicUtil::parse(url, result);
  }

  return result;
}

UserBasicTripodAdapter::UserBasicTripodAdapter() {
  bizs_.push_back(kBizUserPassport);
  bizs_.push_back(kBizUserName);
  bizs_.push_back(kBizUserState);
  bizs_.push_back(kBizUserBorn);
  bizs_.push_back(kBizUserStage);
  bizs_.push_back(kBizUserDoing);
  bizs_.push_back(kBizUserUrl);
  multi_biz_client_.reset(new xce::tripod::MultiBizTripodClient(kNameSpace));
}

UserBasicTripodAdapter::~UserBasicTripodAdapter() {
  multi_biz_client_.reset();
}

//UserBasic
void UserBasicUtil::parse(const PbUserPassport &passport, UserBasicDataPtr &result) {
  result->id = passport.id();
  result->status = passport.status();
}

void UserBasicUtil::parse(const PbUserName &name, UserBasicDataPtr &result) {
  result->name = name.name();
}

void UserBasicUtil::parse(const PbUserState &state, UserBasicDataPtr &result) {
  result->star = state.star();
}

void UserBasicUtil::parse(const PbUserBorn &born, UserBasicDataPtr &result) {
  result->gender = born.gender();
  result->birthDay = born.birth_day();
  result->birthMonth = born.birth_month();
  result->birthYear = born.birth_year();
  result->homeProvince = born.home_province();
  result->homeCity = born.home_city();
  result->cityCode = born.city_code();
  result->astrology = born.astrology();
}

void UserBasicUtil::parse(const PbUserStage &stage, UserBasicDataPtr &result) {
  result->univ = stage.univ();
  result->stage = stage.stage();
}

void UserBasicUtil::parse(const PbUserDoing &doing, UserBasicDataPtr &result) {
  result->doing = doing.doing();
  result->doingTime = doing.doingtime();
}

void UserBasicUtil::parse(const PbUserUrl &url, UserBasicDataPtr &result) {
  result->tinyUrl = url.tinyurl();
  result->headUrl = url.headurl();
}

}}} //namespace xce::adapter::userbase
