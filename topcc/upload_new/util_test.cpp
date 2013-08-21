/*
 * util_test.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: root
 *      Copyright: renren-inc
 */

#include <string>
#include <gtest/gtest.h>
#include <upload_new/util.h>

TEST(UTIL, exifvalid) {
  std::string a("说明有可能是");
  EXPECT_EQ(IsExifValueValid(a.c_str(), a.length()), true);
  std::string b("说明有可能是abc");
  EXPECT_EQ(IsExifValueValid(b.c_str(), b.length()), true);
  std::string c("\326\320\316\304\327\326");
  EXPECT_EQ(IsExifValueValid(c.c_str(), c.length()), false);
  std::string d("ACD Systems \312\375\302\353\263\311\317\361");
  EXPECT_EQ(IsExifValueValid(d.c_str(), d.length()), false);
}
