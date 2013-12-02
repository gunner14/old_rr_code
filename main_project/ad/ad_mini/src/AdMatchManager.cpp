/*
 * AdMatchManager.cpp
 *
 *  Created on: 2012-4-26
 *      Author: ping.zhang
 */

#include "client/linux/handler/exception_handler.h"
#include "AdMatchManager.h"
#include "UrlMaping.h"
#include "ad_creative_cache.h"
using namespace std;
using namespace xce::ad;

string xce::ad::db_instance;
bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

class AdMiniRequestFactory : public RequestFactory {
  public:
    virtual RequestPtr Create(FCGX_Request * r) {
      char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
      if (path) {
        if (strcmp(path, "/ebpn/show") == 0) {
          return new AdGetRequest(r);
        }
        if (strcmp(path, "/ebpn/click.html") == 0) {
          return new AdClickRequest(r);
        }
        if (strcmp(path, "/ebp/show") == 0) {
          return new AdGateSRequest(r);
        }
        if (strcmp(path, "/ebp/click.html") == 0) {
          return new AdClickRequest(r);
        }
      }
      return NULL;
    }
};

void MyUtil::initialize() {
  //google breakpad for dump
  srand(time(NULL));
  //static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  // service start
  ServiceI& service = ServiceI::instance();
  xce::ad::db_instance = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance", "ade_db");
  MCE_INFO("db_instance = " << xce::ad::db_instance);
  service.getAdapter()->add(&xce::ad::AdMatchManagerI::instance(), service.createIdentity("AdMini", ""));
  TaskManager::instance().execute(new InitTask());
  MCE_INFO("MyUtil::initialize start ");

  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  string fcgi_socket = props->getPropertyWithDefault("Service." + service.getName() + ".FcgiSocket", "0.0.0.0:7001");
  MCE_INFO("Fcgi listens on : " << fcgi_socket);

  // fcgi_server memory managed as RequestFactory ...
  FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 256, 256 * 1024);
  fcgi_server->RegisterRequestFactory(new AdMiniRequestFactory());
  fcgi_server->Start();
  Widget2TypeCache::instance().Load();
  TaskManager::instance().scheduleRepeated(new Widget2TypeCacheTimer());
  TaskManager::instance().schedule(new xce::ad::AdUrlMapingLoadTimer(db_instance, 0));
  
}

void InitTask::handle(){
  AdMatchManagerI::instance().Init();
  MCE_INFO("InitTask::handle --> AdMatchManager::instance().Init(); DONE");
  AdMatchManagerI::instance().InitOk();
  //TODO 大约一小时重新加载一次DB
  usleep(59 * 59 * 1000 * 1000);
  TaskManager::instance().execute(new ReloadTask());
}


void ReloadTask::handle(){
  while(true) {
    try {
      MCE_INFO("ReloadTask::handle --> BEGIN RELOAD");
      AdMatchManagerI::instance().Init();
      MCE_INFO("ReloadTask::handle --> END RELOAD");
      usleep(15 * 59 * 1000 * 1000);
    } catch (std::exception& e) {
      MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__ << " err:" << e.what());
    } catch (...) {
      MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__);
    }   
  }
}

xce::ad::AdMatchManagerI::AdMatchManagerI() {
  init_ok_ = false;
  MCE_INFO("AdMatchNManagerI::Create --> The Engine started.");
}

void AdMatchManagerI::Init() {
  AdCreativeCache::instance().Reload();
  MCE_INFO("AdMatchManagerI::Init --> AdCreativeCache::instance().Reload(); DONE");
  AdCreativeCache::instance().LoadTemplate();
  MCE_INFO("AdMatchManagerI::Init --> AdCreativeCache::instance().LoadTemplate(); DONE");
  //AdUrlMaping::instance().Load();
}

