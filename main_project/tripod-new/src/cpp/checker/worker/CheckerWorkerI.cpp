#include "CheckerWorkerI.h"
#include "LogWrapper.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/TimeCost.h"
#include "util/cpp/InvokeMonitor.h"

using namespace MyUtil;
using namespace com::renren::tripod;
using namespace ::xce::serverstate;

//初始化
//--------------------------------------------------------------
void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();

  service.getAdapter()->add(&CheckerWorkerI::instance(), service.createIdentity("CW", ""));

  //HA
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("CheckerWorker.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("CheckerWorker.Interval", 5);
  ServerStateSubscriber::instance().initialize("ControllerCheckerWorker", &CheckerWorkerI::instance(), mod, interval);
  {
    int status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("CheckerWorker.Status", 1);
    ServerStateSubscriber::instance().setStatus(status);
    std::ostringstream os;
    os << "MyUtil::initialize set status : " << status;
    MCE_WARN( os.str() );
  }
  ServiceMonitorManager::instance().start();
}

//CheckerWorkerI
//--------------------------------------------------------------
CheckerWorkerI::CheckerWorkerI() {
  MCE_DEBUG("CheckerWorkerI::CheckerWorkerI");
  //try {
  //  string businessId = "UserPassport";
  //  string namespaceId = "test";
  //  string zkAddress = "xcetest3:2181/Tripod";

  //  start(businessId, namespaceId, zkAddress, Ice::Current());
  //} catch (exception e) {
  //  MCE_DEBUG("CheckerWorkerI::CheckerWorkerI exception " << e.what());
  //}
}

CheckerWorkerI::~CheckerWorkerI() {
}

void CheckerWorkerI::start(const string& businessId, const string& namespaceId, const string& zkAddress,
    const Ice::Current& current) {
  ostringstream oss;
  oss << "businessId=" << businessId;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_WARN);
 
  ServantMap::iterator it = _servantMap.find(businessId);
  if(it != _servantMap.end()) {
    MCE_INFO("CheckerWorkerI::start Restart businessId : " << businessId);
    MyUtil::TaskManager::instance().execute(new RestartTask(it->second));
  } else {
    TripodProducerPtr producer = _factory.get(businessId);
    if(producer.get() != 0) {
      MCE_INFO("CheckerWorkerI::start New businessId : " << businessId);
      _servantMap[businessId] = new ConsumerServant(businessId, namespaceId, zkAddress);
      MyUtil::TaskManager::instance().execute(new RestartTask(_servantMap[businessId]));
    } else {
      MCE_WARN("CheckerWorkerI::start No such businessId : " << businessId);
    }
  }
}

void CheckerWorkerI::stop(const string& businessId, const Ice::Current& current) {
  MCE_WARN("CheckerWorkerI::stop");
  ostringstream oss;
  oss << "businessId=" << businessId;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_WARN);
}

bool CheckerWorkerI::isValid(const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);

  return ServerStateSubscriber::instance().getStatus();
}

void CheckerWorkerI::setValid(bool valid, const Ice::Current& current) {
  ostringstream oss;
  oss << "value=" << valid;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  ServerStateSubscriber::instance().setStatus((int) valid);
}

RestartTask::RestartTask(const ConsumerServantPtr& servant) : _servant(servant) {
}

void RestartTask::handle() { 
  _servant->restart();
}

ConsumerServant::ConsumerServant(const string& businessId, const string& namespaceId,
      const string& zkAddress, const int minQSize, const int maxQSize,
      const int pBatch, const int cBatch, const int pSize, const int cSize) : _businessId(businessId) {
  MCE_DEBUG("[ConsumerServant::ConsumerServant] minQSize="
    << minQSize << ", maxQSize=" << maxQSize << ", batch=" << pBatch << ", pSize=" << pSize);
  _queue = StrQueuePtr(new StrQueue(minQSize, maxQSize));
  _syncData = PCSyncDataPtr(new PCSyncData());
  MCE_DEBUG("[ConsumerServant] queue init");
  for(int i = 0; i < pSize; i++) {
    KeyProducerThreadPtr producer = new KeyProducerThread(_queue, _syncData, businessId, pBatch);
    _producers.push_back(producer);
  }

  for(int i = 0; i < cSize; i++) {
    CheckerThreadPtr consumer = new CheckerThread(_queue, _syncData, businessId, namespaceId, zkAddress, cBatch);
    _consumers.push_back(consumer);
  }
  _isActive = false;
  this->start();
}

void ConsumerServant::statis() {
  StatisDataIPtr dataAll = new StatisDataI;
  for(CheckerThreadSeq::iterator it = _consumers.begin(); it != _consumers.end(); ++it) {
    (*dataAll) += *((*it)->statis());
  }
  MCE_WARN("ConsumerServant::statis checkedSize=" << dataAll->checkedSize << ", inconsistentSize=" 
      << dataAll->inconsistentSize << ", missedSize=" << dataAll->missedSize << ", invalidSize="
      << dataAll->invalidSize << "timeCost=" << dataAll->timeCost);
  CheckerManagerAdapter::instance().statis(_businessId, dataAll->getData());
}

void ConsumerServant::restart() {
  {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    if(_isActive) {
      for(KeyProducerThreadSeq::iterator it = _producers.begin(); it != _producers.end(); ++it) {
        (*it)->sleep();
      }
      for(CheckerThreadSeq::iterator it = _consumers.begin(); it != _consumers.end(); ++it) {
        (*it)->sleep();
      }
    }
  }
  _queue->init();
  for(KeyProducerThreadSeq::iterator it = _producers.begin(); it != _producers.end(); ++it) {
    (*it)->wakeup();
  }
  for(CheckerThreadSeq::iterator it = _consumers.begin(); it != _consumers.end(); ++it) {
    (*it)->wakeup();
  }
  {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    _isActive = true;
    _mutex.notify();
  }
}
