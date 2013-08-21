#include <gtest/gtest.h>

#include "base/doobs_hash.h"

TEST(HashTest, doobs) {

  for(int i = 0; i < 100; ++i) {
    int h = arch::doobs_hash((char*)&i, sizeof(i), h);

    std::cout << i << " => " << std::hex << h << std::dec <<std::endl;
  }
}
