#include "Journey/JourneyLogic.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <util/cpp/InvokeMonitor.h>
#include <Monitor/client/service/ServiceMonitor.h>
#include "boost/lexical_cast.hpp"
#include <bitset>


using namespace xce::journey;
using namespace MyUtil;
using namespace ::xce::serverstate;
using namespace std;
using namespace boost;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&JourneyLogicI::instance(), service.createIdentity("M", ""));
  
  TaskManager::instance().config(TASK_LEVEL_CREATE, MyUtil::ThreadPoolConfig(1, 1));

  int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("JourneyLogic.Mod", 0);
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("JourneyLogic.Interval",5);
  ServerStateSubscriber::instance().initialize("ControllerJourneyLogic",&JourneyLogicI::instance(),mod,interval);
  int status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("JourneyLogic.Status",0);
  ServerStateSubscriber::instance().setStatus(status);
  ServiceMonitorManager::instance().start();
}

void JourneyLogicI::invote(::Ice::Long key, ::Ice::Int guestId, ::Ice::Int limit, const Ice::Current& current) {
  xce::tempmutex::Lock lock(tempMutexManagerPtr_, key);

  ostringstream inMsg;
  inMsg << "JourneyLogicI::invote key " << key << " guestId :" << guestId << " limit :" << limit;
  InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  IceUtil::Monitor<IceUtil::Mutex> tempMonitor;
  bool tempFlag = false;
   
  TaskManager::instance().execute(new ListOpTask(key, guestId, limit, tempFlag, time_adapter_, tempMonitor));

  string incCount = JourneyLogicCountTripodAdapter::instance().Inc(lexical_cast<string>(key), "1");
  if(incCount == "" || incCount.size() == 0) {
     bool boolResult = false;
     boolResult =  JourneyLogicCountTripodAdapter::instance().Set(lexical_cast<string>(key), "1", kJourneyExpire);
  }

  IceUtil::Monitor<IceUtil::Mutex>::Lock list_lock(tempMonitor);
  while(tempFlag == false) 
    tempMonitor.wait();

  MCE_DEBUG("JourneyLogicI::invote done : key " << key << " guestId " << guestId << " limit " << limit << " operation completed!!!");
}

MyUtil::IntSeq JourneyLogicI::getList(::Ice::Long key, ::Ice::Int limit, const Ice::Current& current) {
  using namespace xce::tripod;

  ostringstream inMsg;
  inMsg << "JourneyLogicI::getList key " << key << " limit :" << limit;
  InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
  IntSeq result;
  StringListType resultStr;
  JourneyLogicListTripodAdapter::instance().GetSortedSet(lexical_cast<string>(key), &resultStr, 0, limit);
  for (StringListType::const_iterator iter = resultStr.begin(); iter != resultStr.end(); iter++) {
    MCE_DEBUG("JourneyLogicI::getList(::Ice::Long key) key " << key << " *iter " << *iter);
    result.push_back(lexical_cast<int>(*iter));
  }
  return result;
}

int JourneyLogicI::getCount(::Ice::Long key, const Ice::Current& current) {
  ostringstream inMsg;
  inMsg << "JourneyLogicI::getCount key " << key;
  InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
  int result = 0;
  string strResult;
  if (JourneyLogicCountTripodAdapter::instance().Get(lexical_cast<string>(key), &strResult)) {
    result = lexical_cast<int>(strResult);
  }
  return result;
}

void JourneyLogicI::reset(::Ice::Long key, const Ice::Current& current) {
  xce::tempmutex::Lock lock(tempMutexManagerPtr_, key);
  ostringstream inMsg;
  inMsg << "JourneyLogicI::reset key " << key;
  InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
  JourneyLogicCountTripodAdapter::instance().Set(lexical_cast<string>(key), "0", kJourneyExpire);
  JourneyLogicListTripodAdapter::instance().TrimSortedSet(lexical_cast<string>(key), 0, -1);
}

/*********************************************ListOpTask***************************************************************************/

ListOpTask::ListOpTask(long key, int guestId, int limit, bool& flag, MyUtil::Clock* time_adapter, 
    IceUtil::Monitor<IceUtil::Mutex>& monitor, int level) : 
    Task(level), key_(key), guest_id_(guestId), limit_(limit), action_flag_(flag), 
    time_adapter_(time_adapter), list_op_monitor_(monitor){
}

void ListOpTask::handle() {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(list_op_monitor_);
  action_flag_ = true;
  MCE_DEBUG("ListOpTask::handle() action : key " << key_ <<" guestId " << guest_id_ );
  
  map<string, string> set_with_scores;
  set_with_scores.insert(make_pair(lexical_cast<string>(-time_adapter_->get_now()), lexical_cast<string>(guest_id_)));
  bool addResult = JourneyLogicListTripodAdapter::instance().AddSortedSet(lexical_cast<string>(key_), set_with_scores, kJourneyExpire);
  if (JourneyLogicI::instance().getCount(key_) > limit_) {
    JourneyLogicListTripodAdapter::instance().TrimSortedSet(lexical_cast<string>(key_), limit_, -1);
  }

  MCE_DEBUG("ListOpTask::handle() end : key " << key_ <<" guestId " << guest_id_ );
  list_op_monitor_.notify();
}


