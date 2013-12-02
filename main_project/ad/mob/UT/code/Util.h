/*
 *  Util.h
 *
 *   Created on: 2012-4-21
 *       Author: yongjun.yin
 *  Description: 定义宏、全局常量、枚举常量、数据库访问方法、类型转换方法
 */

#ifndef __AD_MOB_UTIL_H__
#define __AD_MOB_UTIL_H__

#include "QueryRunner.h"
#include "DbDescriptor.h"
#include <string>

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;

namespace xce {
namespace ad {

//Auto generate setter and getter
#define SETGET_DATA( TYPE, I )\
public:\
  void set_##I( const TYPE& in_##I ) {\
    I##_ = in_##I;\
  }\
  const TYPE I() const{\
    return I##_;\
  }\
private:\
  TYPE I##_;

enum InvertedIndexType {BRAND_INDEX=0, SELF_INDEX=1};
enum AdType {UNKNOWN_AD=-1,BRAND_AD=0, SELF_AD=1};
enum DisplayType { NORMAL3G=9, ROTATE3G=10 };
enum MemberCategoryType { MEMBER_BRAND=1 };
enum StageType { STAGE_SENIOR=1, STAGE_COLLEGE=2, STAGE_STAFF=4, STAGE_ALL=6 };
enum GenderType { GENDER_FEMALE=0, GENDER_MALE=1, GENDER_ALL=2};
enum TransType { CPC=10, CPM=20 };

const int DEFAULT_USER_ID = 2;               //默认用户ID
const long MOB_ZONE_TOP    = 100000000098;   //3G广告位ID 置顶广告位
const long MOB_ZONE_BOTTOM = 100000000099;   //3G广告位ID 底端广告位
const long MOB_ZONE_TEST   = 900000000000;   //3G广告位ID 测试广告位

const uint64_t COUNTRY = 86000000000000; 
const string MEDIA_URI = "http://jebe.xnimg.cn";
const int MAX_WIDTH = 960;

struct AdMobPool {
  enum Type { MEMBER = 1, CAMPAIGN = 2, GROUP = 3, ZONE = 4, USERBIND = 5, PLATFORM = 6, BRAND3G = 7, 
		          RESOLUTION =8, RESOLUTION_WIDGET = 9, MAX= 10};
};

struct AdMobIndex {
  enum Type { GENDER = 1, STAGE = 2, AGE = 3, GRADE = 4, SCHOOL = 5, AREA = 6, SCHOOLAREA = 7, PLATFORM = 8, NETWORK = 9, 
	           BRAND3G = 10, RESOLUTION = 11, LBS = 12, MAX =13};
};

class Util {
public:
	static void ExecuteQuery(const Statement& sql, const string& db, const string& pattern, mysqlpp::StoreQueryResult& res) {
		try{
			res = QueryRunner(db.c_str(), CDbRServer, pattern.c_str()).store(sql);
		} catch (Ice::Exception& e) {
			MCE_WARN("ExecuteQuery : Ice Exception: " << e << " pattern:" << pattern.c_str());
		} catch (std::exception& e) {
			MCE_WARN("ExecuteQuery : std exception: " << e.what() << " pattern:" << pattern.c_str());
		} catch (...) {
			MCE_WARN("ExecuteQuery : unknown exception: " << " pattern:" << pattern.c_str());
		}
	}
	static void ExecuteInsert(const Statement& sql, const string& db, const string& pattern, mysqlpp::StoreQueryResult& res) {
		ExecuteWrite(sql, db, pattern, res);
	}
	static void ExecuteDelete(const Statement& sql, const string& db, const string& pattern, mysqlpp::StoreQueryResult& res) {
		ExecuteWrite(sql, db, pattern, res);
	}
	static void ExecuteWrite(const Statement& sql, const string& db, const string& pattern, mysqlpp::StoreQueryResult& res) {
		try{
			res = QueryRunner(db.c_str(), CDbWServer, pattern.c_str()).store(sql);
		} catch (Ice::Exception& e) {
			MCE_WARN("ExecuteWrite : Ice Exception: " << e << " pattern:" << pattern.c_str());
		} catch (std::exception& e) {
			MCE_WARN("ExecuteWrite : std exception: " << e.what() << " pattern:" << pattern.c_str());
		} catch (...) {
			MCE_WARN("ExecuteWrite : unknown exception: " << " pattern:" << pattern.c_str());
		}
	}
};

#define mysql_long(I) \
  lexical_cast<Ice::Long> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)(I))
#define mysql_int(I) \
  lexical_cast<int> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)(I))
#define mysql_short(I) \
  lexical_cast<short> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)(I))
#define mysql_double(I) \
  lexical_cast<double> ((mysqlpp::Null<string, mysqlpp::NullIsZero>)(I))
#define mysql_string(I) \
  lexical_cast<std::string> ((mysqlpp::Null<string, mysqlpp::NullIsBlank>)(I))

}
}
#endif
