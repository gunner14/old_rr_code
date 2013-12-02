#ifndef __COM_RENREN_TRIPOD_CHECKERWORKERI_H__
#define __COM_RENREN_TRIPOD_CHECKERWORKERI_H__

#include "TripodChecker.h"
#include "../share/MutiThread.h"
#include "util/cpp/TimeCost.h"
#include "OceCxxAdapter/src/CheckerManagerAdapter.h"
#include "tripod-new/src/cpp/producermanager/TripodProducerFactory.h"
#include "tripod-new/src/cpp/include/TripodClient.h"
#include <Ice/Ice.h>
#include <IceUtil/Handle.h>
#include <IceUtil/Mutex.h>
#include <boost/shared_ptr.hpp>
#include "TaskManager.h"
#include "ServiceI.h"

namespace com {
namespace renren {
namespace tripod {

using com::renren::tripod::Queue;
using com::renren::tripod::StatisData;

using com::renren::tripod::KeySeq;
using com::renren::tripod::DataMap;
using com::renren::tripod::TripodClient;
using com::renren::tripod::TripodProducerPtr;
using com::renren::tripod::TripodProducerFactory;
using xce::adapter::checkermanager::CheckerManagerAdapter;

typedef Queue<string> StrQueue;
typedef boost::shared_ptr<StrQueue> StrQueuePtr;

class KeyProducerThread;
typedef IceUtil::Handle<KeyProducerThread> KeyProducerThreadPtr;
typedef std::vector<KeyProducerThreadPtr> KeyProducerThreadSeq;

class CheckerThread;
typedef IceUtil::Handle<CheckerThread> CheckerThreadPtr;
typedef std::vector<CheckerThreadPtr> CheckerThreadSeq;

class ConsumerServant;
typedef IceUtil::Handle<ConsumerServant> ConsumerServantPtr;
typedef map<string, ConsumerServantPtr> ServantMap;

class StatisDataI;
typedef IceUtil::Handle<StatisDataI> StatisDataIPtr;
class StatisDataI : virtual public StatisData {
public:
  StatisDataI(long checked = 0, long inconsistent = 0, long missed = 0, long invalid = 0, long time = 0) {
      checkedSize = checked;
      inconsistentSize = inconsistent;
      missedSize = missed;
      invalidSize = invalid;
      timeCost = time;
  }

  StatisDataI& operator+=(const StatisDataI &data) {
    checkedSize += data.checkedSize;
    inconsistentSize += data.inconsistentSize;
    missedSize += data.missedSize;
    invalidSize += data.invalidSize;
    timeCost += data.timeCost;
    return *this;
  }

  StatisDataPtr getData() {
    StatisDataPtr result = new StatisData;
    result->checkedSize = checkedSize;
    result->inconsistentSize = inconsistentSize;
    result->missedSize = missedSize;
    result->invalidSize = invalidSize;
    result->timeCost = timeCost; 
    return result;
  }

  void parse(StatisDataPtr data) {
    checkedSize = data->checkedSize;
    inconsistentSize = data->inconsistentSize;
    missedSize = data->missedSize;
    invalidSize = data->invalidSize;
    timeCost = data->timeCost;
  }
};

//CheckerWorkerI
//--------------------------------------------------------------
class CheckerWorkerI: virtual public CheckerWorker, public MyUtil::Singleton<CheckerWorkerI> {
public:
	CheckerWorkerI();
	virtual ~CheckerWorkerI();

  void start(const string& businessId, const string& namespaceId, const string& zkAddress, const Ice::Current& current);
  void stop(const string& businessId, const Ice::Current& current);

  bool isValid(const Ice::Current& current);
  void setValid(bool value, const Ice::Current& current);

private:
  ServantMap _servantMap;
  TripodProducerFactory _factory;
};

class RestartTask : public MyUtil::Task {
public:
  RestartTask(const ConsumerServantPtr& servant);

private:
  void handle();

private:
  ConsumerServantPtr _servant;
};

class KeyProducerThread : public RecycleThread {
public:
  KeyProducerThread(StrQueuePtr queue, PCSyncDataPtr syncData, const string& businessId, int batch = 1000) :
      _queue(queue), _syncData(syncData), _businessId(businessId), _batch(batch) {
  }

  bool recycleRun() {
    KeyMsgPtr keyMsg = CheckerManagerAdapter::instance().getKey(_businessId, _batch);
    MCE_WARN("KeyProducerThread get size=" << keyMsg->keys.size());
    bool finished = bool(keyMsg->flag == FINISHED);
    if(finished) {
      MCE_WARN("KeyProducerThread finished " << keyMsg->keys.size());
      _queue->put(keyMsg->keys);
      _queue->finish();
    } else {
      MCE_WARN("KeyProducerThread get size=" << keyMsg->keys.size());
      _queue->put(keyMsg->keys);
    }

    return !finished;
  }

  void wakeupDo() {
    if(_syncData.get()) {
      _syncData->producerAdd();
    }
  }

