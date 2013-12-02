#include "gtest/gtest.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "base/logging.h"
#include "base/ptime.h"

#include "tripod2/cc/core/redis_client.h"
#include "LogWrapper.h"

namespace test {

using namespace xce::tripod;

class HelloTest : public ::testing::Test {
 public:
  static void SetUpTestCase() {
    MCE_DEFAULT_INIT("DEBUG");
  }
  static void ThreadFunction(std::string address, int port, int timeout) {
    for (int i = 0; i < 200; ++i) {
      std::ostringstream otem;
      otem << "Init RedisClient address:" << address << ", port:" << port << ", timeout:" << timeout;
      PTIME(pt, otem.str(), true, 0);
      RedisClient* redis_client = new RedisClient(address, port, timeout);
      //MCE_DEBUG("thread_id:" << boost::this_thread::get_id() << "\tredis_client->IsValid():" << redis_client->IsValid());
     // LOG(INFO) << "thread_id:" << boost::this_thread::get_id() << "\tredis_client->IsValid():" << std::boolalpha << redis_client->IsValid();
     // if (!redis_client->IsValid()) { // 
     //   std::ostringstream otem;
     //   otem << boost::this_thread::get_id() << " NotValidPing";
     //   PTIME(pt, otem.str(), true, 0);
     //   std::string pong = redis_client->Ping();
     // } else {
     //   std::ostringstream otem;
     //   otem << boost::this_thread::get_id() << " ValidPing";
     //   PTIME(pt, otem.str(), true, 0);
     //   std::string pong = redis_client->Ping();
     // }
      if (redis_client) {
        delete redis_client;
        redis_client = 0;
      }
    }
  }
  static boost::thread_group thread_group_;

};

boost::thread_group HelloTest::thread_group_;
 
TEST(tripod_core_test, hello) {
  std::cout << "tripod_core_test hello" << std::endl;
}

TEST_F(HelloTest, MultiThreadConnectionRedisServer) {
  int thread_size = 1;
  thread_size = 20;
  for (int i = 0; i < thread_size; ++i) {
    thread_group_.create_thread(boost::bind(&HelloTest::ThreadFunction, "10.2.78.253", 6379, 5));
  }
  thread_group_.join_all();
  
}

}
