/** 
 * 具体的生产者消费者
 * xiaofeng.liang@renren-inc.com
 */
#ifndef XCE_TRIPOD_CHECKER_USERBASE_EXECUTER_H_
#define XCE_TRIPOD_CHECKER_USERBASE_EXECUTER_H_

#include <vector>

#include "boost/lexical_cast.hpp"

#include "tripod2/cc/core/producer.h"
#include "tripod2/cc/core/tripod_client.h"
#include "DbCxxPool/src/QueryRunner.h"

#include "OceCxxAdapter/src/UserBornAdapter.h"
#include "OceCxxAdapter/src/UserDoingAdapter.h"
#include "OceCxxAdapter/src/UserLoginCountAdapter.h"
#include "OceCxxAdapter/src/UserNameAdapter.h"
#include "OceCxxAdapter/src/UserPassportAdapter.h"
#include "OceCxxAdapter/src/UserRightAdapter.h"
#include "OceCxxAdapter/src/UserStageAdapter.h"
#include "OceCxxAdapter/src/UserStateAdapter.h"
#include "OceCxxAdapter/src/UserUrlAdapter.h"

#include "OceCxxAdapter/src/UserBornTripodAdapter.h"
#include "OceCxxAdapter/src/UserDoingTripodAdapter.h"
#include "OceCxxAdapter/src/UserLoginCountTripodAdapter.h"
#include "OceCxxAdapter/src/UserNameTripodAdapter.h"
#include "OceCxxAdapter/src/UserPassportTripodAdapter.h"
#include "OceCxxAdapter/src/UserRightTripodAdapter.h"
#include "OceCxxAdapter/src/UserStageTripodAdapter.h"
#include "OceCxxAdapter/src/UserStateTripodAdapter.h"
#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"

#include "OceProto/proto/UserBase.pb.h"

#include "old_check_executer.hpp"

using namespace xce::util;
using namespace xce::tripod;
using namespace com::xiaonei::xce;
using namespace xce::tripoddata::userbase;

using namespace xce::adapter::userpassport;
using namespace xce::adapter::username;
using namespace xce::adapter::userstate;
using namespace xce::adapter::userright;
using namespace xce::adapter::userlogincount;
using namespace xce::adapter::userurl;
using namespace xce::adapter::userborn;
using namespace xce::adapter::userstage;
using namespace xce::adapter::userdoing;

