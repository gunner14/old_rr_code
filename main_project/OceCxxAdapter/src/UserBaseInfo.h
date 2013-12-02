#ifndef XCE_ADAPTER_USERBASEADAPTER_H_
#define XCE_ADAPTER_USERBASEADAPTER_H_

#include "UserBase.h"
#include <string>
#include <iostream>

namespace xce {
namespace adapter {
namespace userbase {

using namespace ::xce::userbase;
using namespace MyUtil;
using std::string;
using std::ostringstream;

//为TripodCache保留转换后门
class UserBaseUtil;

class UserBaseFullView;
typedef IceUtil::Handle<UserBaseFullView> UserBaseFullViewPtr;

class UserBaseCommonView;
typedef IceUtil::Handle<UserBaseCommonView> UserBaseCommonViewPtr;


class UserBaseFullView : public Ice::Object {
public:
  UserBaseFullView() {}
  UserBaseFullView(const UserBaseViewData3Ptr& data) :
    id_(data->upd->uID),
    status_(data->upd->status),
    safestatus_(data->upd->safeStatus),
    account_(data->upd->account),
    passwordmd5_(data->upd->passwordmd5),
    domain_(data->upd->domain),
    name_(data->und->name),
    nickname_(data->und->nickname),
    auth_(data->urd->auth),
    rights_(data->urd->rights),
    state_(data->usd->state),
    star_(data->usd->star),
    level_(data->usd->level),
    logincount_(data->ulcd->logincount),
    gender_(data->userbasic->gender),
    birthday_(data->userbasic->birthDay),
    birthmonth_(data->userbasic->birthMonth),
    birthyear_(data->userbasic->birthYear),
    homeprovince_(data->userbasic->homeProvince),
    homecity_(data->userbasic->homeCity),
    citycode_(data->userbasic->cityCode),
    astrology_(data->userbasic->astrology),
    univ_(data->userbasic->univ),
    stage_(data->userbasic->stage),
    doing_(data->userbasic->doing),
    doingtime_(data->userbasic->doingTime),
    largeurl_(data->userurl->largeUrl),
    tinyurl_(data->userurl->tinyUrl),
    headurl_(data->userurl->headUrl),
    mainurl_(data->userurl->mainUrl),
    flexurl_(data->userurl->flexUrl) {
  }

  string to_string() {
    ostringstream oss;
    oss << "UserBaseFullView:[";
    oss << "id:" << id_;
    oss << ",status:" << status_;
    oss << ",safestatus:" << safestatus_;
    oss << ",account:" << account_;
    oss << ",passwordmd5:" << passwordmd5_;
    oss << ",domain:" << domain_;
    oss << ",name:" << name_;
    oss << ",nickname:" << nickname_;
    oss << ",auth:" << auth_;
    oss << ",rights:" << rights_;
    oss << ",state:" << state_;
    oss << ",star:" << star_;
    oss << ",level:" << level_;
    oss << ",logincount:" << logincount_;
    oss << ",gender:" << gender_;
    oss << ",birthday:" << birthday_;
    oss << ",birthmonth:" << birthmonth_;
    oss << ",birthyear:" << birthyear_;
    oss << ",homeprovince:" << homeprovince_;
    oss << ",homecity:" << homecity_;
    oss << ",citycode:" << citycode_;
    oss << ",astrology:" << astrology_;
    oss << ",univ:" << univ_;
    oss << ",stage:" << stage_;
    oss << ",doing:" << doing_;
    oss << ",doingtime:" << doingtime_;
    oss << ",largeurl:" << largeurl_;
    oss << ",tinyurl:" << tinyurl_;
    oss << ",headurl:" << headurl_;
    oss << ",mainurl:" << mainurl_;
    oss << ",flexurl:" << flexurl_;
    oss << "]";
    return oss.str();
  }

  int id() const {return id_;}
  int status() const {return status_;}
  int safestatus() const {return safestatus_;}
  string account() const {return account_;}
  string passwordmd5() const {return passwordmd5_;}
  string domain() const {return domain_;}

  string name() const {return name_;}
  string nickname() const {return nickname_;}

  int auth() const {return auth_;}
  int rights() const {return rights_;}

  long state() const {return state_;}
  int star() const {return star_;}
  int level() const {return level_;}

  int logincount() const {return logincount_;}

  string gender() const {return gender_;}
  int birthday() const {return birthday_;}
  int birthmonth() const {return birthmonth_;}
  int birthyear() const {return birthyear_;}
  string homeprovince() const {return homeprovince_;}
  string homecity() const {return homecity_;}
  string citycode() const {return citycode_;}
  string astrology() const {return astrology_;}

