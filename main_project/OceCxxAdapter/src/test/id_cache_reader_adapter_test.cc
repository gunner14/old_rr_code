#include "gtest/gtest.h"
#include <iostream>
#include "UtilCxx/src/LogWrapper.h"
#include "OceCxxAdapter/src/IdCacheReaderAdapter.h"

namespace idcache {

TEST(IdCacheReaderAdapter, getVirtualIdInfo_normal_id) {
  MCE_DEFAULT_INIT("DEBUG");
  int userId = 35178;
  try {
    VirtualIdInfoCPtr info = IdCacheReaderAdapter::instance().getVirtualIdInfo(userId);
    std::cout << "info->virtualId=" << info->virtualId << std::endl;
    std::cout << "info->userId=" << info->userId << std::endl;
    std::cout << "info->transId=" << info->transId << std::endl;
    ASSERT_FALSE(info->isVirtualId());
    ASSERT_TRUE(info->isValid());
    ASSERT_EQ(info->userId, userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    ASSERT_TRUE(false);
  }
}

TEST(IdCacheReaderAdapter, getVirtualIdInfo_invalid_virtual_id) {
  int userId = 2000000004;
  try {
    VirtualIdInfoCPtr info = IdCacheReaderAdapter::instance().getVirtualIdInfo(userId);
    std::cout << "info->virtualId=" << info->virtualId << std::endl;
    std::cout << "info->userId=" << info->userId << std::endl;
    std::cout << "info->transId=" << info->transId << std::endl;
    ASSERT_TRUE(info->isVirtualId());
    ASSERT_FALSE(info->isValid());
    ASSERT_EQ(info->virtualId, userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    ASSERT_TRUE(false);
  }
}

TEST(IdCacheReaderAdapter, getVirtualIdInfo_valid_virtual_id) {
  int userId = 2000000006;
  try {
    VirtualIdInfoCPtr info = IdCacheReaderAdapter::instance().getVirtualIdInfo(userId);
    std::cout << "info->virtualId=" << info->virtualId << std::endl;
    std::cout << "info->userId=" << info->userId << std::endl;
    std::cout << "info->transId=" << info->transId << std::endl;
    ASSERT_TRUE(info->isVirtualId());
    ASSERT_TRUE(info->isValid());
    ASSERT_EQ(info->virtualId, userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    ASSERT_TRUE(false);
  }
}

TEST(IdCacheReaderAdapter, getVirtualIdInfo_virtual_like_id) {
  int userId = 2000007472;
  try {
    VirtualIdInfoCPtr info = IdCacheReaderAdapter::instance().getVirtualIdInfo(userId);
    std::cout << "info->virtualId=" << info->virtualId << std::endl;
    std::cout << "info->userId=" << info->userId << std::endl;
    std::cout << "info->transId=" << info->transId << std::endl;
    ASSERT_FALSE(info->isVirtualId());
    ASSERT_TRUE(info->isValid());
    ASSERT_EQ(info->userId, userId);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    ASSERT_TRUE(false);
  }
}

} //namespace idcache
