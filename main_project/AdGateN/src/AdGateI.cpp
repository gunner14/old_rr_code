#include "AdGateI.h"

#include <cstring>
#include "client/linux/handler/exception_handler.h"

#include "ad_creative_cache.h"
#include "ad_update_interval.h"
#include "ip_area_cache.h"

#include "FeedMemcProxy/client/user_profile_client.h"
#include "AdUserCacheLoaderReplicaAdapter.h"
#include "AdEngineBReplicaAdapter.h"
#include "AdEngineSAdapter.h"
#include "AdEngineUReplicaAdapter.h"
#include "IndexServiceReplicaAdapter.h"
//#include "AdTwSAdapter.h"
#include "UrlMaping.h"
#include "AdGateConfig.h"
#include "ad/enginejclient/src/EngineJClient.h"
#include "AdEngineJAdapter.h"
#include "ad/Degrade/src/Degrade.h"
using namespace std;
using namespace xce::ad;

class StatFunc {
  string name_;
  int uid_; 
  timeval start_, end_;
public:
  StatFunc(string name, int uid = -1) :
    name_(name),
    uid_(uid) {
    gettimeofday(&start_, NULL);
  }
  

  ~StatFunc() {
    gettimeofday(&end_, NULL);
    float timeuse = 1000000 * (end_.tv_sec - start_.tv_sec) + end_.tv_usec
        - start_.tv_usec;
    timeuse /= 1000;
    if (uid_ >= 0) {
      MCE_INFO(name_ << " uid : " << uid_ << ", time usage : " << timeuse << " ms");
    } else {
      if (timeuse >= 5) {
        MCE_INFO(name_ << " time usage : " << timeuse << " ms");
      } else {
        MCE_DEBUG(name_ << " time usage : " << timeuse << " ms");
//        MCE_DEBUG(name_ << " uid : " << uid_ << ", time usage : " << timeuse << " ms");
      }   
    }
  }
};

