#include "gtest/gtest.h"
#include <fstream>

#include "base/logging.h"
#include "base/stringdigest.h"

TEST(string, SHA256) {
  std::ifstream in("../base/test/sha256.txt");
  if (!in) {
    LOG(LERROR) << "cannot load file";
  }
  std::string line;
  std::getline(in, line);
  int c = 0;
  while (!line.empty()) {
    ++c;
    std::string hval = line.substr(0, line.find("\t", 0));
    std::string url = line.substr(line.find("\t", 0) + 1, line.size());
    ASSERT_STREQ(hval.c_str(), xce::SHA256(url).c_str());
    line.clear();
    std::getline(in, line);
  }
  LOG(INFO) << "c:" << c;
}
