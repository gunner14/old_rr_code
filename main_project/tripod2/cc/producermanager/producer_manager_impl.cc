#include "tripod2/cc/producermanager/producer_manager_impl.h"

#include "ServiceI.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "client/linux/handler/exception_handler.h"
#include "LogWrapper.h"

#include "tripod2/cc/producermanager/producer_factory.h"


bool MyCallback(const char *dump_path, const char *minidump_id, void *context,
        bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  using namespace xce::tripod;
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  MCE_WARN("breakpad");
  MyUtil::ServiceI& service = MyUtil::ServiceI::instance();

  ProducerManagerImpl* producer_manager = new ProducerManagerImpl();

  service.getAdapter()->add(producer_manager, service.createIdentity("M", ""));

//  MyUtil::ServiceMonitorManager::instance().start();
}



namespace xce {
namespace tripod {

ProducerManagerImpl::ProducerManagerImpl() {
  MCE_INFO("ProducerManagerImpl::ProducerManagerImpl()");
}

ProducerManagerImpl::~ProducerManagerImpl() {
  MCE_INFO("ProducerManagerImpl::~ProducerManagerImpl()");
}

ByteSeq ProducerManagerImpl::produce(const std::string& key, 
                                         const std::string& ns, 
                                         const std::string& biz, 
                                         bool use_master, 
                                         const Ice::Current& current) {

  std::ostringstream otem;
  otem << "ProducerManagerImpl::produce() key:" << key << "\tnamespace_id:" 
           << ns << "\tbusiness_id:" << biz << "\tuse_master:" << use_master;
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  ByteSeq value;
  Producer* producer = ProducerFactory::GetInstance().Get(biz);
  if (!producer) {
    MCE_WARN("ProducerManagerImpl::produce() cannot find biz:" << biz << " producer!");
    return value;
  }
  try {
    producer->Create(key, &value, use_master);
  } catch (Ice::TimeoutException& ex) {
    MCE_WARN("ProducerManagerImpl::produce() call producer->Create() Ice::Timeout exception");
  } catch (std::exception& ex) {
    MCE_WARN("ProducerManagerImpl::produce() call producer->Create() exception:" << ex.what());
  } catch (...) {
    MCE_ERROR("ProducerManagerImpl::produce() call producer->Create() ...exception");
  }
  return value;
}

std::string ProducerManagerImpl::ping(const ::Ice::Current& current) {
  std::ostringstream otem;
  otem << "ProducerManagerImpl::ping()";
  MyUtil::InvokeClient ic = MyUtil::InvokeClient::create(current, otem.str(), MyUtil::InvokeClient::INVOKE_LEVEL_INFO);
  return "PONG";
}

}} // xce::tripod