  void sleepDo() {
    if(_syncData.get()) {
      _syncData->producerSub();
    }
  }

private:
  const StrQueuePtr _queue;
  const PCSyncDataPtr _syncData;
  string _businessId;
  int _batch;
};

class CheckerThread : public RecycleThread {
public:
  CheckerThread(StrQueuePtr queue, PCSyncDataPtr syncData, const string& businessId, const string& namespaceId,
      const string& zkAddress, int batch = 100) :
      _queue(queue), _syncData(syncData), _businessId(businessId), _batch(batch) {
    TripodProducerFactory factory;
    _producer = factory.get(businessId);
    _tripodClient = new TripodClient(zkAddress, namespaceId, businessId);
    init();
  }

  bool recycleRun() {
    KeySeq keys;
    _queue->get(keys, _batch);
    MCE_WARN("CheckerThread check size=" << keys.size());
    if(keys.size()) {
      MCE_WARN("CheckerThread check beginKey=" << keys[0]);
      try {
        check(keys);
      } catch (exception e) {
        MCE_WARN("CheckerThread check exception " << e.what());
      }
    } else {
      MCE_WARN("CheckerThread finish");
    }

    return keys.size();
  }

  void wakeupDo() {
    init();
    if(_syncData.get()) {
      _syncData->consumerAdd();
    }
  }

  void sleepDo() {
    if(_syncData.get()) {
      _syncData->consumerSub();
    }
  }

  void init() {
    _statisData = new StatisDataI;
  }

  StatisDataIPtr statis() {
    return _statisData;
  }

  void check(KeySeq keys) {
    MyUtil::Clock clock;
    KeySeq missedKeys, inconsistentKeys, invalidKeys;
    DataMap dataDB, dataCache;
    try {
      dataDB = _producer->create(keys, false);
      dataCache = _tripodClient->get(keys, missedKeys);
      MCE_WARN("[TripodChecker] check : keysSize= " << keys.size() << ", beginKey=" << *keys.begin() 
          << ", dbSize=" << dataDB.size() << ", cacheSize=" << dataCache.size());
    } catch (std::exception e) {
      MCE_WARN("[TripodChecker] exception " << e.what())
      return;
    }
    
    MCE_DEBUG("[TripodChecker] load time = " << clock.step());

    //以DB为标准，对比Cache数据
    for(DataMap::const_iterator itDB = dataDB.begin(); itDB != dataDB.end(); ++itDB) {
      DataMap::const_iterator itCache = dataCache.find(itDB->first);
      if(itCache != dataCache.end()) {
        string valueDB(itDB->second.begin(), itDB->second.end());
        string valueCache(itCache->second.begin(), itCache->second.end());
        if(valueDB != valueCache){
          MCE_WARN("[TripodChecker] Inconsistent : wrong key = " << itDB->first);
          inconsistentKeys.push_back(itDB->first);
        }
      } else {
        MCE_WARN("[TripodChecker] Inconsistent : miss key = " << itDB->first);
        missedKeys.push_back(itDB->first);
      }
    }

    MCE_DEBUG("[TripodChecker] cache time = " << clock.step());

    //查找Cache中有DB中没有的key
    for(DataMap::const_iterator itCache = dataCache.begin(); itCache != dataCache.end(); ++itCache) {
      DataMap::const_iterator itDB = dataDB.find(itCache->first);
      if(itDB == dataDB.end()) {
        MCE_WARN("[TripodChecker] Inconsistent : invalid key = " << itCache->first);
        invalidKeys.push_back(itCache->first);
      }
    }

    MCE_DEBUG("[TripodChecker] DB time = " << clock.step());

    _statisData->checkedSize += keys.size();
    _statisData->inconsistentSize += inconsistentKeys.size();
    _statisData->missedSize += missedKeys.size();
    _statisData->invalidSize += invalidKeys.size();
    _statisData->timeCost += clock.total(); 
    _tripodClient->load(inconsistentKeys, 0, false);
    _tripodClient->load(missedKeys, 0, false);
    _tripodClient->deleteCache(invalidKeys, 1000);
  }
  
private:
  const StrQueuePtr _queue;
  const PCSyncDataPtr _syncData;
  StatisDataIPtr _statisData;
  TripodProducerPtr _producer;
  TripodClient *_tripodClient;
  string _businessId;
  int _batch;
};

class ConsumerServant : virtual public ::Ice::Object, public ::IceUtil::Thread {
public:
  ConsumerServant(const string& businessId, const string& namespaceId, const string& zkAddress,
      const int minQSize = 5000, const int maxQSize = 10000, const int pBatch = 1000,
      const int cBatch = 100, const int pSize = 2, const int cSize = 10);
  void restart();
  void statis();

private:
  void run() {
    MCE_WARN("ConsumerServant " << _businessId << " start!");
    while(true) {
      {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
        while(!_isActive){
          _mutex.wait();
          MCE_INFO("ConsumerServant " << _businessId << " active");
        }
      }
      MCE_INFO("ConsumerServant " << _businessId << " waiting for all finish");
      _syncData->waitForAllFinish();
      {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
        if(_isActive) {
          _isActive = false;
          MCE_INFO("ConsumerServant " << _businessId << " all Finished");
          statis();
        }
      }
    } 
  }

private:
  StrQueuePtr _queue;
  PCSyncDataPtr _syncData;
  CheckerThreadSeq _consumers; 
  KeyProducerThreadSeq _producers; 
  bool _isActive;
  IceUtil::Monitor<IceUtil::Mutex> _mutex;
  const string _businessId;
};

}}} //com::renren::tripod

#endif