void AdMatchManagerI::GetAdsByRefer(string &refer, xce::ad::AdZone2Creatives &ads) {
  MCE_DEBUG("AdMatchNManagerI::GetAdsByRefer.");
  set<Ice::Long> zones;
  AdUrlMaping::instance().GetAdZone(refer, zones);
  AdCreativeCache::instance().GetAdsByZoneInfo(zones,ads);
}

bool AdGetRequest::response() {
  timeval start;
  gettimeofday(&start, NULL);
  if (!_fcgi_out) {
    MCE_WARN("remove notify fcgi_out null");
    return false;
  }
  string pageType = getProperty("pageType");
  if(pageType == "profile") {
    stringstream ad_header1;
    string ad_body1;
    AdZone2Creatives null_ads;
    AdCreativeCache::instance().GetJson(null_ads, &ad_body1);
    ad_header1<< "Content-type: application/x-javascript; charset=utf-8\r\n";
    FCGX_PutS(ad_header1.str().c_str(), _fcgi_out);
    FCGX_PutS(ad_body1.c_str(), _fcgi_out);
    return true;
  }
  string referer = getProperty("ref");
  if (referer.empty() || referer.find("http://") == string::npos) {
    referer = getProperty("r");
    if (referer.empty() || referer.find("http://") == string::npos) {
      referer = GetParamFix("HTTP_REFERER");
    }
  }
  if(pageType == "home") {
    referer = "http://www.renren.com/home";
  }
  AdZone2Creatives final_ads;
  AdMatchManagerI::instance().GetAdsByRefer(referer,final_ads);

  stringstream ad_header;
  string ad_body;
  int rotate_slot = GetCookieInt("an_slot", rand());
  rotate_slot %= 10000;
  ad_header
       << "Content-type: application/x-javascript; charset=utf-8\r\nSet-Cookie: an_slot="
       << ++rotate_slot << "; path=/ebpn\r\n\r\n";
  AdCreativeCache::instance().GetJson(final_ads, &ad_body);
  FCGX_PutS(ad_header.str().c_str(), _fcgi_out);
  FCGX_PutS(ad_body.c_str(), _fcgi_out);
  MCE_DEBUG("AdGetRequest::response --> return uid=" << _login_uid);
  MCE_DEBUG("Adbody:Json:" << ad_body);
  timeval end;
  gettimeofday(&end, NULL);
  if(rand()%100 == 1){
    MCE_INFO("AdMini select ad spend time:" << (1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000) << " ms");
  }
  return true;
}

bool AdClickRequest::response() {
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }
  string url = getProperty("url");
  string output = "Location: " + url + "\r\n\r\n";
  FCGX_PutS(output.c_str(), _fcgi_out);
  return true;

}

bool AdGateSRequest::response() {
  if (!_fcgi_out) {
    MCE_WARN("remove notify fcgi_out null");
    return false;
  }

  string referer = getProperty("ref");
  if (referer.empty() || referer.find("http://") == string::npos) {
    referer = getProperty("r");
    if (referer.empty() || referer.find("http://") == string::npos) {
      referer = GetParamFix("HTTP_REFERER");
    }
  }
  AdZone2Creatives final_ads;
  AdMatchManagerI::instance().GetAdsByRefer(referer,final_ads);
  stringstream ad_header;
  string ad_body;
  AdCreativeCache::instance().GetJsonS(final_ads, &ad_body);
  int rotate_slot = GetCookieInt("an_slot", rand());
  rotate_slot %= 10000;
  ad_header << "Content-type: application/x-javascript; charset=utf-8\r\nSet-Cookie: an_slot=" << ++ rotate_slot << "; path=/ebp\r\n\r\n";

  FCGX_PutS(ad_header.str().c_str(), _fcgi_out);
  FCGX_PutS(ad_body.c_str(), _fcgi_out);
  MCE_DEBUG("AdGetRequest::response --> return uid=" << _login_uid);
  MCE_DEBUG("Adbody:Json:" << ad_body);
  return true;
}
