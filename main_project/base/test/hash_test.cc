#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include "boost/lexical_cast.hpp"
#include "base/consistenthash.h"

namespace test {

/*
TEST(hash, murmur) {
  for (int i = 0; i < 100; ++i) {
    std::string s = boost::lexical_cast<std::string>(i);
    uint32 hash = xce::Continuum::Hash(s);
    std::cerr << s << "\t" << hash << std::endl;
  }
}
*/


TEST(hash, Continuum) {
  xce::Continuum conn;
  conn.Add("wd1", 100);  
  conn.Add("wd2", 100);  
  conn.Add("wd3", 100);  
  conn.Add("wd6", 100);  
  conn.Add("wd7", 100);  
  conn.Add("wd8", 100);  
  conn.Add("wd9", 100);  
  conn.Add("wd10", 100);  
  conn.Add("wd11", 100);  
  conn.Add("wd12", 100);  
  conn.Add("wd13", 100);  
  conn.Rebuild();
  for (int i = 0; i < 100; ++i) {
    std::string k = boost::lexical_cast<std::string>(i);
    uint32 hash = xce::Continuum::Hash(k);
    std::string desc = conn.Locate(hash);
    std:: cout << k << "\t" << hash << "\t" << desc << std::endl;
  }
}

} // test

