#include "gtest/gtest.h"
#include "base/logging.h"
#include "base/network.h"

TEST(network, local_ip) {
  ASSERT_EQ("10.3.17.75", xce::Network::local_ip());
}

