/*
 * EdmSendManagerI.cpp
 *
 *  Created on: 2012-2-16
 *      Author: sunzz
 */

//#include <boost/asio/ip/address_v4.hpp>
//#include <arpa/inet.h>
#include "ValidateCheck.h"
#include "FrequencyCheck.h"
#include "IpAreaCache.h"
#include "AdUserCacheLoaderAdapter.h"
#include <sstream>
#include <ctime>
#include "EdmSendManagerI.h"
#include "LogWrapper.h"
#include "Date.h"
#include "ServiceI.h"
#include "client/linux/handler/exception_handler.h"
#include "feed/FeedMemcProxy/client/user_profile_client.h"
#include "../../../util/src/AdCommonUtil.h"
#include "EdmStructCache.h"
//#include "Notify/util/fcgi_request.h"
#include <boost/asio/ip/address_v4.hpp>
#include <arpa/inet.h>
#include "ParametersController.h"
#include "SelectHandler.h"
#include "SendHandler.h"
#include "UpdateUserInfoHandler.h"
#include "LogHandler.h"
#include "FeedCreatorAdapter.h"
#include "BrandKeeper.h"
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
//#include "ad/ping_client/src/PingSender.h"
//#include  "FeedEdmSenderAdapter.h"
using namespace com::xiaonei::xce;
using namespace xce::ad;
using namespace xce::notify;

using namespace boost;
using namespace MyUtil;
using namespace std;

const string COLON = ":";
const string SPLIT = ";";

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

class EdmRequestFactory : public RequestFactory {
  public:
    virtual RequestPtr Create(FCGX_Request * r) {
      char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
      if (path) {
        if (strcmp(path, "/click") == 0) {
          MCE_DEBUG("Click request");
          return new EdmClickRequest(r);
        } else {
          return new OtherRequest(r);
        }
      }   
      return NULL;
    }
};

void MyUtil::initialize() {
  //google breakpad for dump
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  // service start
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&xce::ad::AdEdmSenderI::instance(), service.createIdentity("ADEDM", ""));

  MyUtil::TaskManager::instance().schedule(new xce::ad::EngineConfig::ConfigLoadTimer(0));
  TaskManager::instance().execute(new InitTask());
  TaskManager::instance().execute(new xce::ad::FrequencyCacheLoadTask()); //频率监测cache加载数据

  TaskManager::instance().schedule(new EngineParametersTimer()); 
  TaskManager::instance().scheduleRepeated(new BrandKeeperRefreshParametersTimer()); 
  TaskManager::instance().execute(new IpAreaCacheReloadTask());
  /*
  BrandKeeper::instance().Load(); 
  TaskManager::instance().scheduleRepeated(new BrandKeeperLoadTimer()); 
  */
  MCE_INFO("MyUtil::initialize start ");
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  string fcgi_socket = props->getPropertyWithDefault("Service."
      + service.getName() + ".FcgiSocket", "0.0.0.0:9001");
  MCE_INFO("Fcgi listens on : " << fcgi_socket);

  // fcgi_server memory managed as RequestFactory ...
  FcgiServer * fcgi_server = new FcgiServer(fcgi_socket, 256, 256 * 1024);
  fcgi_server->RegisterRequestFactory(new EdmRequestFactory());
  fcgi_server->Start();

  /*int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdEdmSender.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdEdmSender.Interval",5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdEdmSender",&AdEdmSenderI::instance(),mod,interval,new EdmChannel());
  MCE_INFO("MyUtil::initialize.mod:"<<mod<<" interval:"<<interval);*/
}
AdEdmSenderI::AdEdmSenderI() {//各个模块添加
  edm_match_handler_list_.push_back(new SelectHandler());
  edm_match_handler_list_.push_back(new SendHandler());
  edm_match_handler_list_.push_back(new LogHandler());
  edm_match_handler_list_.push_back(new UpdateUserInfoHandler());

  init_ok_ = false;

  MCE_INFO("AdEdmSenderI::Create --> The Engine started.");
}