bool MyCallback(const char *dump_path, const char *minidump_id, void *context,
    bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

class CreativeReloadTimer : public MyUtil::Timer {
public:
  CreativeReloadTimer(time_t t) :
    Timer(t) {
  }
  virtual void handle() {
    MCE_INFO("Creative reload start.");
    if (AdCreativeCache::instance().Reload()) {
      MCE_INFO("Creative reload complete.");
    } else {
      MCE_WARN("Creative reload fail.");
    }
  }
};

class GateRequestFactory : public RequestFactory {
public:
  virtual RequestPtr Create(FCGX_Request * r) {
    char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
    if (path) {
      if (strcmp(path, "/ebpn/show") == 0 || strcmp(path, "/ebpt/show") == 0) {
        return new AdGetRequest(r, AdGetRequest::RT_JSON);
      }
      if (strcmp(path, "/ebpn/get.html") == 0) {
        return new AdGetRequest(r, AdGetRequest::RT_HTML);
      }
      if (strcmp(path, "/ebpn/click.html") == 0) {
        return new AdClickRequest(r);
      }
      if (strcmp(path, "/ebpn/close.html") == 0) {
        return new AdCloseRequest(r);
      }
      if (strcmp(path, "/ebpn/test.html") == 0) {
        return new AdTestRequest(r);
      }
    }
    return NULL;
  }
};

void MyUtil::initialize() {
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  ServiceI& service = ServiceI::instance();
  string ad_db_instance = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance_Ads", "jebe_ads_load");
  string main_db_instance = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance_Main","jebe_main_load");
  AdCreativeCache::instance().SetAdDBSource(ad_db_instance);
  AdCreativeCache::instance().SetMainDBSource(main_db_instance);
  MCE_INFO("ad_db_instance = " << ad_db_instance << ", main_db_instance = " << main_db_instance);
  GateConfig::instance().set_server_name(service.getName());
  AdGateI::instance().CreatePingSender(GateConfig::instance().server_name(), GateConfig::instance().ping_seq_size());
  service.getAdapter()->add(&AdGateI::instance(), service.createIdentity("M", ""));

  MCE_INFO("creatives load start.");
  AdCreativeCache::instance().Reload();
  MCE_INFO("creatives load complete.");

  TaskManager::instance().schedule(new AdUpdateIntervalReloadTimer(0));
  TaskManager::instance().execute(new IpAreaCacheReloadTask());
  //TaskManager::instance().schedule(new CreativeReloadTimer(0));
  TaskManager::instance().schedule(new xce::ad::AdUrlMapingLoadTimer(main_db_instance, 0));
  TaskManager::instance().scheduleRepeated(new CreativeReloadTimer(30 * 1000));

  /*
   std::string degrade_host =
     service.getCommunicator()->getProperties()->getPropertyWithDefault("DegradeHost", "10.3.23.69");
   std::string degrade_port = 
     service.getCommunicator()->getProperties()->getPropertyWithDefault("DegradePort", "3306");
   std::string degrade_user =
     service.getCommunicator()->getProperties()->getPropertyWithDefault("DegradeUser", "ad");
   std::string degrade_password =
     service.getCommunicator()->getProperties()->getPropertyWithDefault("DegradePassword", "rrad");
   std::string degrade_db =
     service.getCommunicator()->getProperties()->getPropertyWithDefault("DegradeDB", "index_para");
     */

  std::string source_db = service.getCommunicator()->getProperties()->getPropertyWithDefault("DegradeSource", "ad_st");
  DegradeManager::instance().SetDBSource(source_db);

  DegradeManager::instance().Register("engine_u");
  DegradeManager::instance().Register("engine_b");
  DegradeManager::instance().Register("gate_memcache");
  DegradeManager::instance().Register("gate_cache_loader");
  int ret = DegradeManager::instance().Init();
  if(ret < 0){
    MCE_WARN("DegradeManager Init Failed");
  }else{
    MCE_INFO("DegradeManager Init Succ");
  }

  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  string fcgi_socket = props->getPropertyWithDefault("Service."
      + service.getName() + ".FcgiSocket", "0.0.0.0:9001");
  MCE_INFO("Fcgi listens on : " << fcgi_socket);

  srand( time(NULL));

  // fcgi_server memory managed as RequestFactory ...
  FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 256, 256 * 1024);
  fcgi_server->RegisterRequestFactory(new GateRequestFactory());
  fcgi_server->Start();
}

AdGateI::AdGateI() {
}

bool AdGateI::Reload(const Ice::Current&) {
  return AdCreativeCache::instance().Reload();
}

