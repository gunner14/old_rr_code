#include "AdMobLogI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace MyUtil;

void MyUtil::initialize(){
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&AdMobLogI::instance(), service.createIdentity("M", ""));
  
	int buf_size = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.BufSize",20);
  AdMobLogI::instance().SetBufSize(buf_size);
  AdMobLogI::instance().Initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval",5);
  string type = service.getCommunicator()->getProperties()->getProperty("Service.Type");
  string name = "Controller" + type;
  xce::serverstate::ServerStateSubscriber::instance().initialize(name,&AdMobLogI::instance(),mod,interval, new AdMobChannel());
}

void AdMobLogI::Initialize() {
  pv_log_maker_ = new AdMobLogMaker("mob_pv", "../log/" + ServiceI::instance().getName() + "/pv_val", "INFO", "%m%n", buf_size_); 
  click_log_maker_ = new AdMobLogMaker("mob_click", "../log/" + ServiceI::instance().getName() + "/click_val", "INFO", "%m%n", buf_size_); 
  cpm_charge_log_maker_ = new AdMobLogMaker("mob_cpm", "../log/" + ServiceI::instance().getName() + "/cpm_val", "INFO", "%m%n", buf_size_); 
  cpc_charge_log_maker_ = new AdMobLogMaker("mob_cpc", "../log/" + ServiceI::instance().getName() + "/cpc_val", "INFO", "%m%n", buf_size_); 
}

void AdMobLogI::Pv(const string& log, const Ice::Current& current) {
  pv_log_maker_->PushBack( log );
}

void AdMobLogI::Click(const string& log, const Ice::Current& current) {
  click_log_maker_->PushBack( log );
}

void AdMobLogI::CpmCharge(const string& log, const Ice::Current& current) {
  cpm_charge_log_maker_->PushBack( log );
}

void AdMobLogI::CpcCharge(const string& log, const Ice::Current& current) {
  cpc_charge_log_maker_->PushBack( log );
}

void AdMobLogI::PvBatch(const StrSeq& logs, const Ice::Current& current) {
  pv_log_maker_->PushBack( logs );
}

void AdMobLogI::ClickBatch(const StrSeq& logs, const Ice::Current& current) {
  click_log_maker_->PushBack( logs );
}

void AdMobLogI::CpmChargeBatch(const StrSeq& logs, const Ice::Current& current) {
  cpm_charge_log_maker_->PushBack( logs );
}

void AdMobLogI::CpcChargeBatch(const StrSeq& logs, const Ice::Current& current) {
  cpc_charge_log_maker_->PushBack( logs );
}
