#include <time.h> 

#include "Engine.h"
#include "client/linux/handler/exception_handler.h"
#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "LogWrapper.h"

#include "Click.h"
#include "CachePool.h"
#include "Struct.h"
#include "InvertedIndex.h"
#include "Config.h"
#include "Monitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace com::xiaonei::xce;
using namespace xce::ad;
using namespace MyUtil;
using namespace std;
using namespace boost;


bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
	printf("got crash %s %s\n", dump_path, minidump_id);
	return true;
}

void MyUtil::initialize() {
	static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
	ServiceI& service = ServiceI::instance();
	int index = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("ServerIndex", -1);
	string DbInstance_Main = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance_Main","adedm_db_main");
	string DbInstance_Ads = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance_Ads","adedm_db_ads");
	string DbInstance_Wr = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance_Wr","adedm_db_main");

	MCE_INFO("MyUtil::initialize --> DbInstance_Main: " << DbInstance_Main);
	MCE_INFO("MyUtil::initialize --> DbInstance_Ads: " << DbInstance_Ads);
	MCE_INFO("MyUtil::initialize --> DbInstance_Wr: " << DbInstance_Wr);

	
	
	srand(time(NULL));

	InvertedIndex::instance().SetDB(DbInstance_Ads);
	EngineConfig::instance().Initialize(index);

	PlatformMap::instance().Initialize(DbInstance_Main, ONE_MINUTE);
	Brand3GMap::instance().Initialize(DbInstance_Main, ONE_MINUTE);
	ResolutionMap::instance().Initialize(DbInstance_Main, ONE_MINUTE);
	ResolutionMatcher::instance().Initialize(DbInstance_Main, ONE_MINUTE);

	MemberPool::instance().Initialize(DbInstance_Main, ONE_MINUTE);
	CampaignPool::instance().Initialize(DbInstance_Ads, ONE_MINUTE);
	GroupPool::instance().Initialize(DbInstance_Ads, ONE_MINUTE);
	ZonePool::instance().Initialize(DbInstance_Main, ONE_MINUTE);

	UserBindPool::instance().Initialize(DbInstance_Main, ONE_MINUTE);

	string fcgi_socket = service.getCommunicator()->getProperties()->getPropertyWithDefault("Service." + service.getName() + ".FcgiSocket", "127.0.0.1:1987");
	MCE_INFO("Fcgi listens on : " << fcgi_socket);

	// fcgi_server memory managed as RequestFactory ...
	FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 256, 256 * 1024);
	fcgi_server->RegisterRequestFactory(new AdMobRequestFactory());
	fcgi_server->Start();

	AdMobEngine::instance().initOK();

	service.getAdapter()->add(&AdMobEngine::instance(), service.createIdentity("M",""));
	service.getAdapter()->add(&AdMobMonitorI::instance(), service.createIdentity("B",""));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdMobEngine.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdMobEngine.Interval",5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdMobEngine",
			&AdMobEngine::instance(),
			mod,
			interval,
			new AdMobChannel());
	MCE_INFO("MyUtil::initialize.mod:" << mod << " interval:" << interval);
}

void AdMobEngine::initOK() {
	init_ok_ = true;
}

AdMobResPara AdMobEngine::GetAds(const xce::ad::AdMobReqPara& reqPara, const Ice::Current&) {
	if ( false == init_ok_ ) {
		MCE_INFO("initializing...");
		return AdMobResPara();
	}
	MCE_DEBUG("AdMobEngine::GetAds --> Start");
	HandlerPara para(reqPara);
	ZoneHandlerPtr handler = ZoneHandlerFactory::GetZoneHandler( reqPara.adzoneId );
	if ( handler ) {
		handler->handle( para );
	}
	MCE_INFO("AdMobEngine::GetAds --> End, Uid:" << para.request_.uid << " Selected Group:" << para.response_.adgroupId << " Handle flag:" << para.handle_flag_);
	return para.response_;
}

AdMobResPara AdMobEngine::GetAdsForTest(const xce::ad::AdMobReqPara& reqPara, const Ice::Current&) {

	MCE_INFO("AdMobEngine::GetAdsForTest --> Get a Request from Mobile Start");
	HandlerPara para(reqPara);
	ZoneHandlerPtr handler = ZoneHandlerFactory::GetZoneHandler( MOB_ZONE_TEST );
	if ( handler ) {
		handler->handle( para );
	}
	MCE_INFO("AdMobEngine::GetAdsForTest --> Get a Request from Mobile End");
	return para.response_;
}

void AdMobEngine::PvFeedBack(const xce::ad::AdMobFeedback& para, const Ice::Current&) {
	MCE_INFO("AdMobEngine::PvFeedBack --> Get a PvFeedBack from Mobile");
}

AdMobEngine::AdMobEngine() {
	init_ok_ = false;
}

