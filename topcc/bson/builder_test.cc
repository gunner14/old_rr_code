#include <gtest/gtest.h>
#include "bson/builder.h"

using namespace bson;

TEST(BsonBuilder, BufferBuilder) {
  BufBuilder bb;
  EXPECT_EQ(512, bb.getSize());
  EXPECT_EQ(0, bb.len());

  bb.appendChar('c');
  EXPECT_EQ(1, bb.len());

  bb.appendNum(1);
  EXPECT_EQ(1 + sizeof(int), bb.len());

  bb.grow(1);
  EXPECT_EQ(1 + sizeof(int) + 1, bb.len());

  bb.grow(1024);
  EXPECT_EQ(1 + sizeof(int) + 1 + 1024 + 16 * 1024, bb.getSize());
  EXPECT_EQ(1 + sizeof(int) + 1 + 1024, bb.len());
}
