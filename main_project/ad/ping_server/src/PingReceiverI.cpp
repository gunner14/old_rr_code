#include "PingReceiverI.h" 
#include "client/linux/handler/exception_handler.h"
#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "LogWrapper.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "PingMonitor.h"
#include "EmailJwsmpt.h"
#include "TaskManager.h"

using namespace std;
using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace MyUtil;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
	MCE_INFO("PingReceiver --> start");
  ServiceI& service = ServiceI::instance();
	string db_instance = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance", "db_adwr");
  int threshold = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Threshold", 200);  
  MCE_INFO("PingReceiver --> db_instance=" << db_instance); 
  PingReceiverI::instance().Initialize(threshold, db_instance);
  PingConfig::instance().LoadConfig();
  MonitorConfigPool::instance().SetConfigDBSource(db_instance);
  MonitorConfigPool::instance().Init();	
  JwsmptEmail::instance().Init();

  service.getAdapter()->add(&PingReceiverI::instance(), service.createIdentity("PR",""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval",5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerPingReceiver", 
			&PingReceiverI::instance(), mod, interval,	new AdMonitorChannel(),"10", "3000", "131072", "udp");

  TaskManager::instance().scheduleRepeated(new MonitorConfigTimer());
  TaskManager::instance().scheduleRepeated(new SendEmailTimer());
  PingReceiverI::instance().initOK();
	MCE_INFO("PingReceiver --> stared");
}

void PingReceiverI::Initialize(const int threshold, const string& DbInstance){
  db_source_ = DbInstance;
  threshold_ = threshold;
  manager_ = new PingInfoManager(threshold, DbInstance);
}

void PingReceiverI::initOK() {
  init_ok_ = true;
}

/*
 *  Func:PingReceiverI::WriteBatch
 *  Desc:把服务的方法及其响应时间写入数据库
 */
void PingReceiverI::WriteBatch(const xce::ad::ServerMethodSeq& methodRTimeSeq, const Ice::Current&){
  if (init_ok_ == false) {
    MCE_WARN("PingReceiverI::WriteBatch --> init not ok");
  }
  manager_->Put(methodRTimeSeq);
}
