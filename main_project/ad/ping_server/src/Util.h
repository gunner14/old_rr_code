#ifndef __AD_PINGSERVER_UTIL_H__
#define __AD_PINGSERVER_UTIL_H__

#include <curl/curl.h>
#include <string>
#include <vector>
#include "LogWrapper.h"

using namespace std;

namespace xce {
namespace ad {

const int EXCEPT_COUNT_DEFAULT = 2;
const int TIME_THRESHOLD_DEFAULT = 100;
const double AVG_MULTIPLE_DEFAULT = 2; 
const long ONE_MINUTE = 60*1000;

//size_t WriteCallBack(void *ptr, size_t size, size_t nmemb, void *data);
//bool GetHttpResult(const string& url, string& result);
void SendPhoneMsg(const vector<string>& phones, const string& msg);

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
