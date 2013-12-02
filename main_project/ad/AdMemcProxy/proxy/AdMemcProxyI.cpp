#include "AdMemcProxyI.h"

#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "key_mapping.h"
#include "proxy_server.h"

#include "AdMemcProxyAdapter.h"
#include "AdMemcProxyReplicaAdapter.h"
#include "ServiceI.h"
#include "LogWrapper.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

//#include "PerformanceStat.h"

//#include "client/linux/handler/exception_handler.h"

using namespace xce::ad;
using namespace MyUtil;

/*bool MyCallback(const char *dump_path,
    const char *minidump_id,
    void *context,
    bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}*/

void MyUtil::initialize() {
	MCE_DEBUG("begin initialize...");
  
	//TaskManager::instance().scheduleRepeated(&PerformanceStat::instance());
  //static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&AdMemcProxyI::instance(), service.createIdentity("M", ""));
	
//	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
//	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
//	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdMemcProxy", &AdMemcProxyI::instance(), mod, interval, new AdChannel());
//	MCE_INFO("MyUtil::initialize. mod:");

  if (!InitCacheClusters()) {
	  MCE_WARN("InitCacheClusters() error.");
    return;
  }

  boost::thread t(boost::bind(&ProxyServer::Run, &ProxyServer::instance()));
  t.detach();
}

bool AdMemcProxyI::SetUpstreamList(const ::std::string & cfg, const Ice::Current&) {
  return false;
}

string GetIp(){
  char host_name[255]; 
  if(gethostname(host_name, 255) == -1) { 
    MCE_INFO("FeedMemcProxyI::GetIp --> get host name err"); 
    return ""; 
  } 


  struct hostent *phe = gethostbyname(host_name); 
  if (phe == 0) { 
    return ""; 
  }
  string ip; 
  for (int i = 0; phe->h_addr_list[i] != 0; ++i) { 
    struct in_addr addr; 
    memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr)); 
    ip = inet_ntoa(addr);
    MCE_INFO("FeedMemcProxyI::GetIp --> " << ip);
	}
  return ip;
}

ProxyInfoPtr AdMemcProxyI::GetProxyInfo(const Ice::Current&){
  if(ip_.empty() || ip_=="127.0.0.1"){
    ip_ = GetIp();

    if(ip_.empty() || ip_=="127.0.0.1"){
      return NULL;
    }
  }
  ProxyInfoPtr info = new ProxyInfo;
  info->ip = ip_;
  info->port = 11315;
  info->connections = ProxyServer::instance().ClientSize();

  return info;
}

  // 取得McProxy 的 memcache 监听端口
string AdMemcProxyI::GetEndpoint(const Ice::Current&) {
  char host_name[255]; 
  if(gethostname(host_name, 255) == -1) { 
    MCE_INFO("FeedMemcProxyI::GetEndpoint --> get host name err"); 
    return ""; 
  } 
  struct hostent *phe = gethostbyname(host_name); 
  if (phe == 0) { 
    return ""; 
  }
  string ip; 
  for (int i = 0; phe->h_addr_list[i] != 0; ++i) { 
    struct in_addr addr; 
    memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr)); 
    ip = inet_ntoa(addr);
    MCE_INFO("FeedMemcProxyI::GetEndpoint --> " << ip);
  } 
  return ip+":11315";
}