bool AdGateI::ReloadCreativeById(Ice::Long id, const Ice::Current&) {
  return AdCreativeCache::instance().ReloadById(id);
}
/**
* 响应广告请求：下游请求EngineB/U  AdTwS
*
*
**/
bool AdGetRequest::response() {
  timeval start;
  string pageType = getProperty("pageType");
  if(pageType == "profile") {
    stringstream ad_header1;
    string ad_body1;
    AdZone2Creatives null_ads; 
    string refer_null = "";
    map<long, EngineTypes> self_zones_null;
    AdCreativeCache::instance().GetJson(refer_null,
        getPropertyInt("refresh_source", 0),
        getPropertyInt("refresh_idx", 0),
        self_zones_null,
        null_ads,
        &ad_body1);
    ad_header1<< "Content-type: application/x-javascript; charset=utf-8\r\n";
    FCGX_PutS(ad_header1.str().c_str(), _fcgi_out);
    FCGX_PutS(ad_body1.c_str(), _fcgi_out);
    return true;
  }
  gettimeofday(&start, NULL);
  if (!_fcgi_out) {
    MCE_WARN("remove notify fcgi_out null");
    return false;
  }

  _login_uid = 2;
  string uid = getProperty("uid");
  if (_cookies.find("id") != _cookies.end()) {
    uid = _cookies["id"];
  }
  if (!uid.empty()) {
    try {
      _login_uid = boost::lexical_cast<int>(uid);
    } catch (...) {
      MCE_WARN("lexical_cast cookie id error : " << uid);
    }
  }

  string serialized;
  if (_login_uid > 0 && DegradeManager::instance().IsEnable("gate_memcache")) {
    bool b = false;
    try {
      StatFunc stat("memc GetSerialized", _login_uid);
      b = UserProfileClient::instance().GetSerialized(_login_uid, &serialized);
    } catch (std::exception& e) {
      MCE_WARN("UserProfileClient error : " << e.what() << " uid="
          << _login_uid);
    }

    if (!b && DegradeManager::instance().IsEnable("gate_cache_loader")) {
//      MCE_INFO("profile not found in memcache. id=" << _login_uid);
      try {
        StatFunc stat("memc AdUserCacheLoader", _login_uid);
        serialized = AdUserCacheLoaderReplicaAdapter::instance().GetById(_login_uid);
      } catch (Ice::Exception& e) {
        MCE_WARN("AdUserCacheLoaderReplicaAdapter GetById error : " << e << " uid="
            << _login_uid);
      } catch (std::exception& e) {
        MCE_WARN("AdUserCacheLoaderReplicaAdapter GetById error : " << e.what()
            << " uid=" << _login_uid);
      }
    }
  }
  // FCGX_PutS("Content-type: application/x-javascript; charset=utf-8\r\n\r\n{}", _fcgi_out);
  // return true;

  UserProfile profile;
  if (serialized.empty()) {
    profile.set_stage(4);
    profile.set_gender(0);
    profile.set_age(25);
    profile.set_school(0);
    profile.set_major(0);
    profile.set_grade(0);
    profile.set_home_area("");
    profile.set_current_area("");
  } else {
    profile.ParseFromString(serialized);
  }

  unsigned int ip = ntohl(inet_addr(GetRemoteAddr().c_str()));

  profile.set_id(_login_uid);
  profile.set_ip(ip);
  profile.set_ip_area(IpAreaCache::instance().GetIpArea(ip));
  profile.SerializeToString(&serialized);

  int rotate_slot = GetCookieInt("an_slot", rand());
  rotate_slot %= 10000;

  string referer = getProperty("ref");
  string count = getProperty("count");
  if (referer.empty() || referer.find("http://") == string::npos) {
    referer = getProperty("r");
    if (referer.empty() || referer.find("http://") == string::npos) {
      referer = GetParamFix("HTTP_REFERER");
    }
  }
  //MCE_DEBUG("REFERER = " << referer << ", uid " << uid << ", count = " << count);

  if(pageType == "home") {
    referer = "http://www.renren.com/home";
  }
  bool err_flag = false;

  stringstream log_url;
  try {
    char sep = '&';
    if (referer.find_first_of('?') == string::npos) {
      sep = '?';
    }
    int refresh_source = getPropertyInt("refresh_source", 0);
    int refresh_idx = getPropertyInt("refresh_idx", 0);
    log_url << referer << sep << "refresh_source=" << refresh_source
        << "&refresh_idx=" << refresh_idx;
    
    stringstream stat_desc;
    stat_desc << _login_uid << " GetAds";
    StatFunc stat(stat_desc.str());
  } catch (Ice::Exception& e) {
    MCE_WARN("AdGetRequest::response error : " << e << " uid=" << _login_uid);
    err_flag = true;
  } catch (std::exception& e) {
    MCE_WARN("AdGetRequest::response : " << e.what() << " uid=" << _login_uid);
    err_flag = true;
  }

  AdResultMap ads;
  AdZone2Creatives final_ads;
  MCE_DEBUG("AdGetRequest::response,uid:" << _login_uid << ",log_url:" << log_url.str());

  try {
    ads = AdEngineBReplicaAdapter::instance().GetAds(serialized, log_url.str(),
        rotate_slot);
    //AdGateI::instance().translate(ads,final_ads);
  } catch (Ice::Exception& e) {
    MCE_WARN("AdEngineBReplicaAdapter::GetAds error : " << e << " uid=" << _login_uid);
    err_flag = true;
  } catch (std::exception& e) {
    MCE_WARN("AdEngineBReplicaAdapter::GetAds error : " << e.what() << " uid="
        << _login_uid);
    err_flag = true;
  }

  string tag, photo;
  map<long, EngineTypes> self_zones; // 需要中小自助广告广告位id
  ZoneInfoMap request_zones;
  for(AdResultMap::iterator it =  ads.begin(); it != ads.end(); it++) {
    if(!it->second.empty() && (it->second.at(0).groupid == -1)) {
      self_zones[it->first] = ENGINE;
      ZoneInfo zone_info;
      zone_info.zoneid = it->first;
      MCE_DEBUG("LINE: " << __LINE__ << " request_zones[" << it->first << "]: zone_info.zoneid = " << zone_info.zoneid);
      request_zones[it->first] = zone_info;
    } else if(!it->second.empty() && (it->second.at(0).groupid == -2)) {
      //余量
      self_zones[it->first] = ENGINE;
      tag = "rr_REMAIN_1";
      ZoneInfo zone_info;
      zone_info.zoneid = it->first;
      zone_info.tag = "rr_REMAIN_1";
      MCE_DEBUG("LINE: " << __LINE__ << " request_zones[" << it->first << "]: zone_info.zoneid = " << zone_info.zoneid << ", zone_info.tag = " << zone_info.tag);
      request_zones[it->first] = zone_info;
    } else if(!it->second.empty() && (it->second.at(0).groupid == -3)) {
      //余量
      self_zones[it->first] = ENGINE;
      tag = "rr_REMAIN_2";
      photo = "true";
      MCE_DEBUG("photo = true");
      ZoneInfo zone_info;
      zone_info.zoneid = it->first;
      zone_info.tag = "rr_REMAIN_2";
      MCE_DEBUG("LINE: " << __LINE__ << " request_zones[" << it->first << "]: zone_info.zoneid = " << zone_info.zoneid << ", zone_info.tag = " << zone_info.tag);
      request_zones[it->first] = zone_info;
    }
  }

  if (!self_zones.empty()) {
    if(!DegradeManager::instance().IsEnable("engine_u")){
      MCE_INFO("engine_u is disabled"); 
      err_flag = true;
    }else{
      try {
        MCE_DEBUG("AdGetRequest::response --> from AdEngineU rotate_slot = " << rotate_slot);
        AdZone2Creatives e_ads;
        e_ads = AdEngineUReplicaAdapter::instance().GetAdsByZoneInfo(serialized, log_url.str(), rotate_slot, request_zones, _login_uid, photo, count, GateConfig::instance().env_test());
        if (e_ads.empty()) {
          MCE_INFO("AdGetRequest::response --> from AdEngineU empty: uid = "
              << _login_uid << ", referer = " << log_url.str()
              << ", tag = " << tag << ", photo = " << photo);
        } else {
          for(AdZone2Creatives::iterator xxit =  e_ads.begin(); xxit != e_ads.end(); xxit++) {
            MCE_DEBUG("AdGetRequest::response --> from AdEngineU " << xxit->second.size());
          }
        }
        AdGateI::instance().translate(ads,final_ads);
        for (map<long, EngineTypes>::iterator zit = self_zones.begin(); zit != self_zones.end(); zit++) {
          AdZone2Creatives::iterator eit = e_ads.find(zit->first);
          if (eit != e_ads.end()) {
            final_ads[zit->first] = eit->second;
          } else {
            final_ads[zit->first] = AdCreativeSeq();
          }
        }
      } catch (Ice::Exception& e) {
        MCE_WARN("AdEngineUReplicaAdapter::GetAds error : " << e << " uid=" << _login_uid);
        err_flag = true;
      } catch (std::exception& e) {
        MCE_WARN("AdEngineUReplicaAdapter::GetAds error : " << e.what() << " uid=" << _login_uid);
        err_flag = true;
      }
    }
//      }
  } else {
    AdGateI::instance().translate(ads,final_ads);
  }
  if (err_flag) {
    stringstream stat_desc;
    stat_desc << _login_uid << " PushDefault";
    StatFunc stat(stat_desc.str());
    AdCreativeCache::instance().PushDefault(_login_uid, profile.stage(), GetRemoteAddr(),
        referer, &ads);
    AdGateI::instance().translate(ads,final_ads);
  }

  stringstream ad_header;
  string ad_body;
  if (render_ == RT_HTML) {
    ad_header << "Content-type: text/html; charset=utf-8\r\nSet-Cookie: an_slot=" << ++rotate_slot;
    if(_path == "/ebpt/show") {
      ad_header << "; path=/ebpt\r\n\r\n";
    } else {
      ad_header << "; path=/ebpn\r\n\r\n";
    }
    //AdCreativeCache::instance().Get(ads, &ad_body);
    AdCreativeCache::instance().Get(final_ads, &ad_body);
  } else if (render_ == RT_JSON) {
    ad_header
        << "Content-type: application/x-javascript; charset=utf-8\r\nSet-Cookie: an_slot="
        << ++rotate_slot;
    if(_path == "/ebpt/show") {
      ad_header << "; path=/ebpt\r\n\r\n";
    } else {
      ad_header << "; path=/ebpn\r\n\r\n";
    }
    stringstream stat_desc;
    stat_desc << _login_uid << " GetJson";
    StatFunc stat(stat_desc.str());
    AdCreativeCache::instance().GetJson(referer,
                                        getPropertyInt("refresh_source", 0),
                                        getPropertyInt("refresh_idx", 0),
                                        self_zones,
                                        final_ads,
                                        &ad_body);
  }

  FCGX_PutS(ad_header.str().c_str(), _fcgi_out);
  FCGX_PutS(ad_body.c_str(), _fcgi_out);
  timeval end;
  gettimeofday(&end, NULL);
  AdGateI::instance().ping_sender_.Send("GetAds", 1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000);
  return true;
}

