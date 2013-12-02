#ifndef __AD_PINGCOMPUTOR_UTIL_H__
#define __AD_PINGCOMPUTOR_UTIL_H__

#include <string>
#include <vector>

using namespace std;

namespace xce {
namespace ad {

const long ONE_MINUTE = 60*1000;
enum StatType { NN=1, NZ=2, AVG=3, STDEV=4};

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
