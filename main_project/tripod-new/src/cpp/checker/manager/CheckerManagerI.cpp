#include "CheckerManagerI.h"
#include "CheckerWorkerAdapter.h"
#include "LogWrapper.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/TimeCost.h"
#include "util/cpp/InvokeMonitor.h"

using namespace MyUtil;
using namespace com::renren::tripod;
using namespace ::xce::serverstate;
using namespace xce::adapter::checkerworker;

//初始化
//--------------------------------------------------------------
void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();

  service.getAdapter()->add(&CheckerManagerI::instance(), service.createIdentity("CM", ""));

  //HA
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("CheckerManager.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("CheckerManager.Interval", 5);
  ServerStateSubscriber::instance().initialize("ControllerCheckerManager", &CheckerManagerI::instance(), mod, interval);
  {
    int status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("CheckerManager.Status", 1);
    ServerStateSubscriber::instance().setStatus(status);
    std::ostringstream os;
    os << "MyUtil::initialize set status : " << status;
    MCE_WARN( os.str() );
  }
  string path = service.getCommunicator()->getProperties()->getPropertyWithDefault(
      "Service." + service.getName() + ".LogPath", "../log/" + service.getName());
  LOG_INIT("ST", path + "/statis_log", "INFO");
  LOG_WARN("ST", "statis log init");
  ServiceMonitorManager::instance().start();
}

//CheckerManagerI
//--------------------------------------------------------------
CheckerManagerI::CheckerManagerI() {
  MCE_DEBUG("CheckerManagerI::CheckerManagerI");
}

CheckerManagerI::~CheckerManagerI() {
}

void CheckerManagerI::restart(const string& businessId, const string& namespaceId,
    const string& zkAddress, const Ice::Current& current) {
  ostringstream oss;
  oss << "businessId=" << businessId;
  MCE_WARN("CheckerManagerI::restart " << businessId);
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_WARN);
  ServantMap::iterator it = _servantMap.find(businessId);
  if(it != _servantMap.end()) {
    MCE_WARN("CheckerManagerI::restart new business : " << businessId);
    MyUtil::TaskManager::instance().execute(new RestartTask(it->second));
  } else {
    TripodProducerPtr producer = _factory.get(businessId);
    if(producer.get() != 0) {
      _servantMap[businessId] = new ProducerServant(businessId, namespaceId, zkAddress);
      MyUtil::TaskManager::instance().execute(new RestartTask(_servantMap[businessId]));
    } else {
      MCE_WARN("CheckerManagerI::restart No such businessId");
    }
  }
}

void CheckerManagerI::statis(const string& businessId, const StatisDataPtr & data,
    const Ice::Current& current) {
  ostringstream oss;
  oss << "businessId = " << businessId;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  LOG_WARN("ST", oss.str() << " checkedSize = " << data->checkedSize << " inconsistentSize = " 
           << data->inconsistentSize << " missedSize = " << data->missedSize << " invalidSize = "
           << data->invalidSize << " timeCost = " << data->timeCost
  );
}

KeyMsgPtr CheckerManagerI::getKey(const string& businessId, const int batch,
    const Ice::Current& current) {
  ostringstream oss;
  oss << "businessId=" << businessId << ", batch=" << batch;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  ServantMap::iterator it = _servantMap.find(businessId);
  if(it != _servantMap.end()) {
    return it->second->getKey(batch);
  } else {
    MCE_WARN("No such businessId : " << businessId);
    KeyMsgPtr result = new KeyMsg;
    return result;
  }

}

bool CheckerManagerI::isValid(const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);

  return ServerStateSubscriber::instance().getStatus();
}

void CheckerManagerI::setValid(bool valid, const Ice::Current& current) {
  ostringstream oss;
  oss << "value=" << valid;
InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

  ServerStateSubscriber::instance().setStatus((int) valid);
}

RestartTask::RestartTask(const ProducerServantPtr& servant) : _servant(servant) {
}

void RestartTask::handle() { 
  _servant->restart();
}

ProducerServant::ProducerServant(const string& businessId, const string& namespaceId,
    const string& zkAddress, const int minQSize, const int maxQSize, const int batch, const int pSize):
    _businessId(businessId), _namespaceId(namespaceId), _zkAddress(zkAddress) {
  MCE_DEBUG("[ProducerServant::ProducerServant] businessId=" << businessId << ", namespaceId="
      << namespaceId << ", zkAddress=" << zkAddress << ", minQSize=" << minQSize << ", maxQSize="
      << maxQSize << ", batch=" << batch << ", pSize=" << pSize);
  _queue = new StrQueue(minQSize, maxQSize);
  TripodProducerFactory factory;
  TripodProducerPtr tripodProducer = factory.get(businessId);
  _producer = new KeyProducerThread(tripodProducer, _queue, batch);
}

KeyMsgPtr ProducerServant::getKey(const int batch) {
  KeyMsgPtr result = new KeyMsg;
  bool isActive;
  {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
    isActive = _isActive;
  }
  if(isActive) {
    _queue->timedGet(result->keys, batch);
    result->flag = (_producer->isActive() || result->keys.size()) ? NORMAL : FINISHED;
  } else {
    result->flag = FINISHED;
    KeySeq keys;
    result->keys = keys;
  }
  return result;
}

void ProducerServant::restart() {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
  _isActive = false;
  _queue->init();
  _producer->restart();
  _isActive = true;
  CheckerWorkerAdapter::instance().start(_businessId, _namespaceId, _zkAddress);
}