bool AdTestRequest::response() {
  MCE_DEBUG("AdTestRequest::response");
  if (!_fcgi_out) {
    MCE_WARN("AdTestRequest::response fcgi_out null");
    return false;
  }

  string uid = getProperty("uid");
  if (_cookies.find("id") != _cookies.end()) {
    uid = _cookies["id"];
  }
  if (!uid.empty()) {
    try {
      _login_uid = boost::lexical_cast<int>(uid);
    } catch (...) {
      MCE_WARN("lexical_cast cookie id error : " << uid);
    }
  }

  uid = getProperty("uid");
  string gender = getProperty("gender");
  string age = getProperty("age");
  string stage = getProperty("stage");
  string area = getProperty("area");
  string school = getProperty("school");
  string school_area = getProperty("school_area");
  string work_area = getProperty("work_area");
  string home_area = getProperty("home_area");
  string grade = getProperty("grade");
  string ip = getProperty("ip");
  string ip_area = getProperty("ip_area");
  string referer = getProperty("ref");
  MCE_DEBUG("AdTestRequest --> referer=" << referer << ",uid:" << uid
      << ", ip:" << ip << ", ip_area = " << ip_area);

  //UserID|Gender|Birthday|Age|Stage|Area|School_SchoolArea|Grade|IPArea

  UserProfile profile;
  //profile.set_ip_area(IpAreaCache::instance().GetIpArea(ip));
  profile.set_ip_area(ip_area);

  try {
    profile.set_id(boost::lexical_cast<int>(uid));
  } catch (...) {
  }

  try {
    profile.set_stage(boost::lexical_cast<int>(stage));
  } catch (...) {
  }

  try {
    profile.set_home_area(home_area);
  } catch (...) {
  }

  try {
    profile.set_gender(boost::lexical_cast<int>(gender));
  } catch (...) {
  }

  try {
    profile.set_age(boost::lexical_cast<int>(age));
  } catch (...) {
  }

  if (stage == "4") {
    profile.set_current_area(work_area);
    profile.set_school(0);
    profile.set_grade(0);
  } else if (stage == "2" || stage == "1") {
    profile.set_current_area(school_area);
    try {
      profile.set_school(boost::lexical_cast<Ice::Long>(school));
      profile.set_grade(boost::lexical_cast<int>(grade));
    } catch (...) {
    }
  }

  string serialized;
  profile.SerializeToString(&serialized);

  int rotate_slot = GetCookieInt("an_slot", rand());
  rotate_slot %= 10000;

  bool hide = false;
  string isvip = getProperty("isvip");
  string hideads = getProperty("hideads");
  if(isvip.compare("true")==0 && hideads.compare("true")==0) {
    hide = true;
  }

  AdResultMap ads;
  // string referer = GetParamFix("HTTP_REFERER");
  try {
    ads = AdEngineBReplicaAdapter::instance().GetAds(serialized, referer, rotate_slot, hide);
  } catch (Ice::Exception& e) {
    MCE_WARN("AdEngineBReplicaAdapter::GetAds error : " << e << " uid=" << uid);
  } catch (std::exception& e) {
    MCE_WARN("AdEngineBReplicaAdapter::GetAds error : " << e.what() << " uid=" << uid);
  }
  for (AdResultMap::iterator it = ads.begin(); it != ads.end(); ++it) {
    for (size_t i = 0; i < it->second.size(); ++i) {
      if (it->second.at(i).creativeid == 0) {
        MCE_WARN("test, creativeid is 0");
      }
    }
  }

  MCE_INFO("AdEngineBReplicaAdapter::GetAds return size : " << ads.size() << " uid="
      << uid << " stage=" << profile.stage() << " ip_area="
      << profile.ip_area());

  string ad_json;
  string ad_body;

  AdCreativeCache::instance().Get(ads, &ad_body);
  ad_body = ad_body + "<p/>" + ad_json;
  MCE_DEBUG("AdTestRequest::response --> uid:" << uid << " ,body:" << ad_body);
  ostringstream ad_header;
  ad_header << "Content-type: text/html; charset=utf-8\r\nSet-Cookie: an_slot="
      << ++rotate_slot << "; path=/ebpn\r\n\r\n";
  FCGX_PutS(ad_header.str().c_str(), _fcgi_out);
  FCGX_PutS(ad_body.c_str(), _fcgi_out);

  return true;
}