void AdEdmSenderI::SendEdmTest(int user_id, const MyUtil::Str2StrMap& params, const Ice::Current&){
  //time_t send_time = time(NULL);
  //params["time"] = boost::lexical_cast<std::string>(send_time) + "000";
  //params["userId"] = boost::lexical_cast<std::string>(user_id);
  /*Str2StrMap::const_iterator it = params.find("titlelink"); 
  if (params.end() != it){
      LogHelper::instance().ProcessUrl(it->second);
  }
  it = params.find("bodyLink");
  if (params.end() != it){
      LogHelper::instance().ProcessUrl(it->second);
  }*/
  MCE_INFO("SendEdmTest::yulei");
  Str2StrMap::const_iterator it = params.find("type");
  string edm_type_str;
  if (params.end() != it){
      edm_type_str = it->second;//TODO
  }
  //新鲜事的分发表达式
  ostringstream expression;
  //expression << "u(229942474)";
 // expression << "u(" << user_id << ")";
  //params["expr"] = expression.str();
  int edm_type_number = 8001;
  try {
    edm_type_number = boost::lexical_cast<int>(edm_type_str);
  } catch(std::exception& e) {
    MCE_WARN("SendHandler::BuildEdmAndSend-->"<<edm_type_str<<"-,error:"<<e.what());
  } catch(...){
    MCE_WARN("SendHandler::BuildEdmAndSend->Unknown error");
  }
  MCE_DEBUG("edm_type_number:" << edm_type_number);

  stringstream ss;
  MyUtil::Str2StrMap::const_iterator mIter = params.begin();
  for (; mIter != params.end(); ++mIter){
    ss << mIter->first << COLON << mIter->second << SPLIT;
  }
  MCE_DEBUG(ss.str());
  //int edm_type_number = 8002; //新鲜事小类型号 如 8002 从edm信息中获取
  const int edm_version = 1; //版本号 目前就是1
  try {
    //bool edm_send_success = 1;
    bool edm_send_success = xce::feed::FeedCreatorAdapter::instance().CreateEDM(edm_type_number,edm_version,params);
    if (!edm_send_success) {
      MCE_WARN("edm send fail : user_id : "<<user_id);
    } else {
      MCE_DEBUG("edm sended successfully : user_id : "<<user_id);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("call FeedCreatorAdapter error -> "<<e);
  } catch (std::exception& ex) {
    MCE_WARN("call FeedCreatorAdapter exception error -> "<<ex.what());
  } catch (...) {
    MCE_WARN("call FeedCreatorAdapter failed"); 
  }
}

bool IsForOldEngine(int user_id) {
/*  bool is_for_old_engine = false;
  string value = EngineParametersCache::instance().GetParameterByName("is_for_old_edm_engine");
  MCE_DEBUG("IsForOldEngine --> key = is_for_old_engine," << " value = " << value);
  if (!value.empty()) {
    try{
      is_for_old_engine = boost::lexical_cast<bool>(value);
    } catch (...){
      MCE_WARN("IsForOldEngine --> is_for_old_engine error.");
    }   
  }
  return is_for_old_engine;*/
  bool is_for_old_engine = false;
  int for_old_ratio = 100;
  string ratio = EngineParametersCache::instance().GetParameterByName("switch_old_edm_engine_ratio");
  MCE_DEBUG("IsForOldEngine --> key = for_old_engine_ratio," << " value = " << ratio);
  if (!ratio.empty()) {
    try{
      for_old_ratio = boost::lexical_cast<int>(ratio);
    } catch (...){
      MCE_WARN("IsForOldEngine --> is_for_old_engine error.");
    }   
  }
  if(user_id % 100 < for_old_ratio) {
    is_for_old_engine = true;
  }
  return is_for_old_engine;
}

void AdEdmSenderI::SendEdm(int user_id, const MyUtil::Str2StrMap& params, const Ice::Current&){
  MCE_INFO("AdEdmSenderI::SendEdm-->user_id:" << user_id);
  /*if(IsForOldEngine(user_id )){
    try{
      xce::edm::FeedEdmSenderAdapter::instance().Receive(user_id, params);
      MCE_INFO("AdEdmSenderI::SendEdm-->user_id:" << user_id << ",need trans to old engine");
    } catch (Ice::Exception& e) {
      MCE_WARN("AdEdmSenderI::SendEdm--> error : " << e << " uid = " << user_id);
    } catch (std::exception& e) {
      MCE_WARN("AdEdmSenderI::SendEdm--> error :" << e.what() << " uid = " << user_id);
    } catch (...) {
      MCE_WARN("AdEdmSenderI::SendEdm--> error : " << " uid = " << user_id);  
    }
    return;
  }*/
  timeval start;
  gettimeofday(&start, NULL);

  if(!init_ok_){
    MCE_INFO("AdEdmSenderI::SendEdm() --> initing.......");
    return ;
  }
  ///////////////////////////////////测试控制//TODO上线前删除
  bool check_flag = true;
  int user_status_set = -1;
  Str2StrMap::const_iterator itp = params.find("check_flag");
  if(itp != params.end()) {
    check_flag = boost::lexical_cast<bool>(itp->second);
  }
  itp = params.find("user_status_set");
  if(itp != params.end()) {
    user_status_set = boost::lexical_cast<int>(itp->second);
  }

///////////////////////////////////////////////////////
  int user_status = FrequencyCache::instance().CheckFrequencyLimit(user_id);
  if (check_flag) {//如果请求不需要检查，则直接跳过检查//TODO上线前删除
    if (0 == user_status) { //不合法状态
      MCE_DEBUG("AdEdmSenderI::SendEdm-->invalid user_status :" << user_status);
      return;
    }
    //call User check module, 检查User info，如果不能看，不参与后续edm选择
    if (!ValidateCheckCache::instance().CheckUserValid(user_id)) { //无效用户不发送edm,并插入有效用户脚印
      MCE_DEBUG("AdEdmSenderI::SendEdm-->invalid user_id:" << user_id);
      return;
    }
  }
///////////////////TODO 上线前删除
  if(user_status_set >= 0) {//如果请求设定用户状态，则采用设定状态
    user_status = user_status_set;
  }
////////////////////////////////////////////////////////////
  //获取用户信息
  UserProfile profile;
  GetUserInfo(user_id, profile, params);
  //构建全局参数，为选择edm准备数据
  MatchHandlerParamter para(profile, user_status);

  /*完成如下步骤：
  1、广告选择模块，选择出投放广告
  2、组装edm新鲜事模板，发送edm
  3、打印log
  4、更新用户相关信息*/
  for(size_t i = 0; i < edm_match_handler_list_.size(); ++i) {
    edm_match_handler_list_[i]->handle(para);
  }

  timeval end;
  gettimeofday(&end, NULL);
  MCE_INFO("AdEdmSender select edm spend time:" << (1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000));
  return;
}

void AdEdmSenderI::ClearLoginCacheByUser(Ice::Int user_id, const Ice::Current&) {
  ValidateCheckCache::instance().RemoveUserFootPrint(user_id);
  MCE_DEBUG("ClearLoginCacheByUser user_id : " << user_id);
}

//void AdEdmSenderI::UserInfoObain(int user_id,UserProfile &profile) { 
void AdEdmSenderI::GetUserInfo(int user_id, UserProfile &profile, const MyUtil::Str2StrMap& params){
  string serialized;
  if (user_id > 0) {
    bool get_serialized_success = false;
    try {
      StatFunc stat("memc GetSerialized");
      get_serialized_success = UserProfileClient::instance().GetSerialized(user_id, &serialized);
    } catch (std::exception& e) {
      MCE_WARN("UserProfileClient error : " << e.what() << " uid = " << user_id);
    } catch (...) {
      MCE_WARN("UserProfileClient error : " << " uid = " << user_id);
    }
    if (!get_serialized_success) {
      MCE_INFO("profile not found in memcache. id=" << user_id);
      try {
        StatFunc stat("memc AdUserCacheLoader");
        serialized = AdUserCacheLoaderAdapter::instance().GetById(user_id);
      } catch (Ice::Exception& e) {
        MCE_WARN("AdUserCacheLoaderAdapter GetById error : " << e << " uid = " << user_id);
      } catch (std::exception& e) {
        MCE_WARN("AdUserCacheLoaderAdapter GetById error : " << e.what() << " uid = " << user_id);
      } catch (...) {
        MCE_WARN("AdUserCacheLoaderAdapter GetById error : " << " uid = " << user_id);  
      }
    }
  }
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
  string user_ip;  
  Str2StrMap::const_iterator itp = params.find("ip");
  if(itp != params.end()) {
    user_ip = itp->second;
    MCE_DEBUG("user ip : " << user_ip);
  }
  unsigned int ip = ntohl(inet_addr(user_ip.c_str()));
  //unsigned int ip = ntohl(inet_addr(GetRemoteAddr().c_str()));
  profile.set_id(user_id);
  profile.set_ip(ip);
  profile.set_ip_area(IpAreaCache::instance().GetIpArea(ip));
    MCE_DEBUG("user ip_area : " << profile.ip_area());
}
/*
string SendHandler::GetMD5(const string &str){
  MD5_CTX ctx;
  char *data = (char *)str.c_str();
  unsigned char md[16];
  char tmp[3] = {'\0'};
  string ans = "";
  MD5_Init(&ctx);
  MD5_Update(&ctx,data,strlen(data));
  MD5_Final(md,&ctx);
  for (int i=0;i<16;++i) {
    sprintf(tmp,"%02x",md[i]);
    ans += tmp;
  }
  return ans;
}*/

void AdEdmSenderI::Init(){
  TimeStat ts;
  MCE_INFO("AdEdmSenderI::Init --> begin init :" << Date(time(NULL)).toDateTime());
  try {
    AdMemberPool::instance().Init();
    MCE_INFO("AdEdmSenderI::Init --> AdMemberPool::instance().Init(); DONE");
    AdCampaignPool::instance().Init();
    MCE_INFO("AdEdmSenderI::Init -->  AdCampaignPool::instance().Init(); DONE");
    AdGroupPool::instance().Init();
    MCE_INFO("AdEdmSenderI::Init -->  AdGroupPool::instance().Init(); DONE");
  } catch (std::exception& e) {
    MCE_WARN("AdEdmSenderI::Init()-->  ERR  line:" << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("AdEdmSenderI::Init()--> reload ERR  line:" << __LINE__);
  }  

}

void InitTask::handle(){
  try {
  SchoolPool::instance().Init();
  MCE_INFO("AdEdmSenderI::Init --> SchoolPool::instance().Init(); DONE");
  AdEdmSenderI::instance().Init();
  MCE_INFO("AdEdmSenderI::Init --> AdEdmSenderI::instance().Init(); DONE");
  } catch (std::exception& e) {
    MCE_WARN("AdEdmSenderI::Init()-->  ERR  line:" << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("AdEdmSenderI::Init()--> reload ERR  line:" << __LINE__);
  }  
  AdEdmSenderI::instance().InitOk();

  usleep(60 * 1000 * 1000);
  TaskManager::instance().execute(new ReloadTask());
}

void ReloadTask::handle(){
  while(true) {
    try {
      MCE_INFO("ReloadTask::handle --> BEGIN RELOAD");
      AdEdmSenderI::instance().Init();
      MCE_INFO("ReloadTask::handle --> END RELOAD");
      usleep(60 * 1000 * 1000);
    } catch (std::exception& e) {
      MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__ << " err:" << e.what());
    } catch (...) {
      MCE_WARN("ReloadTask--> reload ERR  line:" << __LINE__);
    }
  }
}

bool EdmClickRequest::response() {
  //timeval start;
  //gettimeofday(&start, NULL);
  if (!_fcgi_out) {
    MCE_WARN("fcgi_out null");
    return false;
  }
  //获取点击用户的uid
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
 
  //获取redirect url并进行302跳转
  string url = getProperty("url");
  MCE_INFO("click uid:" << _login_uid << " location:" << url); 
  string output = "Location: " + url + "\r\n\r\n"; 
  FCGX_PutS(output.c_str(), _fcgi_out); 
  
  //TO DO 点击Log处理
  string matcher_cookie = getProperty("mc");
  MCE_DEBUG("log md5: click log_str :" << matcher_cookie);
  vector<string> para_strs;
  boost::algorithm::split_regex(para_strs,matcher_cookie,regex("\\^B"));
  if (para_strs.size() > 1 ) {
    MCE_DEBUG("log md5: para_strs[0] :"<<para_strs[0]);
    MCE_DEBUG("log md5: para_strs[1] :"<<para_strs[1]);
   string md5_str = LogHelper::instance().GetMD5(para_strs[0]);
    MCE_DEBUG("log md5: md5_str:"<<md5_str);
    if (para_strs[1] == md5_str) {
      LogHelper::instance().Click(_login_uid, para_strs[0]);
    }
  }
  //timeval end; 
  //gettimeofday(&end, NULL); 
  return true; 
}

bool OtherRequest::response() {
  //MCE_INFO("FCGI:Other request");
  string output = "Location: \r\n\r\n"; 
  FCGX_PutS(output.c_str(), _fcgi_out); 
}
 
