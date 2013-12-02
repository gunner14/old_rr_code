#ifndef XCE_ADAPTER_USERBORNINFO_
#define XCE_ADAPTER_USERBORNINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userborn {

using namespace xce::userbase;
using namespace MyUtil;

class UserBornInfo;
typedef IceUtil::Handle<UserBornInfo> UserBornInfoPtr;

class UserBornInfo : virtual public Ice::Object, public MyUtil::Markable {
public:
  UserBornInfoPtr parse(const UserBornDataPtr& source) {
    astrology_=source->astrology;
    birthDay_=source->birthDay;
    birthMonth_=source->birthMonth;
    birthYear_=source->birthYear;
    cityCode_=source->cityCode;
    gender_=source->gender;
    homeCity_=source->homeCity;
    homeProvince_=source->homeProvince;
    id_ = source->id;
    return this;
  }
  //为UserBornReaderAdapter编译通过而存在，实际没有用到
  //UserBornReaderAdapter弃用后此方法可废弃
  UserBornInfoPtr parse(const UserBornDataNPtr& source) {
    astrology_=source->astrology;
    birthDay_=source->birthDay;
    birthMonth_=source->birthMonth;
    birthYear_=source->birthYear;
    cityCode_=source->cityCode;
    gender_=source->gender;
    homeCity_=source->homeCity;
    homeProvince_=source->homeProvince;
    id_ = source->id;
    return this;
  }
  std::string to_string() {
    std::ostringstream oss;
    oss << "UserBornInfo:[";
    oss << "id:" << id_;
    oss << ",birthMonth:" << birthMonth_;
    oss << ",birthDay:" << birthDay_;
    oss << ",birthYear:" << birthYear_;
    oss << ",astrology:" << astrology_;
    oss << ",cityCode:" << cityCode_;
    oss << ",gender:" << gender_;
    oss << ",homeCity:" << homeCity_;
    oss << ",homeProvince:" << homeProvince_;
    oss << "]";
    return oss.str();
  }
  int id() const { return id_;}
  int birthMonth() const { return birthMonth_;}
  int birthDay() const { return birthDay_;}
  int birthYear() const { return birthYear_;}
  std::string astrology() const { return astrology_;}
  std::string cityCode() const { return cityCode_;}
  std::string gender() const { return gender_;}
  std::string homeCity() const { return homeCity_;}
  std::string homeProvince() const { return homeProvince_;}
  void set_id(int id) {id_ = id;mark("ID", boost::lexical_cast<std::string>(id));}
  void set_birthMonth(int birthMonth) {birthMonth_ = birthMonth;mark("BIRTH_MONTH", boost::lexical_cast<std::string>(birthMonth));}
  void set_birthDay(int birthDay) {birthDay_ = birthDay;mark("BIRTH_DAY", boost::lexical_cast<std::string>(birthDay));}
  void set_birthYear(int birthYear) {birthYear_ = birthYear;mark("BIRTH_YEAR", boost::lexical_cast<std::string>(birthYear));}
  void set_astrology(const std::string astrology) {astrology_ = astrology;mark("ASTROLOGY", astrology);}
  void set_cityCode(const std::string cityCode) {cityCode_ = cityCode;mark("CITY_CODE", cityCode);}
  void set_gender(const std::string gender) {gender_ = gender;mark("GENDER", gender);}
  void set_homeCity(const std::string homeCity) {homeCity_ = homeCity;mark("HOME_CITY", homeCity);}
  void set_homeProvince(const std::string homeProvince) {homeProvince_ = homeProvince;mark("HOME_PROVINCE", homeProvince);}
private:
  int id_;
  int birthMonth_;
  int birthDay_;
  int birthYear_;
  std::string astrology_;
  std::string cityCode_;
  std::string gender_;
  std::string homeCity_;
  std::string homeProvince_;

};

}}} //namespace xce::adapter::userborn
#endif