bool AdClickRequest::response() {
  timeval start;
  gettimeofday(&start, NULL);
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }

  string uid = getProperty("uid");
  if (_cookies.find("id") != _cookies.end()) {
    uid = _cookies["id"];
  }
  if (!uid.empty()) {
    try {
      _login_uid = boost::lexical_cast<int>(uid);
    } catch (...) {
      MCE_WARN("lexical_cast cookie id error : " << uid);
    }
  }
  string referer = getProperty("ref");
  if (referer.empty() || referer.find("http://") == string::npos) {
    referer = getProperty("r");
    if (referer.empty() || referer.find("http://") == string::npos) {
      referer = GetParamFix("HTTP_REFERER");
    }
  }

  string matcher_cookie = getProperty("mc");
  string url = getProperty("url");

  stringstream log_url;
  try {
    char sep = '&';
    if (url.find_first_of('?') == string::npos) {
      sep = '?';
    }
    int refresh_source = getPropertyInt("refresh_source", 0);
    int refresh_idx = getPropertyInt("refresh_idx", 0);
    log_url << url << sep << "refresh_source=" << refresh_source
        << "&refresh_idx=" << refresh_idx;
  } catch (Ice::Exception& e) {
    MCE_WARN("AdClickRequest::response error : " << e << " uid=" << _login_uid);
  } catch (std::exception& e) {
    MCE_WARN("AdClickRequest::response error : " << e.what() << " uid="
        << _login_uid);
  }

  EngineTypes etype = (EngineTypes) getPropertyInt("engine_type");
  if (etype == ENGINE) {
    try {
      AdEngineUReplicaAdapter::instance().Click(_login_uid, GetRemoteAddr(), matcher_cookie, referer, log_url.str(), GateConfig::instance().env_test());
    } catch (Ice::Exception& e) {
      MCE_WARN("AdEngineUReplicaAdapter::Click error : " << e << " uid=" << _login_uid);
    } catch (std::exception& e) {
      MCE_WARN("AdEngineUReplicaAdapter::Click error : " << e.what() << " uid="
          << _login_uid);
    }
  } else {
    try {
      AdEngineBReplicaAdapter::instance().Click(_login_uid, GetRemoteAddr(),
          matcher_cookie, referer, log_url.str());
    } catch (Ice::Exception& e) {
      MCE_WARN("AdEngineBReplicaAdapter::Click error : " << e << " uid=" << _login_uid);
    } catch (std::exception& e) {
      MCE_WARN("AdEngineBReplicaAdapter::Click error : " << e.what() << " uid="
          << _login_uid);
    }

  }

  MCE_INFO("click uid:" << _login_uid << " location:" << url);
  string output = "Location: " + url + "\r\n\r\n";
  FCGX_PutS(output.c_str(), _fcgi_out);
  timeval end;
  gettimeofday(&end, NULL);
  AdGateI::instance().ping_sender_.Send("Click", 1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000);
  return true;
}

