#include "gen-cpp/TwitterService.h" 
#include "xoa2/client/client_pool.h"
#include "xoa2/client/xoa_client_factory.h"

long success_count, timeout, fail;

using namespace xoa2::client;
using namespace xoa2::demo::twitter;

void * TestThread(void * data) {
  ClientPool<TwitterServiceClient> * pool = (ClientPool<TwitterServiceClient> *)data;
  
  TwitterServiceClient * client = NULL;

  Location loc;

  Tweet tweet;
  tweet.userId = 0;
  tweet.userName = "x";
  tweet.text = "hello";
  tweet.loc.latitude = 327.5; 
  tweet.loc.longitude = 327.5; 

  MCE_INFO("TestThread : thread is begin " << pthread_self());
  
  int i = 0;
  while (i++ < 10) {
    client = pool->Alloc(0);
    if (client) {
      bool success = true;
      try {
        client->ping(0);
        // client->postTweet(tweet);
        cout << "ping." << endl;
        ++success_count;
      } catch (TTransportException& e) {
        MCE_WARN("TestThread : Search() error : " << e.what());
        success = false;
        ++timeout;
      } catch (...) {
        MCE_WARN("TestThread : Search() error.");
        success = false;
        ++fail;
      }

      pool->Release(client, success); 
      if (success_count % 10000 == 0) {
        std::cerr << "success count " << success_count
          << " timeout " << timeout
          << " failure " << fail 
          << std::endl;
      }
      // break;
      usleep(1000);
    } else {
      cout << "No endpoint available." << endl;
    }
  }

  if (true || success_count % 1 == 0) {
    std::cerr << "success count " << success_count
      << " timeout " << timeout
      << " failure " << fail 
      << std::endl;
  }
  return NULL;
}
int main(int argc, char **argv) {
  MCE_INIT("./test.log", "DEBUG");
  ClientPool<TwitterServiceClient> pool("twitter.demo.xoa.renren.com", 200);

  TestThread((void *)&pool);
  sleep(100000000);
  return 0;

  int thread_count = 100;
  pthread_t * threads = new pthread_t[thread_count];

  for(int i = 0; i < thread_count; ++i) {
    int ret = pthread_create(&threads[i], NULL, TestThread, (void *)&pool);
    if (ret != 0) {
      printf("can't create thread : %s\n", strerror(ret));
    }
    MCE_DEBUG("thread " << i << "success.");
  }

  for(int i = 0; i < thread_count; ++i) {
    pthread_join(threads[i],NULL);
    MCE_DEBUG("thread " << i << "quit ");
  }
  return 0;
}

