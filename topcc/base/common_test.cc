#include <gtest/gtest.h>

#include "base/common.h"
#include <iostream>

TEST(CommonTest, Memoryusage) {
  size_t a = arch::CurrentMemoryUsage();
  char* f = new char[1024];
  size_t b = arch::CurrentMemoryUsage();

  // std::cout << a << " " << b << std::endl;
  EXPECT_LT(a, b);
}
