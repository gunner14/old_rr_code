#include "MonitorDispatcherI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <MonitorAdapter.h>

using Ice::Current;
using monitor::MonitorDispatcherI;
using xce::serverstate::ServerStateSubscriber;
using monitor::MonitorHeartBeaterAdapter;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&MonitorDispatcherI::instance(), service.createIdentity("M", ""));
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("MonitorDispatcher.Mod");
  ServerStateSubscriber::instance().initialize("ControllerMonitorDispatcher", &MonitorDispatcherI::instance(), mod, 5, new MyUtil::OceChannel, "5000", "5000", "1048576");
}


void MonitorDispatcherI::report(const string& type, const string& message, const Current& current) {
  MCE_INFO(type<< " "<<type.size() << " "<< message);
  if (type=="MASHINE_LIVE") {
    MonitorHeartBeaterAdapter::instance().beat(message);
  } else if (type == "MASHINE_MEMORY" || type == "MASHINE_LOAD" || type == "MASHINE_DISK" || type == "MASHINE_CPU") {
    MonitorAnalyzerAdapter::instance().analyse(type, message);
  }
}