bool AdCloseRequest::response() {
  timeval start;
  gettimeofday(&start, NULL);
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }

  string uid;
  if (_cookies.find("id") != _cookies.end()) {
    uid = _cookies["id"];
  }
  
  if(uid.empty()){
    MCE_WARN("AdCloseRequest::response cannot get uid from cookie");
    return false;
  }

  try {
    _login_uid = boost::lexical_cast<int>(uid);
  } catch (...) {
    MCE_WARN("lexical_cast cookie id error : " << uid);
    return false;
  }

  string creative_str = getProperty("cid");
  Ice::Long creative_id = 0;
  try {
    creative_id = boost::lexical_cast<Ice::Long>(creative_str);
  } catch(...) {
    MCE_WARN("lexical_cast creative_str error : " << creative_str);
    return false;
  }
  stringstream log_url;

  try {
    IndexServiceReplicaAdapter::instance().Close(_login_uid, creative_id);
  } catch (Ice::Exception& e) {
    MCE_WARN("IndexServiceReplicaAdapter::Close error : " << e << " uid=" << _login_uid);
  } catch (std::exception& e) {
    MCE_WARN("IndexServiceReplicaAdapter::Close error : " << e.what() << " uid=" << _login_uid);
  }catch(...){
    MCE_WARN("IndexServiceReplicaAdapter::Close caught unknown error");
  }

  MCE_INFO("close ad uid = " << _login_uid << ", ad = " << creative_str);

  string output = "Content-type: text/html; charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(), _fcgi_out);
  timeval end;
  gettimeofday(&end, NULL);
  AdGateI::instance().ping_sender_.Send("Close", 1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000);
  return true;
}

void AdGateI::translate( AdResultMap & map, AdZone2Creatives & adzone2c ){
  for(AdResultMap::const_iterator it = map.begin(); it != map.end(); ++ it) {
    AdInfoSeq ad_info_seq = it->second;
    AdCreativeSeq ad_c_seq;
    int abs_pos =0 ;
    for(AdInfoSeq::const_iterator ad_it = ad_info_seq.begin() ; ad_it != ad_info_seq.end() ; ++ ad_it){
      AdCreativeInfo ac ;
      ac.widgetid = 0;
      ac.creativeid = ad_it->creativeid;
      ac.groupid = ad_it->groupid;
      ac.text  = ad_it->text;
      ac.extra = "";
      ac.position = ++abs_pos;
      ad_c_seq.push_back(ac);
    }
    if(ad_c_seq.size()>0){
      adzone2c[it->first] = ad_c_seq;
    }
  }
}