  int univ() const {return univ_;}
  int stage() const {return stage_;}

  string doing() const {return doing_;}
  int doingtime() const {return doingtime_;}

  string largeurl() const {return largeurl_;}
  string tinyurl() const {return tinyurl_;}
  string headurl() const {return headurl_;}
  string mainurl() const {return mainurl_;}
  string flexurl() const {return flexurl_;}

private:
  friend class UserBaseUtil;
  int id_;
  int status_;
  int safestatus_;
  string account_;
  string passwordmd5_;
  string domain_;

  string name_;
  string nickname_;

  int auth_;
  int rights_;

  long state_;
  int star_;
  int level_;

  int logincount_;

  string gender_;
  int birthday_;
  int birthmonth_;
  int birthyear_;
  string homeprovince_;
  string homecity_;
  string citycode_;
  string astrology_;

  int univ_;
  int stage_;

  string doing_;
  int doingtime_;

  string largeurl_;
  string tinyurl_;
  string headurl_;
  string mainurl_;
  string flexurl_;
};

class UserBaseCommonView : public Ice::Object {
public:
  UserBaseCommonView() {}
  UserBaseCommonView(const UserCommonViewDataNPtr& data) :
    id_(data->userpassport->uID),
    status_(data->userpassport->status),
    safestatus_(data->userpassport->safeStatus),
    account_(data->userpassport->account),
    passwordmd5_(data->userpassport->passwordmd5),
    domain_(data->userpassport->domain),
    name_(data->username->name),
    nickname_(data->username->nickname),
    auth_(data->userright->auth),
    rights_(data->userright->rights),
    state_(data->userstate->state),
    star_(data->userstate->star),
    level_(data->userstate->level),
    logincount_(data->userlogincount->logincount),
    gender_(data->gender),
    univ_(data->univ),
    stage_(data->stage),
    largeurl_(data->userurl->largeUrl),
    tinyurl_(data->userurl->tinyUrl),
    headurl_(data->userurl->headUrl),
    mainurl_(data->userurl->mainUrl),
    flexurl_(data->userurl->flexUrl){
  }

  string to_string() {
    ostringstream oss;
    oss << "UserBaseCommonView:[";
    oss << "id:" << id_;
    oss << ",status:" << status_;
    oss << ",safestatus:" << safestatus_;
    oss << ",account:" << account_;
    oss << ",passwordmd5:" << passwordmd5_;
    oss << ",domain:" << domain_;
    oss << ",name:" << name_;
    oss << ",nickname:" << nickname_;
    oss << ",auth:" << auth_;
    oss << ",rights:" << rights_;
    oss << ",state:" << state_;
    oss << ",star:" << star_;
    oss << ",level:" << level_;
    oss << ",logincount:" << logincount_;
    oss << ",gender:" << gender_;
    oss << ",univ:" << univ_;
    oss << ",stage:" << stage_;
    oss << ",largeurl:" << largeurl_;
    oss << ",tinyurl:" << tinyurl_;
    oss << ",headurl:" << headurl_;
    oss << ",mainurl:" << mainurl_;
    oss << ",flexurl:" << flexurl_;
    oss << "]";
    return oss.str();
  }

  int id() const {return id_;}
  int status() const {return status_;}
  int safestatus() const {return safestatus_;}
  string account() const {return account_;}
  string passwordmd5() const {return passwordmd5_;}
  string domain() const {return domain_;}

  string name() const {return name_;}
  string nickname() const {return nickname_;}

  int auth() const {return auth_;}
  int rights() const {return rights_;}

  long state() const {return state_;}
  int star() const {return star_;}
  int level() const {return level_;}

  int logincount() const {return logincount_;}

  string gender() const {return gender_;}

  int univ() const {return univ_;}
  int stage() const {return stage_;}

  string largeurl() const {return largeurl_;}
  string tinyurl() const {return tinyurl_;}
  string headurl() const {return headurl_;}
  string mainurl() const {return mainurl_;}
  string flexurl() const {return flexurl_;}
private:
  friend class UserBaseUtil;
  int id_;
  int status_;
  int safestatus_;
  string account_;
  string passwordmd5_;
  string domain_;

  string name_;
  string nickname_;

  int auth_;
  int rights_;

  long state_;
  int star_;
  int level_;

  int logincount_;

  string gender_;

  int univ_;
  int stage_;

  string largeurl_;
  string tinyurl_;
  string headurl_;
  string mainurl_;
  string flexurl_;
};

}}} //namespace xce::adapter::userbase

#endif
