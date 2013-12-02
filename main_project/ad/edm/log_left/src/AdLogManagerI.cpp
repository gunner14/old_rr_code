#include <ostream>
#include "AdLogManagerI.h"
//#include "AdUnion/gate/ad_logger.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace MyUtil;

void MyUtil::initialize(){
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&AdLogManagerI::instance(), service.createIdentity("M", ""));
	int buf_size = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.BufSize",300);
	AdLogManagerI::instance().SetBufSize(buf_size);
	AdLogManagerI::instance().Initialize();
  
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval",5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdEdmLogLeft",&AdLogManagerI::instance(),mod,interval, new EdmChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
}

void AdLogManagerI::Initialize() {
	click_log_maker_ = new AdLogMaker("left_click", "../log/" + ServiceI::instance().getName() + "/leftclick_val", "INFO", "%m%n", buf_size_);
  cpc_charge_log_maker_ = new AdLogMaker("left_click_charge", "../log/" + ServiceI::instance().getName() + "/charge_val", "INFO", 
			"%m%n", buf_size_);
	//pv_log_maker_ = new AdLogMaker("ad_pv", "../log/" + ServiceI::instance().getName() + "/pv_val", "INFO", "%m%n", buf_size_);
	//pv_charge_log_maker_ = new AdLogMaker("ad_pv_charge", "../log/" + ServiceI::instance().getName() + "/pv_charge_val", "INFO", "%m%n", buf_size_);
}

void AdLogManagerI::ClickForBp(const string& log,const Ice::Current& current){
  click_log_maker_->PushBack(log);
}

void AdLogManagerI::ClickForCharge(const string& log,const Ice::Current& current){
  MyUtil::Date date(time(NULL));
  string clicktime = date.toDateTime();
  string timeval = date.toDateTime("%Y-%m-%d %H:%M:");
  ostringstream os;
  os << log << "," << timeval << "00," << timeval << "59";
  cpc_charge_log_maker_->PushBack(os.str()); 
}

void AdLogManagerI::PvForCharge(const ::MyUtil::StrSeq& logs,const Ice::Current& current){
}

void AdLogManagerI::Pv(const string& log, const Ice::Current& current){
}

void AdLogManagerI::PvBySeq(const MyUtil::StrSeq& logs, const Ice::Current& current){
}

Ice::Int AdLogManagerI::UpdateOnceCount(const Ice::Int count, const Ice::Current& current) {
	return 0;
}

void AdLogManagerI::PvBatch(const ::MyUtil::StrSeq& logs, const Ice::Current& current){
}

void AdLogManagerI::EngineLog(const string& log, const Ice::Current& current){
}

void AdLogManagerI::EngineLogBatch(const ::MyUtil::StrSeq& logs, const Ice::Current& current){
}

