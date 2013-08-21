#if 1

// g++ -o boost_test boost_test.cc 

#include <gtest/gtest.h>

#include <boost/throw_exception.hpp>
#include "base/logging.h"

struct foo_exception : public std::exception {
  foo_exception(const char* name) : what_(name) {}
  virtual ~foo_exception() throw() {}
  virtual const char* what() const throw() {
    return what_.c_str();
  }
private:
  std::string what_;
};

TEST(BoostTest, Exception) {
  // arch::LogMessage::LogToDebug(arch::LS_VERBOSE);
#ifdef BOOST_THROW_EXCEPTION // for boost 1.35
  BOOST_THROW_EXCEPTION(foo_exception("use macro throw"));
#endif

  boost::throw_exception(foo_exception("use function throw"));
}


#else

// g++ -o boost_test boost_test.cc -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN -lboost_unit_test_framework-gcc43-mt 

#include <gtest/gtest.h>


TEST(BoostTest, Compile) {}


#endif

