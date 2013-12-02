#include "AdMobLogRI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace MyUtil;

void MyUtil::initialize(){
  ServiceI& service = ServiceI::instance();
  
	int buf_size = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.BufSize",100);
  AdMobLogRI::instance().SetBufSize(buf_size);
  AdMobLogRI::instance().Initialize();
  service.getAdapter()->add(&AdMobLogRI::instance(), service.createIdentity("M", ""));

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval",5);
  string type = service.getCommunicator()->getProperties()->getProperty("Service.Type");
  string name = "Controller" + type;
  xce::serverstate::ServerStateSubscriber::instance().initialize(name,&AdMobLogRI::instance(),mod,interval, new AdMobChannel());
}

void AdMobLogRI::Initialize() {
  req_log_maker_ = new AdMobLogMaker("mob_req", "../log/" + ServiceI::instance().getName() + "/req_val", "INFO", "%m%n", buf_size_); 
}

void AdMobLogRI::ReqLog(const string& log, const Ice::Current& current) {
  req_log_maker_->PushBack(log);
}
