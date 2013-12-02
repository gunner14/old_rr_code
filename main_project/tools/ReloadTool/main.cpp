/* 用于重新加载UserBase，UserConfig，UserDesc服务
 * 从标准输入读取id列表，多线程并发加载
 * 
 * xiaofeng.liang@renren-inc.com
 */

#include <iostream>
#include <vector>
#include <IceUtil/Thread.h>
#include <IceUtil/Monitor.h>
#include "UserBaseTripodAdapter.h"
#include "UserConfigAdapter.h"
#include "UserDescReaderAdapter.h"
#include "UserStateAdapter.h"
#include "UserPassportAdapter.h"
#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"
#include "OceCxxAdapter/src/UserStateTripodAdapter.h"
#include "OceCxxAdapter/src/UserLoginCountTripodAdapter.h"

using namespace std;
using namespace xce::adapter;
using namespace xce::adapter::userbase;
using namespace xce::adapter::userdesc;
using namespace xce::adapter::userurl;
using namespace xce::adapter::userstate;
using namespace xce::adapter::userpassport;
using namespace xce::adapter::userlogincount;

/* 多线程读写队列 */
template<class T> class Queue : public IceUtil::Monitor<IceUtil::Mutex> {
  list<T> _q;
  short _waitingReaders;
  bool _finished;
  bool _isFull;
  const unsigned _min, _max;
public:
  Queue(int min, int max)
      : _waitingReaders(0), _finished(false), _isFull(false), _min(min), _max(max) {}

  void put(const vector<T> & items) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    if(_isFull) {
      notifyAll();
      wait();
    }
    _q.insert(_q.end(), items.begin(), items.end());
    if(_q.size() > _max) {
      _isFull = true;
    }
    if (_waitingReaders) {
      notify();
    }
  }

  vector<T> get(int size) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    while (_q.size() == 0 && !_finished) {
      try {
        ++_waitingReaders;
        wait();
        --_waitingReaders;
      } catch (...) {
        --_waitingReaders;
        throw;
      }
    }
    vector<T> items;
    if(_q.size() != 0) {
      while(_q.size() && size--) {
        items.push_back(_q.front());
        _q.pop_front();
      }
      if(_q.size() < _min && !_finished) {
        _isFull = false;
        notify();
      }
    }
    return items;
  }

  void finish() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    _finished = true;
    notifyAll();
  }
};

/* 消费者，从队列获取ID并Reload */
class ConsumerThread : public IceUtil::Thread {
  Queue<int> *_queue;
  int _batch;
  string _service;
public:
  ConsumerThread(Queue<int> *queue, int batch, string service)
      : _queue(queue), _batch(batch), _service(service) {}
  virtual void run() {
    int count = 0;
    while(true) {
      vector<int> ids = (*_queue).get(_batch);
      if(ids.size() != 0) {
        for(vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
          ostringstream ss;
          ++count;
          bool result = false;
          for(int i = 0; i < 3; i++) {
            try {
              result = reload(*it);
            } catch (Ice::Exception e) {
              MCE_WARN(_service << " " << *it << " Ice::Exception: " << e.what());
            } catch (std::exception e) {
              MCE_WARN(_service << " " << *it << " std::exception: " << e.what());
            }
            if(result) {
              break;
            } else {
              MCE_INFO("UserBaseThread Load Failed id = " << (*it) << ", time = " << i);
            }
          }
        }
      } else {
        break;
      }
      if(count % 1000 == 0) {
        MCE_INFO(_service << " 1000 done, count = " << count);
      }
    }
  }
  virtual bool reload(int id) = 0;
};

class UserConfigThread : public ConsumerThread {
public:
  UserConfigThread(Queue<int> *queue, int batch, string service = "UserConfig")
      : ConsumerThread(queue, batch, service) {}

  virtual bool reload(int id) {
    UserConfigAdapter::instance().getUserConfig(id);
    return true;
  }
};

class UserDescThread : public ConsumerThread {
public:
  UserDescThread(Queue<int> *queue, int batch, string service = "UserDesc")
      : ConsumerThread(queue, batch, service) {}

  virtual bool reload(int id) {
    UserDescReaderAdapter::instance().getUserDescN(id);
    return true;
  }
};

class UserUrlThread : public ConsumerThread {
public:
  UserUrlThread(Queue<int> *queue, int batch, string service = "UserUrl")
      : ConsumerThread(queue, batch, service) {}

  virtual bool reload(int id) {
    return UserUrlTripodAdapter::instance().Load(id);
  }
};

class UserStateThread : public ConsumerThread {
public:
  UserStateThread(Queue<int> *queue, int batch, string service = "UserState")
      : ConsumerThread(queue, batch, service) {}

  virtual bool reload(int id) {
    return UserStateTripodAdapter::instance().Load(id);
  }
};

class UserStateIceThread : public ConsumerThread {
public:
  UserStateIceThread(Queue<int> *queue, int batch, string service = "UserStateIce")
      : ConsumerThread(queue, batch, service) {}

  virtual bool reload(int id) {
    UserStateAdapter::instance().reload(id);
    return true;
  }
};

class UserPassportIceThread : public ConsumerThread {
public:
  UserPassportIceThread(Queue<int> *queue, int batch, string service = "UserPassportIce")
      : ConsumerThread(queue, batch, service) {}

