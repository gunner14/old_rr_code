#include "AccountConnectI.h"

#include "util/cpp/InvokeMonitor.h"
#include "UtilCxx/src/LogWrapper.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "client/linux/handler/exception_handler.h"
#include "OceCxxAdapter/src/AccountConnectTripodAdapter.h"
#include "DbCxxPool/src/QueryRunner.h"

using namespace std;
using namespace MyUtil;
using namespace xce::account;
using namespace xce::serverstate;
using namespace xce::adapter::account;
using namespace com::xiaonei::xce;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

//初始化
//--------------------------------------------------------------
void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  MCE_WARN("breakpad");

  ServiceI& service = ServiceI::instance();

  //读取配置
  int cfg_mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVICE_PREFIX + ".Mod", 0);
  int cfg_interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVICE_PREFIX + ".Interval", 5);
  int cfg_status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(SERVICE_PREFIX + ".Status", 0);

  //注册服务
  service.getAdapter()->add(&AccountConnectI::instance(), service.createIdentity(SERVICE_IDENTITY, ""));

  //HA
  ServerStateSubscriber::instance().initialize(CONTROLLER_ENDPOINT, &AccountConnectI::instance(), cfg_mod, cfg_interval);
  {
    ServerStateSubscriber::instance().setStatus(cfg_status);
    std::ostringstream os;
    os << "MyUtil::initialize set status : " << cfg_status;
    MCE_WARN(os.str());
  }

  //监控
  ServiceMonitorManager::instance().start();
}

AccountConnectI::AccountConnectI() {
  MCE_INFO("AccountConnectI::AccountConnectI init");
  tempMutexManagerPtr_ = new xce::tempmutex::TempMutexManager;
}

bool AccountConnectI::set(const string &cid, int type, long uid, const Ice::Current& current) {
  ostringstream oss;
  oss << "cid: " << cid << " type: " << type << " uid: " << uid;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  xce::tempmutex::Lock lock(tempMutexManagerPtr_, uid);
  bool result = false;
  Statement sql;
  sql << "INSERT INTO account_connect (cid,type,uid) VALUE ('" << cid << "'," << type << "," << uid << ")"
    << " ON DUPLICATE KEY UPDATE uid = " << uid;
  try {
    mysqlpp::SimpleResult sql_result = QueryRunner("user_passport", CDbWServer).execute(sql);
    MCE_INFO("AccountConnectI::set << " << oss.str() << " affect rows:" << sql_result.rows());
    result = AccountConnectTripodAdapter::instance().Load(cid, type, true); 
  } catch (std::exception e) {
    MCE_WARN("AccountConnectI::set << " << oss.str() << " failed e: " << e.what());
  }
  return result;
}

bool AccountConnectI::isValid(const Ice::Current& current) {
  InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
  return ServerStateSubscriber::instance().getStatus();
}

void AccountConnectI::setValid(bool valid, const Ice::Current& current) {
  ostringstream oss;
  oss << "value: " << valid;
  InvokeClient ic = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  ServerStateSubscriber::instance().setStatus((int) valid);
}
