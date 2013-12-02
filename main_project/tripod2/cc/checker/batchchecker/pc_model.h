#ifndef XCE_UTIL_PC_MODEL_H_
#define XCE_UTIL_PC_MODEL_H_

#include <list>
#include <vector>

#include "boost/shared_ptr.hpp"
#include "IceUtil/Thread.h"
#include "IceUtil/Monitor.h"

#include "util/cpp/TimeCost.h"

namespace xce {
namespace util {
using namespace std;

/**
 * 多线程读写队列
 */
template<class T>
class Queue : public IceUtil::Monitor<IceUtil::Mutex> {
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

private:
  list<T> _q;
  short _waitingReaders;
  bool _finished;
  bool _isFull;
  const unsigned _min, _max;
};

/** 
 * 消费者
 * 从队列获取ID并消费
 * 每次取batch个ID消费
 * 取到id数为0的时候退出线程
 */
template<class T>
class ConsumerThread : public IceUtil::Thread {
public:
  ConsumerThread<T>(Queue<T> *queue, int batch)
      : _queue(queue), _batch(batch) {
  }

  virtual void run() {
    while(true) {
      vector<T> ids = (*_queue).get(_batch);
      if(ids.size() != 0) {
        bool result = consum(ids);
      } else {
        break;
      }
    }
  }

protected:
  Queue<T> *_queue;
  int _batch;
  virtual bool consum(vector<T> ids) = 0;
};

/** 
 * 生产者
 *
 * 每秒钟生产上限为band，超过则sleep等待
 * 每次生产batch个id，放入队列中
 * 当连续多次生产的id为0的时候，停止生产并通知队列
 */ 
template<class T>
class ProducerThread : public IceUtil::Thread {
public:
  ProducerThread(Queue<T> *queue, int band, int batch)
      : _queue(queue), _band(band), _batch(batch) {}
  
  virtual void run() {
    int band_count = _band;
    MyUtil::Clock clock;
    int fail_count = 0;
    while(true) {
      vector<T> ids;
      produce(&ids, _batch);
      if(ids.size()) {
        fail_count = 0;
        band_count -= ids.size();
        (*_queue).put(ids);
        if(band_count <= 0) {
          long time_cost = clock.total();
          if(time_cost < 1000000) {
            usleep(1000000 - time_cost);
          }
          band_count = _band;
          clock.restart();
        }
      } else {
        fail_count ++;
        if(fail_count > 3) {
          break;
        }
      }
    }
    (*_queue).finish();
  }

protected:
  Queue<T> *_queue;
  int _band;
  const unsigned _batch;
  virtual void produce(vector<T> *ids, int batch) = 0;
};

}} //namespace xce::util

#endif
