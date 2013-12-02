#ifndef MUTITHREAD_H
#define MUTITHREAD_H

#include <iostream>
#include <vector>
#include <list>
#include <IceUtil/Thread.h>
#include <IceUtil/Monitor.h>
#include <LogWrapper.h>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace MyUtil;

namespace com {
namespace renren {
namespace tripod {

/**
 * @description 多线程读写队列，线程安全。
 */
template<class T>
class Queue : public IceUtil::Monitor<IceUtil::Mutex> {
  list<T> _queue; //读写队列
  int _waitingReaders; //等待读取队列的数量
  bool _isFull; //队列是否已满
  bool _isProducing; //是否有生产者
  const unsigned _min; //队列下限，低于此开始生产
  const unsigned _max; //队列上限，高于此停止生产
public:
  Queue(int min = 5000, int max = 10000) : _min(min), _max(max) {
    init();
  }

  /**
   * @description 初始化，每次重新使用队列的时候调用
   */
  void init() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    //_waitingReaders = 0;
    _isProducing = true;
    _isFull = false;
    if(_queue.size()) {
      list<T> queue;
      _queue.swap(queue);
    }
  }

  int size() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    return _queue.size();
  }

  bool isRunning() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    return _isProducing || _queue.size();
  }

  void finish() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    _isProducing = false;
    notifyAll();
  }

  /**
   * @description 把元素写入队列，当队列已满时阻塞。
   */
  void put(const vector<T> &items) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    if(_isFull) {
      notifyAll();
      wait();
    }
    _queue.insert(_queue.end(), items.begin(), items.end());
    if(_queue.size() > _max) {
      _isFull = true;
    }
    if(_waitingReaders) {
      notify();
    }
  }

  bool timedGet(vector<T> &seq, int batchSize) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    if(_queue.size() == 0) {
      try {
        ++_waitingReaders;
        timedWait(IceUtil::Time::microSeconds(200));
        --_waitingReaders;
      } catch (...) {
        --_waitingReaders;
        throw;
      }
    } 

    while(_queue.size() && batchSize--) {
      seq.push_back(_queue.front());
      _queue.pop_front();
    }
    if(_queue.size() < _min) {
      _isFull = false;
      notify();
    }

    return (_queue.size() == 0);
  }

  /**
   * @description 从队列中读取元素
   * @return 返回是否队列是否可以结束读取
   * @param seq 读取的结果存储的引用
   * @param batchSize 批量读取的大小
   */
  bool get(vector<T> &seq, int batchSize) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    while(_queue.size() == 0 && _isProducing) {
      try {
        ++_waitingReaders;
        wait();
        --_waitingReaders;
      } catch (...) {
        --_waitingReaders;
        throw;
      }
    } 

    while(_queue.size() && batchSize--) {
      seq.push_back(_queue.front());
      _queue.pop_front();
    }
    if(_queue.size() < _min) {
      _isFull = false;
      notify();
    }

    return (_queue.size() == 0);
  }

};

/**
 ** @description 用于生产者消费者同步
 **/
class PCSyncData : public ::Ice::Object {
public:
  PCSyncData() : _comsumersCount(0), _producersCount(0) {
  }

  void init() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    _comsumersCount = 0;
    _producersCount = 0;
  }

  void waitForAllFinish() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    while(_producersCount || _comsumersCount) {
      _mutex.wait();
    }
  }

  void waitForProduceFinish() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    while(_producersCount) {
      _mutex.wait();
    }
  }

  void waitForConsumerFinish() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    while(_comsumersCount) {
      _mutex.wait();
    }
  }
  bool _isProducing() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    return _producersCount;
  }

  bool _isConsuming() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    return _comsumersCount;
  }

  void producerAdd() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    _producersCount--;
    _mutex.notifyAll();
  }

  void producerSub() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    _producersCount++;
    _mutex.notifyAll();
  }

  void consumerAdd() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    _comsumersCount--;
    _mutex.notifyAll();
  }

  void consumerSub() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    _comsumersCount++;
    _mutex.notifyAll();
  }

private:
  int _comsumersCount;
  int _producersCount;
  IceUtil::Monitor<IceUtil::Mutex> _mutex;
};

typedef boost::shared_ptr<PCSyncData> PCSyncDataPtr;

/**
 * 循环线程，可由外部控制开始和停止
 * 留有开始后和停止后的虚接口供继承类实现
 * 初始自动启动并进入就绪状态，有外部唤醒
 */
class RecycleThread : public IceUtil::Thread {
  bool _isActive;
  PCSyncDataPtr _syncData;
  IceUtil::Monitor<IceUtil::Mutex> _mutex;
public:
  RecycleThread() : _isActive(false) {
    start();
  }

  void wakeup() {
    MCE_DEBUG("RecycleThread wakeup");
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    if(!_isActive) {
      _isActive = true;
      wakeupDo();
      _mutex.notify();
    }
  }

  void sleep() {
    MCE_DEBUG("RecycleThread sleep");
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    if(_isActive) {
      _isActive = false;
      sleepDo();
    }
  }

  bool isActive() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    return _isActive;
  }

protected:
  /**
   * @description 线程停止运行后执行的操作
   */
  virtual void wakeupDo() {}

  /**
   * @description 线程重新唤醒后执行的操作
   */
  virtual void sleepDo() {}

  /**
   * @description 循环子模块
   * @return 是否继续下一次循环
   */
  virtual bool recycleRun() = 0;

  virtual void run() {
    while(true) {
      {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
        if(!_isActive) {
          _mutex.wait();
        }
      }
      if(!recycleRun()) {
        sleep();      
      }
    }
  }
};

/* 消费者，从队列获取ID并Reload */
template <class T>
class ConsumerThread : public IceUtil::Thread {
  Queue<T> *_queue; //读写队列
  int _batch; //批量读取的元素大小
  bool _isActive;
  IceUtil::Monitor<IceUtil::Mutex> _mutex;
public:
  ConsumerThread(Queue<T> *queue, int batch) : _queue(queue), _batch(batch) {}
  virtual void run() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    if(!_isActive) {
      _mutex.wait();
    }
    while(true) {
      vector<T> items = (*_queue).get(_batch);
      if(items.size() != 0) {
        consume(items);
      } else {
        _isActive = false;
      }
    }
  }
  virtual void consume(vector<T> &items) = 0;
};

}}} //namespace com::renren::tripod
#endif
