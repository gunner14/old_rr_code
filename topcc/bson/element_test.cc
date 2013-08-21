#include <gtest/gtest.h>
#include "bson/bsonelement.h"

using namespace bson;

TEST(ElementTest, Compile) {
  be e, f;
  e < f;
}
