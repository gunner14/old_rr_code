#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "LogWrapper.h"
#include "Monitor.h"
#include "FeedLogRequest.h"
#include "FeedLogProxyI.h"
#include "Notify/util/fcgi_request.h"
#include "client/linux/handler/exception_handler.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace xce::feed;
using namespace IceUtil;
using namespace MyUtil;
using namespace std;

bool MyCallback(const char* dump_path, const char* minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s \n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  ServiceI& service = ServiceI::instance();
 
  FeedLogProxyI::instance().Initialize();

  string fcgi_socket = service.getCommunicator()->getProperties()->getPropertyWithDefault("Service." + service.getName() + ".FcgiSocket", "127.0.0.1:8086");
  MCE_INFO("Fcgi listens on : " << "fcgi_socket");
  FcgiServer* fcgi_server = new FcgiServer(fcgi_socket, 256,256 * 1024);
  fcgi_server->RegisterRequestFactory(new FeedLogRequestFactory());
  fcgi_server->Start();
/*
  service.getAdapter()->add(&FeedLogProxyI::instance(), service.createIdentity("M", ""));
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FeedLogProxy.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedLogProxy.Interval", 5);
  string name = "Controller" + service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedLogProxy.Name", "FeedLogProxy");
  xce::serverstate::ServerStateSubscriber::instance().initialize(name, &FeedLogProxyI::instance(), mod, interval, new FeedLogChannel());
*/
}

void FeedLogProxyI::Initialize() {
}
