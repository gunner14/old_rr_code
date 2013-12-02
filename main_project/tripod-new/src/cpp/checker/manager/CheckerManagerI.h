#ifndef __COM_RENREN_TRIPOD_CHECKERMANAGERI_H__
#define __COM_RENREN_TRIPOD_CHECKERMANAGERI_H__

#include "TripodChecker.h"
#include "../share/MutiThread.h"
#include "tripod-new/src/cpp/producermanager/TripodProducerFactory.h"
#include <Ice/Ice.h>
#include <IceUtil/Handle.h>
#include <IceUtil/Mutex.h>
#include "TaskManager.h"
#include "ServiceI.h"

namespace com {
namespace renren {
namespace tripod {

using com::renren::tripod::Queue;

typedef Queue<string> StrQueue;
class KeyProducerThread;
typedef IceUtil::Handle<KeyProducerThread> KeyProducerThreadPtr;
class ProducerServant;
typedef IceUtil::Handle<ProducerServant> ProducerServantPtr;
typedef map<string, ProducerServantPtr> ServantMap;

//CheckerManagerI
//--------------------------------------------------------------
class CheckerManagerI: virtual public CheckerManager, public MyUtil::Singleton<CheckerManagerI> {
public:
	CheckerManagerI();
	virtual ~CheckerManagerI();

  KeyMsgPtr getKey(const string& businessId, const int batch,
      const Ice::Current& current);
  void restart(const string& businessId, const string& namespaceId, const string& zkAddress, 
      const Ice::Current& current);
  void statis(const string& businessId, const StatisDataPtr & data,
      const Ice::Current& current);

  bool isValid(const Ice::Current& current);
  void setValid(bool value, const Ice::Current& current);

private:
  ServantMap _servantMap;
  TripodProducerFactory _factory;
};

class RestartTask : public MyUtil::Task {
public:
  RestartTask(const ProducerServantPtr& servant);

private:
  void handle();

private:
  ProducerServantPtr _servant;
};

class KeyProducerThread : public RecycleThread{
public:
  //激活循环进程，并进入等待状态，由restart()开始进程
  KeyProducerThread(TripodProducerPtr &producer, StrQueue *queue, int batch = 1000) :
      _producer(producer), _queue(queue), _batch(batch), _beginKey("") {
  }

  void restart() {
    _beginKey = "";
    wakeup();
  }

protected:
  bool recycleRun() {
    vector<string> items = _producer->createKeys(_beginKey, _batch);
    MCE_WARN("[KeyProducerThread] produce : beginKey=" << _beginKey << ", size=" << items.size());
    //最后一个key多重复一次，保证兼容性
    if(!items.empty()) {
      _beginKey = items.back();
      //结束生产，返回空
      MCE_WARN("[KeyProducerThread] produce : compare")
      if(items.size() == 1 && items.back() == _beginKey) {
        MCE_WARN("[KeyProducerThread] produce : end")
        vector<string> itemNull;
        items.swap(itemNull);
      }
    }

    if(items.size()) {
      _queue->put(items);
    }

    return items.size();
  }

private:
  TripodProducerPtr _producer;
  StrQueue *_queue;
  int _batch;
  string _beginKey;
};

/**
 * @description Key生产服务
 * 保存businessId, namespaceId, zkAddress是为了让CheckerWroker的配置更简洁
 *
 */
class ProducerServant : virtual public ::Ice::Object {
public:
  ProducerServant(const string& businessId, const string& namespaceId, const string& zkAddress,
      const int minQSize = 5000, const int maxQSize = 10000, const int batch = 1000, int pSize = 1);
  KeyMsgPtr getKey(const int batch);
  void restart();

private:
  StrQueue *_queue;
  KeyProducerThreadPtr _producer; 
  const string _businessId;
  const string _namespaceId;
  const string _zkAddress;
  bool _isActive;
  string _beginKey;
  IceUtil::Monitor<IceUtil::Mutex> _mutex;

};


}}} //com::renren::tripod

#endif

