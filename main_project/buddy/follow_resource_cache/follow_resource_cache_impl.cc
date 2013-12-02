#include "follow_resource_cache_impl.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <algorithm>
#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "util/cpp/InvokeMonitor.h"

#define __FUNCTION_NAME__ "[" << __FUNCTION__ << "]"

using MyUtil::ServiceI;
using MyUtil::TaskManager;
using MyUtil::ThreadPoolConfig;
using MyUtil::InvokeClient;
using xce::buddy::IdSeqDataFactory;
using xce::buddy::FollowResourceCacheImpl;
using xce::buddy::ResourceCacheDataCategory;

int main() {
  return 0;
}

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();

  // 注册ResourceCacheData的ObjectCache
  service.registerObjectCache(ResourceCacheDataCategory, "FRC", new SyncServantFactory(new IdSeqDataFactory));

  service.getAdapter()->add(&FollowResourceCacheImpl::instance(), service.createIdentity("FRCID", ""));
  //
  int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FollowResourceCache.Mod", 0);
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(
      "FollowResourceCache.Interval", 5);

  //----------------------------Monitor-----------------------------------------
  MyUtil::ServiceMonitorManager::instance().start();

  TaskManager::instance().config(xce::buddy::TASK_LEVEL_LOAD, ThreadPoolConfig(1, 100));
  TaskManager::instance().config(xce::buddy::TASK_LEVEL_RELOAD, ThreadPoolConfig(1, 100));

  service.registerXceObserver(new xce::buddy::ServiceSchemaI);
  // register to Controller Service
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerFollowResourceCache",
      &FollowResourceCacheImpl::instance(), mod, interval);
}

//=========================================================

namespace xce {
namespace buddy {

void FollowResourceCacheImpl::reload(const int userId, const int type, const Ice::Current& current) {
  ostringstream msg;
  msg << " userId:" << userId << " type:" << type;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  if (userId < 0 || type < 0) {
    MCE_WARN(__FUNCTION_NAME__ << " Invalid parameters! userId:" << userId << " type:" << type);
    return;
  }

  TaskManager::instance().execute(new ReloadTask(userId, type));
}

int FollowResourceCacheImpl::getCount(const int userId, const int type, const Ice::Current& current) {
  ostringstream msg;
  msg << " userId:" << userId << " type:" << type;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  if (userId < 0 || type < 0) {
    MCE_WARN(__FUNCTION_NAME__ << " Invalid parameters! userId:" << userId << " type:" << type);
    return 0;
  }

  long guid = FollowResourceCacheImpl::getGuid(userId, type);
  int count = 0;

  {
    xce::tempmutex::Lock lock(tempMutexManagerPtr_, guid);
    IdSeqDataPtr rdata = ServiceI::instance().locateObject<IdSeqDataPtr> (ResourceCacheDataCategory, guid);
    count = rdata->getCount();
  }

  MCE_DEBUG(__FUNCTION_NAME__ << " return count:" << count);
  return count;
}

MyUtil::LongSeq FollowResourceCacheImpl::getList(const int userId, const int type, const int begin, const int limit,
    const Ice::Current& current) {
  ostringstream msg;
  msg << " userId:" << userId << " type:" << type << " begin:" << begin << " limit:" << limit;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  MyUtil::LongSeq seq;

  if (userId < 0 || type < 0 || begin < 0 || limit < -1 || limit == 0) {
    MCE_WARN(__FUNCTION_NAME__ << " Invalid parameters! userId:" << userId << " type:" << type << " begin:" << begin
        << " limit:" << limit);
    return seq;
  }

  long guid = FollowResourceCacheImpl::getGuid(userId, type);

  {
    xce::tempmutex::Lock lock(tempMutexManagerPtr_, guid);
    IdSeqDataPtr rdata = ServiceI::instance().locateObject<IdSeqDataPtr> (ResourceCacheDataCategory, guid);
    seq = rdata->get(begin, limit);
  }

  MCE_DEBUG(__FUNCTION_NAME__ << " return seq.size():" << seq.size());
  return seq;
}

MyUtil::BoolSeq FollowResourceCacheImpl::hasRelationBatch(const int userId, const int type,
    const MyUtil::IntSeq& idSeq, const Ice::Current& current) {
  ostringstream msg;
  msg << " userId:" << userId << " type:" << type << " idSeq.size:" << idSeq.size();
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  MyUtil::BoolSeq seq;

  if (userId < 0 || type < 0 || idSeq.size() < 1) {
    MCE_WARN(__FUNCTION_NAME__ << " Invalid parameters! userId:" << userId << " type:" << type << " idSeq.size:"
        << idSeq.size());
    return seq;
  }

  long guid = FollowResourceCacheImpl::getGuid(userId, type);

  {
    xce::tempmutex::Lock lock(tempMutexManagerPtr_, guid);
    IdSeqDataPtr rdata = ServiceI::instance().locateObject<IdSeqDataPtr> (ResourceCacheDataCategory, guid);
    for (MyUtil::IntSeq::const_iterator it = idSeq.begin(); it != idSeq.end(); ++it) {
      seq.push_back(rdata->find(*it));
    }
  }

  MCE_DEBUG(__FUNCTION_NAME__ << " return seq.size():" << seq.size());
  return seq;
}

void ServiceSchemaI::configure(const Ice::PropertiesPtr& props) {
  //...
  TaskManager::instance().config(xce::buddy::TASK_LEVEL_LOAD, ThreadPoolConfig(1, 5));
  TaskManager::instance().config(xce::buddy::TASK_LEVEL_RELOAD, ThreadPoolConfig(1, 5));
}

}
} // end xce::buddy

