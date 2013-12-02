#include "gtest/gtest.h"

#include <sstream>
#include "base/consistenthash.h"
#include "base/logging.h"


namespace test {

class ContinuumTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    continuum_ = new xce::Continuum("test");
    for (int i = 1; i <= loop; ++i) {
      std::ostringstream otem;
      otem << "br" << i;
      std::string desc = otem.str();
      continuum_->Add(desc, capacity);
      continuum_->Rebuild();
    }
  }

  static void TearDownTestCase() {
    if (continuum_) {
      delete continuum_;
      continuum_ = 0;
    }
  }

  static int loop;
  static int capacity;
  static xce::Continuum* continuum_;
};

xce::Continuum* ContinuumTest::continuum_;
int ContinuumTest::loop = 40;
int ContinuumTest::capacity = 100;

TEST_F(ContinuumTest, PointSize) {
  int point_size = loop * capacity;
  int real_point_size = continuum_->size();
  ASSERT_EQ(point_size, real_point_size);
//  LOG(INFO) << continuum_->Dump();
}

} // test