namespace xce {
namespace tripod {
namespace checker {

class UrlExecuter : virtual public OldCheckExecuter<UserUrlInfo, UserUrlInfoPtr, PbUserUrl> {
public:
  UrlExecuter() : OldCheckExecuter<UserUrlInfo, UserUrlInfoPtr, PbUserUrl>("user_url"){}
  virtual bool compare(UserUrlInfoPtr db, UserUrlInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<string>(id, "largeurl", db->largeUrl(), cache->largeUrl());
    result &= checkItem<string>(id, "mainurl", db->mainUrl(), cache->mainUrl());
    result &= checkItem<string>(id, "headurl", db->headUrl(), cache->headUrl());
    result &= checkItem<string>(id, "tinyurl", db->tinyUrl(), cache->tinyUrl());
    result &= checkItem<string>(id, "flexurl", db->flexUrl(), cache->flexUrl());
    return result;
  }
  virtual UserUrlInfoPtr get(int uid) {
    return UserUrlAdapter::instance().getUserUrl(uid);
  }
  virtual void load(int uid) {
    UserUrlAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserUrl pb_data, UserUrlInfoPtr &data) {
    UserUrlUtil::parse(pb_data, data);
  }
};

class NameExecuter : virtual public OldCheckExecuter<UserNameInfo, UserNameInfoPtr, PbUserName> {
public:
  NameExecuter() : OldCheckExecuter<UserNameInfo, UserNameInfoPtr, PbUserName>("user_name"){}
  virtual bool compare(UserNameInfoPtr db, UserNameInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<string>(id, "name", db->name(), cache->name());
    result &= checkItem<string>(id, "nickname", db->nickname(), cache->nickname());
    return result;
  }
  virtual UserNameInfoPtr get(int uid) {
    return UserNameAdapter::instance().getUserName(uid);
  }
  virtual void load(int uid) {
    UserNameAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserName pb_data, UserNameInfoPtr &data) {
    UserNameUtil::parse(pb_data, data);
  }
};

class StageExecuter : virtual public OldCheckExecuter<UserStageInfo, UserStageInfoPtr, PbUserStage> {
public:
  StageExecuter() : OldCheckExecuter<UserStageInfo, UserStageInfoPtr, PbUserStage>("user_stage"){}
  virtual bool compare(UserStageInfoPtr db, UserStageInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<int>(id, "univ", db->univ(), cache->univ());
    result &= checkItem<int>(id, "stage", db->stage(), cache->stage());
    return result;
  }
  virtual UserStageInfoPtr get(int uid) {
    return UserStageAdapter::instance().getUserStage(uid);
  }
  virtual void load(int uid) {
    UserStageAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserStage pb_data, UserStageInfoPtr &data) {
    UserStageUtil::parse(pb_data, data);
  }
};

class StateExecuter : virtual public OldCheckExecuter<UserStateInfo, UserStateInfoPtr, PbUserState> {
public:
  StateExecuter() : OldCheckExecuter<UserStateInfo, UserStateInfoPtr, PbUserState>("user_state"){}
  virtual bool compare(UserStateInfoPtr db, UserStateInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<long>(id, "state", db->state(), cache->state());
    result &= checkItem<int>(id, "star", db->star(), cache->star());
    result &= checkItem<int>(id, "level", db->level(), cache->level());
    return result;
  }
  virtual UserStateInfoPtr get(int uid) {
    return UserStateAdapter::instance().getUserState(uid);
  }
  virtual void load(int uid) {
    UserStateAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserState pb_data, UserStateInfoPtr &data) {
    UserStateUtil::parse(pb_data, data);
  }
};

class BornExecuter : virtual public OldCheckExecuter<UserBornInfo, UserBornInfoPtr, PbUserBorn> {
public:
  BornExecuter() : OldCheckExecuter<UserBornInfo, UserBornInfoPtr, PbUserBorn>("user_born"){}
  virtual bool compare(UserBornInfoPtr db, UserBornInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<int>(id, "birth_day", db->birthDay(), cache->birthDay());
    result &= checkItem<int>(id, "birth_month", db->birthMonth(), cache->birthMonth());
    result &= checkItem<int>(id, "birth_year", db->birthYear(), cache->birthYear());
    result &= checkItem<string>(id, "astrology", db->astrology(), cache->astrology());
    result &= checkItem<string>(id, "gender", db->gender(), cache->gender());
    result &= checkItem<string>(id, "home_province", db->homeProvince(), cache->homeProvince());
    result &= checkItem<string>(id, "home_city", db->homeCity(), cache->homeCity());
    result &= checkItem<string>(id, "city_code", db->cityCode(), cache->cityCode());
    //result &= checkItem<int>(id, "is_lunar_birth", db->is_lunar_birth(), cache->is_lunar_birth());
    return result;
  }
  virtual UserBornInfoPtr get(int uid) {
    return UserBornAdapter::instance().getUserBorn(uid);
  }
  virtual void load(int uid) {
    UserBornAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserBorn pb_data, UserBornInfoPtr &data) {
    UserBornUtil::parse(pb_data, data);
  }
};

class RightExecuter : virtual public OldCheckExecuter<UserRightInfo, UserRightInfoPtr, PbUserRight> {
public:
  RightExecuter() : OldCheckExecuter<UserRightInfo, UserRightInfoPtr, PbUserRight>("user_right"){}
  virtual bool compare(UserRightInfoPtr db, UserRightInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<int>(id, "auth", db->auth(), cache->auth());
    result &= checkItem<int>(id, "common_rights", db->rights(), cache->rights());
    return result;
  }
  virtual UserRightInfoPtr get(int uid) {
    return UserRightAdapter::instance().getUserRight(uid);
  }
  virtual void load(int uid) {
    UserRightAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserRight pb_data, UserRightInfoPtr &data) {
    UserRightUtil::parse(pb_data, data);
  }
};

class DoingExecuter : virtual public OldCheckExecuter<UserDoingInfo, UserDoingInfoPtr, PbUserDoing> {
public:
  DoingExecuter() : OldCheckExecuter<UserDoingInfo, UserDoingInfoPtr, PbUserDoing>("user_doing"){}
  virtual bool compare(UserDoingInfoPtr db, UserDoingInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<long>(id, "doingtime", db->dointTime(), cache->dointTime());
    result &= checkItem<string>(id, "doing", db->doing(), cache->doing());
    return result;
  }
  virtual UserDoingInfoPtr get(int uid) {
    return UserDoingAdapter::instance().getUserDoing(uid);
  }
  virtual void load(int uid) {
    UserDoingAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserDoing pb_data, UserDoingInfoPtr &data) {
    UserDoingUtil::parse(pb_data, data);
  }
};

//class LoginCountExecuter : virtual public OldCheckExecuter<UserLoginCountInfo, UserLoginCountInfoPtr, PbUserLoginCount> {
//public:
//  LoginCountExecuter() : OldCheckExecuter<UserLoginCountInfo, UserLoginCountInfoPtr, PbUserLoginCount>("user_logincount"){}
//  virtual bool compare(UserLoginCountInfoPtr db, UserLoginCountInfoPtr cache) {
//    bool result = true;
//    int id = db->id();
//    result &= checkItem<int>(id, "id", db->id(), cache->id());
//    result &= checkItem<int>(id, "logincount", db->logincount(), cache->logincount());
//    return result;
//  }
//  virtual UserLoginCountInfoPtr get(int uid) {
//    return UserLoginCountAdapter::instance().getUserLoginCount(uid);
//  }
//  virtual void load(int uid) {
//    UserLoginCountAdapter::instance().reload(uid); 
//  }
//  virtual void parse(PbUserLoginCount pb_data, UserLoginCountInfoPtr &data) {
//    UserLoginCountUtil::parse(pb_data, data);
//  }
//};
//
class PassportExecuter : virtual public OldCheckExecuter<UserPassportInfo, UserPassportInfoPtr, PbUserPassport> {
public:
  PassportExecuter() : OldCheckExecuter<UserPassportInfo, UserPassportInfoPtr, PbUserPassport>("user_passport"){}
  virtual bool compare(UserPassportInfoPtr db, UserPassportInfoPtr cache) {
    bool result = true;
    int id = db->id();
    result &= checkItem<int>(id, "id", db->id(), cache->id());
    result &= checkItem<int>(id, "status", db->status(), cache->status());
    result &= checkItem<int>(id, "safestatus", db->safestatus(), cache->safestatus());
    result &= checkItem<string>(id, "account", db->account(), cache->account());
    result &= checkItem<string>(id, "passwordmd5", db->passwordmd5(), cache->passwordmd5());
    result &= checkItem<string>(id, "domain", db->domain(), cache->domain());
    return result;
  }
  virtual UserPassportInfoPtr get(int uid) {
    return UserPassportAdapter::instance().getUserPassport(uid);
  }
  virtual void load(int uid) {
    UserPassportAdapter::instance().reload(uid); 
  }
  virtual void parse(PbUserPassport pb_data, UserPassportInfoPtr &data) {
    UserPassportUtil::parse(pb_data, data);
  }
};


}}}// namespace xce::tripod

#endif
