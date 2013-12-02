/* 利用single_table_producer从DB批量获取数据来check，以增加check速度，
 * 
 * xiaofeng.liang@renren-inc.com
 */

#include <vector>
#include <iostream>
#include <fstream>

#include "IceUtil/Thread.h"
#include "IceUtil/Monitor.h"

#include "client/linux/handler/exception_handler.h"
#include "tripod2/cc/producermanager/producer_factory.h"

#include "pc_model.h"
#include "pc_impl.h"
#include "old_checker.h"
#include "cache_handler.h"

using namespace std;
using namespace xce::util;
using namespace xce::tripod;
using namespace xce::tripod::checker;

void useage(string filename) {
  cout << "用法:" << endl;
  cout << filename << " -op arg [...]" << endl;
  cout << "n:  namespace_id" << endl;
  cout << "s:  business_id" << endl;
  cout << "l:  log level, default INFO" << endl;
  cout << "f:  input id, default ./id" << endl;
  cout << "D   produce via db, all passport id" << endl;
  cout << "d:  produce via db, also give a min:max id" << endl;
  cout << "I   produce via stdin" << endl;
  cout << "L   skip check, load ids" << endl;
  cout << "b:  吞吐量，每秒钟处理ID的上限，默认100000" << endl;
  cout << "t:  消费者线程数，默认20" << endl;
  cout << "c:  消费者每次加入到队列中的id个数，默认2000" << endl;
  cout << "p:  生产者每次加入到队列中的id个数，默认10000" << endl;
  cout << "m:  队列最小长度，低于这个值唤醒ID生产者线程，默认30000" << endl;
  cout << "M:  队列最大长度，高于这个值暂停加载，默认100000" << endl;
  cout << endl;
  cout << "例子:" << endl;
  cout << filename << " -n xce_user -s user_url -f check_id >log.check 2>&1 &" << endl;
  cout << filename << " -n xce_user -s user_url -I <check_id >log.check 2>&1 &" << endl;
  cout << filename << " -n xce_user -s user_url -D >log.check 2>&1 &" << endl;
  cout << filename << " -n xce_user -s user_url -D -L >log.check 2>&1 &" << endl;
  cout << filename << " -n xce_user -s user_url -d 400000000:500000000 >log.check 2>&1 &" << endl;
  cout << filename << " -n xce_user -s user_url -D -r remove:old.xml:new.xml >log.check 2>&1 &" << endl;
}

bool MyCallback(const char *dump_path, const char *minidump_id, void *context,
            bool succeeded) {
    printf("got crash %s %s\n", dump_path, minidump_id);
      return true;
}

int main(int argc, char ** argv){
  const static int MDBA = 0;
  const static int MDBMM = 1;
  const static int MFILE = 2;
  const static int MSTDIN = 3;
  const static int NCHECK = 0;
  const static int NLOAD = 1;
  const static int NOLDCHECK = 2;
  const static int NCACHE = 3;
  /* default arguments */
  string biz = "";
  string ns = "";
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
  vector<string> id_limits;
  vector<string> opt_arg;

  if(argc == 1) {
    useage(argv[0]);
    return 0;
  }

  /* process arguments */
  char c;
  while (-1 != (c = getopt(argc, argv,
    "s:"
    "n:"
    "f:"
    "d:"
    "D"
    "I"
    "L"
    "l:"
    "b:"
    "t:"
    "c:"
    "p:"
    "r:"
    "m:"
    "M:"
  ))) {
    switch (c) {
    case 's':
      biz = optarg;
      break;
    case 'n':
      ns = optarg;
      if(ns == "old") {
        consumer_method = NOLDCHECK;
      }
      break;
    case 'd':
      producer_method = MDBMM;
      boost::split(id_limits, optarg, boost::is_any_of(":"));
      if(id_limits.size() == 2) {
        id_min = boost::lexical_cast<int>(id_limits[0]);
        id_max = boost::lexical_cast<int>(id_limits[1]);
      }
      break;
    case 'D':
      producer_method = MDBA;
      break;
    case 'I':
      producer_method = MSTDIN;
      break;
    case 'L':
      consumer_method = NLOAD;
      break;
    case 'f':
      producer_method = MFILE;
      file_name = optarg;
      break;
    case 'l':
      log_level = optarg;
      break;
    case 'b':
      band = atoi(optarg);
      break;
    case 't':
      consumer_thread = atoi(optarg);
      break;
    case 'p':
      producer_batch = atoi(optarg);
      break;
    case 'r':
      consumer_method = NCACHE;
      boost::split(opt_arg, optarg, boost::is_any_of(":"));
      break;
    case 'c':
      consumer_batch = atoi(optarg);
      break;
    case 'm':
      queue_min = atoi(optarg);
      break;
    case 'M':
      queue_max = atoi(optarg);
      break;
    default:
      break;
    }
  }

  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);

  MCE_DEFAULT_INIT(log_level);
  MCE_WARN("[Main]Init");
  MyUtil::Clock clock;
  Queue<int> mainQueue(queue_min, queue_max);
  
  /* 一个生产者线程，多个消费者线程 */
  vector<IceUtil::ThreadControl> threads;
  boost::shared_ptr<Producer> producer;
  producer.reset(ProducerFactory::GetInstance().Get(biz));
  if(producer.get() != NULL){
    com::xiaonei::xce::ConnectionPoolManager::instance().initialize();
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = NULL;
      switch(consumer_method) {
        case NCHECK:
          t = new BatchChecker(&mainQueue, consumer_batch, ns, biz);
          break;
        case NLOAD:
          t = new SimpleLoader(&mainQueue, consumer_batch, ns, biz);
          break;
        case NOLDCHECK:
          t = new OldChecker(&mainQueue, consumer_batch, biz);
          break;
        case NCACHE:
          if(opt_arg.size() != 3) {
            MCE_WARN("arg error");
            exit(-1);
          }
          int method_int;
          if(opt_arg[0] == "remove") {
            method_int = 0;
          } else if(opt_arg[0] == "load") {
            method_int = 1;
          } else if (opt_arg[0] == "set") {
            method_int = 2;
          } else {
            MCE_WARN("no supported method:" << opt_arg[0]);
            exit(-1);
          }
          t = new CacheHandler(&mainQueue, consumer_batch, ns, biz, opt_arg[1], opt_arg[2], method_int);
          break;
        default:
          break;
      }
      threads.push_back(t->start());
    }
  } else {
    cout << biz << " unavaliable" << endl;
    return 0;
  }

  IceUtil::ThreadPtr t = NULL;
  switch(producer_method) {
    case MDBMM:
    case MDBA:
      t = new DbProducer(&mainQueue, band, producer_batch, id_min, id_max);
      threads.push_back(t->start());
      break;
    case MSTDIN:
      t = new StdinProducer(&mainQueue, band, producer_batch);
      threads.push_back(t->start());
      break;
    case MFILE:
      t = new FileProducer(&mainQueue, band, producer_batch, file_name);
      threads.push_back(t->start());
      break;
    default:
      break;
  }

  for (vector<IceUtil::ThreadControl>::iterator it = threads.begin(); it != threads.end(); ++it) {
    it->join();
  }

  long cost_sec = clock.total() / 1000000;
  int cost_s = cost_sec % 60;
  int cost_m = cost_sec / 60;
  MCE_WARN("[Main]Done, total_cost: " << cost_m << " min " << cost_s << " sec.");
  return 0;
}
