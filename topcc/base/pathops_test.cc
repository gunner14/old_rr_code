#include <gtest/gtest.h>
#include "base/pathops.h"

using namespace arch;

TEST(Pathops, Dirname) {
  EXPECT_EQ("/a", Dirname("/a/b"));
  EXPECT_EQ("/a/b", Dirname("/a/b/"));
  EXPECT_EQ("a", Dirname("a/b"));
  EXPECT_EQ("", Dirname("a"));
}
