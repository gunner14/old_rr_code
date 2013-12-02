#include <sys/resource.h>

//#include "Request.h"
#include "Server.h"
#include "Cache.h"
#include "Session.h"
#include "ManagerI.h"
#include "ClientBuffer.h"


using namespace talk::http;


void MyUtil::initialize() {
	MCE_DEBUG("begin initialize...");
	//set openfiles
	struct rlimit limit;
	if (getrlimit(RLIMIT_NOFILE, &limit) < 0) {
		MCE_WARN("no right to get open files");
	} else {
		MCE_INFO("openfile  cur:"<<limit.rlim_cur<<"  max:"<<limit.rlim_max);
	}
	limit.rlim_cur = limit.rlim_max = RLIM_INFINITY;
	if (setrlimit(RLIMIT_NOFILE, &limit) < 0) {
		MCE_WARN("no right to set open files");
	} else {
		MCE_INFO("openfile  cur:"<<limit.rlim_cur<<"  max:"<<limit.rlim_max);
	}

	ClientPoolManager::instance().startTasks();//下线 统计 timer


	ServiceI& service = ServiceI::instance();
	Ice::PropertiesPtr props = service.getCommunicator()->getProperties();

	Server::instance().setWapFlag( (bool)props->getPropertyAsIntWithDefault("Service."+ServiceI::instance().getName()+".IsWap",0));

	service.getAdapter()->add(&ManagerI::instance(), service.createIdentity(PROXY_MANAGER,
			""));

	service.getObjectCache()->registerCategory(UPC, "UPC", new UserPageFactoryI, props, true);//page contet cache 
	service.getObjectCache()->registerCategory(UN, "UN", new UserNameFactoryI, props, true);//用户名缓存

	string configFilePath = props->getPropertyWithDefault("Service.ConfigFilePath", "/opt/XNTalk/etc/XNHttpServer.conf");
	ConnectingStat::instance().loadConfig(configFilePath);
	try{
	WTalkProxyLogic::instance().closeAccessServer(service.getName());
	}catch(Ice::Exception& e){
		MCE_WARN("initialize --> close access client err:"<<e<<" file:"<<__FILE__<<" line:"<<__LINE__);
	}catch(std::exception& e){
		MCE_WARN("initialize --> close access client err:"<<e.what()<<" file:"<<__FILE__<<" line:"<<__LINE__);
	}
	MCE_DEBUG("Ready to start Server, ISWAP:"<<Server::instance().isWap());
	MCE_INFO("LINUX_VERSION_CODE:"<< LINUX_VERSION_CODE
			<<"   epoll basic level:"<< KERNEL_VERSION(2, 5, 45));

	Server::instance().start(props);

	MCE_DEBUG("finish initialize...");


}

//----------------------------------------------------------------------

void Server::start(const Ice::PropertiesPtr& props) {
	TaskManager::instance().scheduleRepeated(&(IdleConnManager::instance()));

	int port = props->getPropertyAsIntWithDefault("Service.Acceptor.Port", 80);
	_serverIndex = props->getPropertyAsIntWithDefault("Service.ServerIndex", 0);

	_refactor = new ::xce::nio::Refactor(port, _serverIndex, new SessionFactoryI, 50, "", 500000);
	_refactor->start();

	MCE_DEBUG("Server::started");
}

void Server::stop() {
}


SessionPtr Server::getSession(Ice::Long connId) {
	xce::nio::ConnectionPtr c;
	c = _refactor->getConnection(connId);
	if (c) {
		return SessionPtr::dynamicCast(c->channel());

	}
	return 0;
}

bool Server::indexExist(Ice::Long index){
	return _refactor->getConnection(index);
}


void Server::deliverStat(const int status_code, const string &body) {
  _deliver_stat.deliverstat(status_code, body);
    
}

void IdleConnManager::handle(){

		//SessionPtr
	int readNothingCount = 0;	//read nothing

	int readSomethingCount = 0;

	int verifyCount = 0;		//verify count
	int notVerifyCount = 0;

	int validCometCount = 0;
	int nullCometCount = 0;
	int otherConnIdCount = 0;

	while(true){
		pair<Ice::Long, time_t> p;
		{
			IceUtil::Mutex::Lock lock(_mutex);
			if(_sessions.empty()){
				break;
			}
			p = _sessions.front();
			if(::time(NULL) - p.second < CONN_TIMEOUT){
				break;
			}
			_sessions.pop_front();
		}

		SessionPtr s = Server::instance().getSession(p.first);
		if(!s){
			continue;
		}
		if(!s->isReadSomething()){
			readNothingCount++;
			MCE_WARN("~~~DOS Attack~~~");
			s->shutdown(true);
		}else{
			readSomethingCount++;
			if(s->verify()){
				verifyCount++;
				JidPtr& jid = s->jid();
				if(!jid){
					continue;
				}
				Ice::Long connId = ClientPoolManager::instance().getClientPool(jid->index).getActiveConnection(jid->index);
				if(connId==0){
					nullCometCount++;
				}else if(jid->index == connId){
					validCometCount++;
				}else{
					otherConnIdCount++;
				}
			}else{
				notVerifyCount++;
			}
		}
	}
	/*
	MCE_INFO("~! readNothingCount:" << readNothingCount
			<< ", readSomethingCount:" << readSomethingCount
			<< ", verifyCount:" << verifyCount
			<< ", notVerifyCount:" << notVerifyCount
			<< ", nullCometCount:" << nullCometCount
			<< ", validCometCount:" << validCometCount
			<< ", otherConnIdCount:" << otherConnIdCount
			<< ", cometConnSwitch" << _cometConnSwitch
			);
	*/
	/*
	MCE_INFO("~@ stoppedReadNothing:" << _stoppedReadNothing
			<< ", _stoppedReadSomething:" << _stoppedReadSomething
			<< ", _stoppedVerify:" << _stoppedVerify
			<< ", _stoppedNotVerify:" << _stoppedNotVerify
			<< ", _stopedNullComet:" << _stopedNullComet
			<< ", _stopedOtherConnId:" << _stopedOtherConnId
			<< ", _stopedValidComet:" << _stopedValidComet
			);
	*/
	_cometConnSwitch = 0;
	_stoppedReadNothing = 0;
	_stoppedVerify = 0;
	_stoppedNotVerify = 0;
	_stoppedReadSomething =0 ;
	_stopedNullComet = 0;
	_stopedOtherConnId = 0;
	_stopedValidComet = 0;
	//TaskManager::instance().schedule(this);
}
/*
void IdleConnManager::connStopped(Ice::Long connId, bool isReadSomthing,
								bool isVerify){

	if(!isReadSomthing){
		_stoppedReadNothing++;
	}else{
		_stoppedReadSomething++;
		if(isVerify){
			_stoppedVerify++;
			Ice::Long activeConnId = 0;
			SessionPtr s = Server::instance().getSession(connId);
			if(s){
				JidPtr jid = s->jid();
				if(jid){
					activeConnId = ClientPoolManager::instance().getClientPool(jid->index).
										getActiveConnection(jid->index);
				}
			}

			if(activeConnId==0){
				_stopedNullComet++;
			}else if(activeConnId == connId){
				_stopedValidComet++;
			}else{
				_stopedOtherConnId++;
			}
		}else{
			_stoppedNotVerify++;
		}
	}
}*/