  virtual bool reload(int id) {
    UserPassportAdapter::instance().reload(id);
    //int safeStatus = UserPassportAdapter::instance().getUserPassport(id)->safestatus();
    //if(safeStatus == 7) {
    //  MCE_WARN("error id=" << id << ", safeStatus=" << safeStatus); 
    //}
    return true;
  }
};

class UserLoginCountThread : public ConsumerThread {
public:
  UserLoginCountThread(Queue<int> *queue, int batch, string service = "UserLoginCount")
      : ConsumerThread(queue, batch, service) {}

  virtual bool reload(int id) {
    return UserLoginCountTripodAdapter::instance().Load(id);
  }
};
/* 生产者，从标准输入读取ID */
class ProducerThread : public IceUtil::Thread {
Queue<int> *_queue;
int _band;
const unsigned _batch;
public:
  ProducerThread(Queue<int> *queue, int band, int batch)
      : _queue(queue), _band(band), _batch(batch) {}
  
  virtual void run() {
    int id;
    vector<int> ids;
    int band_count = _band;
    while(cin >> id) {
        ids.push_back(id);
        band_count--;
      if(ids.size() < _batch) {
        continue;
      } else {
        (*_queue).put(ids);
        ids.clear();
        if(band_count < 0) {
          sleep(1);
          band_count = _band;
        }
      }
    }
    (*_queue).put(ids);
    ids.clear();
    (*_queue).finish();
  }
};

void useage(string filename) {
  cout << "用法:" << endl;
  cout << filename << " -op arg [...]" << endl;
  cout << "b:  吞吐量，每秒钟处理ID的上限" << endl;
  cout << "t:  消费者线程数" << endl;
  cout << "c:  消费者每次加入到队列中的id个数" << endl;
  cout << "p:  生产者每次加入到队列中的id个数" << endl;
  cout << "m:  队列最小长度，低于这个值唤醒ID生产者线程" << endl;
  cout << "M:  队列最大长度，高于这个值暂停加载" << endl;
  cout << endl;
  cout << "例子:" << endl;
  cout << filename << " -s UserUrl -b 10000 -t 20 >log.output <user_all_id" << endl;
}

int main(int argc, char ** argv){
  const string UB("UserBase");
  const string UC("UserConfig");
  const string UD("UserDesc");
  const string UR("UserUrl");
  const string US("UserState");
  const string UL("UserLoginCount");
  const string USI("UserStateIce");
  const string UPI("UserPassportIce");


  /* default arguments */
  string service = "";
  unsigned band = 1000;
  unsigned consumer_thread = 20;
  unsigned consumer_batch = 100;
  unsigned producer_batch = 1000;
  unsigned queue_min = 3000;
  unsigned queue_max = 10000;

  MCE_DEFAULT_INIT("DEBUG");
  if(argc == 1) {
    useage(argv[0]);
    return 0;
  }

  /* process arguments */
  char c;
  while (-1 != (c = getopt(argc, argv,
    "s:"  /* 服务名 */
    "b:"  /* 吞吐量，每秒钟处理ID的上限 */
    "t:"  /* 消费者线程数 */
    "c:"  /* 消费者每次加入到队列中的id个数 */
    "p:"  /* 生产者每次加入到队列中的id个数 */
    "m:"  /* 队列最小长度，低于这个值唤醒ID生产者线程 */
    "M:"  /* 队列最大长度，高于这个值暂停加载 */
  ))) {
    switch (c) {
    case 's':
      service = optarg;
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

  Queue<int> mainQueue(queue_min, queue_max);
  
  /* 一个生产者线程，多个消费者线程 */
  vector<IceUtil::ThreadControl> threads;
  if(service == UC){
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = new UserConfigThread(&mainQueue, consumer_batch);
      threads.push_back(t->start());
    }
  } else if(service == UD){
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = new UserDescThread(&mainQueue, consumer_batch);
      threads.push_back(t->start());
    }
  } else if(service == UR){
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = new UserUrlThread(&mainQueue, consumer_batch);
      threads.push_back(t->start());
    }
  } else if(service == US){
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = new UserStateThread(&mainQueue, consumer_batch);
      threads.push_back(t->start());
    }
  } else if(service == UL){
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = new UserLoginCountThread(&mainQueue, consumer_batch);
      threads.push_back(t->start());
    }
  } else if(service == USI){
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = new UserStateIceThread(&mainQueue, consumer_batch);
      threads.push_back(t->start());
    }
  } else if(service == UPI){
    for (unsigned i = 0; i < consumer_thread; ++i) {
      IceUtil::ThreadPtr t = new UserPassportIceThread(&mainQueue, consumer_batch);
      threads.push_back(t->start());
    }
  } else {
    cout << "Available services are:" << endl;
    cout << UB << ", " << UC << ", " << UD  << ", " << UR << endl;
    return 0;
  }
  IceUtil::ThreadPtr t = new ProducerThread(&mainQueue, band, producer_batch);
  threads.push_back(t->start());

  for (vector<IceUtil::ThreadControl>::iterator it = threads.begin(); it != threads.end(); ++it) {
    it->join();
  }

  MCE_WARN("Done!");
  return 0;
}
