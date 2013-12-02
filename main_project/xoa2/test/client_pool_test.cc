#include "gen-cpp/SearchEngine.h" 
#include "xoa2/client/client_pool.h"
#include "xoa2/client/xoa_client_factory.h"

long success, timeout, fail;

using namespace xoa2::client;

void * TestThread(void * data) {
  ClientPool<SearchEngineClient> * pool = (ClientPool<SearchEngineClient> *)data;
  return NULL;

  SearchEngineClient * client = NULL;
  SearchResult res;
  while (true) {
    client = pool->Alloc(0);
    if (client) {
      bool ok = true;
      MCE_DEBUG("Client allocation success.");
      for (int i = 0; i < 1; ++i) {
        if (i % 1 == 0) {
          // usleep(10);
          sleep(1);
        }
        sleep(1);
        try {
          client->Search(res, "文档", 0, 5);
          ++success;
          MCE_DEBUG("Search() ok.");
        } catch (TTransportException& e) {
          MCE_WARN("Search() error : " << e.what());
          ++timeout;
          ok = false;
        } catch (...) {
          MCE_WARN("Search() error.");
          ++fail;
          ok = false;
        }

        if (true || success % 1 == 0) {
          std::cerr << "success " << success 
            << " timeout " << timeout
            << " failure " << fail 
            << std::endl;
        }
      }
      pool->Release(client, ok);
    } else {
      MCE_WARN("NULL Client.");
    }
    break;
    sleep(1);
  }

  return NULL;
}

int main(int argc, char **argv) {
  MCE_INIT("./test.log", "DEBUG");
  ClientPool<SearchEngineClient> pool("search.demo.xoa.renren.com", 200);

  // TestThread((void *)&pool);
  // return 0;

  int thread_count = 200;
  pthread_t * threads = new pthread_t[thread_count];
  for(int i = 0; i < thread_count; i++) {
    pthread_create(&threads[i], NULL, TestThread, (void *)&pool);
  }

  for(int i = 0; i < thread_count; i++)
  {
    pthread_join(threads[i],NULL);
  }

  return 0;
}

