#include <vector>
#include <iostream>

#include "IceUtil/Thread.h"
#include "IceUtil/Monitor.h"

#include "pc_model.h"
#include "pc_impl.h"

using namespace std;
using namespace xce::util;
using namespace xce::score;

int main(int argc, char ** argv){
  /* default arguments */
  string log_level = "INFO";
  string file_name = "id";
  int producer_method = 0;
  int consumer_method = 0;
  int id_min = 0;
  int id_max = 0x7fffffff; 
  unsigned band = 1000000;
  unsigned consumer_thread = 20;
  unsigned consumer_batch = 2000;
  unsigned producer_batch = 10000;
  unsigned queue_min = 30000;
  unsigned queue_max = 100000;

  com::xiaonei::xce::ConnectionPoolManager::instance().initialize();

  MCE_DEFAULT_INIT(log_level);
  MCE_WARN("[Main]Init");
  MyUtil::Clock clock;
  Queue<int> mainQueue(queue_min, queue_max);
  vector<int> counts(consumer_thread, 0);
  
  /* 一个生产者线程，多个消费者线程 */
  vector<IceUtil::ThreadControl> threads;
  for (unsigned i = 0; i < consumer_thread; ++i) {
    IceUtil::ThreadPtr t = new ScoreGuarder(&mainQueue, consumer_batch, counts[i]);
    threads.push_back(t->start());
  }
  IceUtil::ThreadPtr t = new DbProducer(&mainQueue, band, producer_batch, id_min, id_max);
  threads.push_back(t->start());

  for (vector<IceUtil::ThreadControl>::iterator it = threads.begin(); it != threads.end(); ++it) {
    it->join();
  }

  int countAll = 0;
  for (int i = 0; i < counts.size(); ++i) {
    countAll += counts[i];
  }

  long cost_sec = clock.total() / 1000000;
  int cost_s = cost_sec % 60;
  int cost_m = cost_sec / 60;
  MCE_INFO("[Main]Done, total_cost: " << cost_m << " min " << cost_s << " sec, count: " << countAll << ".");
  return 0;
}
