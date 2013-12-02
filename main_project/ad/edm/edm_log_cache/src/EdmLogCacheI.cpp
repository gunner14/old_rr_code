#include "EdmLogCacheI.h"
#include "EdmLogCacheDBManager.h"
#include "LogWrapper.h"
#include <boost/lexical_cast.hpp>
#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "../../../util/src/AdCommonUtil.h"
#include "client/linux/handler/exception_handler.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "Date.h"
#include "AdEdmLogAdapter.h"
#include "AdEdmLogLeftAdapter.h"
using namespace mysqlpp;

using namespace std;
using namespace boost;
using namespace xce::ad;
using namespace com::xiaonei::xce;

const unsigned int ONE_MINUTE = 60 * 1000 * 1000; 
const unsigned int FIFTEEN_DAYS = 15 * 24 * 3600;
const unsigned int ONE_DAY = 1 * 24 * 3600;
const unsigned int ZERO = 0;
const unsigned int THREE_CLOCK = 3;

const string TABLE_NAME = "edm_log";
const static string MACHINE_INFO = "AdEdmSender0@127.0.0.1";

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  //google breakpad for dump
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  // service start
  ServiceI& service = ServiceI::instance();
  EdmLogCacheDBManager::instance().SetDB(service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance", "edmsend_db"));
  service.getAdapter()->add(&EdmLogCacheI::instance(), service.createIdentity("ADEDM", ""));

  TaskManager::instance().execute(new EvictTask());
  EdmLogCacheDBManager::instance().Init();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("EdmLogCache.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("EdmLogCache.Interval",5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerEdmLogCache",&EdmLogCacheI::instance(),mod,interval,new EdmChannel());
  MCE_INFO("MyUtil::initialize start ");
}


EdmLogCacheI::EdmLogCacheI(){
  last_clear_time_ = GetExpireTime(time(NULL), ONE_DAY);
  MCE_INFO("EdmLogCacheI::EdmLogCacheI() --> last_clear_time: " << last_clear_time_);
}

time_t EdmLogCacheI::GetExpireTime(time_t expire_time, const time_t days){
  expire_time += days;
  struct tm time;
  localtime_r(&expire_time, &time);

  time.tm_hour = THREE_CLOCK;
  time.tm_min = ZERO;
  time.tm_sec = ZERO;
  return mktime(&time);
}

/*
 * Add 由 LogHandler::handle调用，LogHandler::handle函数首先组装SendLog并且发送后，旧EDM提取ClickLog和ExposeLog公共log为value，
 * 推左EDM除了保存ExposeLog还需保存pv charge log以及click log组合为value,
 * 以userID_adgroupID为key，调用Add接口存储在EdmLogCache服务的cache中，并且以异步方式写入DB中持久化。
 */
void EdmLogCacheI::Add(const string& key, const string& value, const Ice::Current&){
  time_t expire_time = GetExpireTime(time(NULL), FIFTEEN_DAYS);
  AddInCache(key, value, expire_time);

  expire_time = time(NULL) + FIFTEEN_DAYS;
  DBEntryPtr ptr(new DBEntry(key, value, expire_time));
  EdmLogCacheDBManager::instance().Insert(ptr);
}

/*
 * AddInCache 将log数据存储到cache中
 */
void EdmLogCacheI::AddInCache(const string& key, const string& value, const time_t& expire_time){
  {
    IceUtil::RWRecMutex::WLock lock(log_mutex_);
    list<Entry>::reverse_iterator rit = log_cache_.rbegin();
    for (; rit != log_cache_.rend(); ++rit) {
      if (rit->GetTime() == expire_time) {
        break;
      }
    }
    if (rit == log_cache_.rend()) {
      Entry entry;
      entry.SetTime(expire_time);
      entry.InsertLog(key, value);
      log_cache_.push_back(entry);
    } else {
      rit->InsertLog(key, value);
    }
  }

}

/*
 * key为userID_adgroupID，如果cache中存在此key，则直接返回value(log)，
 * 如果不存在则从db中查询，如果db中存在则返回，否则返回空。
 */
string EdmLogCacheI::Get(const string& key) {
  string value;
  {
    IceUtil::RWRecMutex::RLock lock(log_mutex_);
    list<Entry>::reverse_iterator rit = log_cache_.rbegin();
    for (; rit != log_cache_.rend(); ++rit) {
      value = rit->FindLog(key);
      if (!value.empty()) {
        return value;
      }
    }
  }

  value = EdmLogCacheDBManager::instance().GetFromDB(key);  

  if (value.size() > 0) {
    time_t expire_time = GetExpireTime(time(NULL), FIFTEEN_DAYS);
    AddInCache(key, value, expire_time);
  } 
  return value;
}

/*
 * Expose接口供AdEdmSender中Expose调用，如果获取到log，则直接调用AdEdmLog服务打印log,
 * 否则打印错误;
 */
void EdmLogCacheI::Expose(const string& key, Ice::Int index, const Ice::Current&) {
  string value = Get(key);
  if (!value.empty()) {
    string edm_expose_log_substr;
    string left_click_log_substr;
    string left_cpc_charge_substr;
    Process::ProcessLogStr(value, edm_expose_log_substr, left_click_log_substr, left_cpc_charge_substr);

    if ( true == edm_expose_log_substr.empty() ) { 
      MCE_WARN("EdmLogCacheI::Expose --> Parse Expose Log failed! key: " << key);
      return;
    }

    ostringstream expose_log;
    MyUtil::Date date(time(NULL));
    expose_log << MACHINE_INFO << "," << edm_expose_log_substr << "," << date.toDateTime() << "," << index;
    try {
      AdEdmLogAdapter::instance().Expose(expose_log.str());  
    } catch (Ice::Exception& e) { 
      MCE_WARN("EdmLogCacheI::Expose --> call AdEdmLogAdapter error -> "<< e); 
      return;
    } catch (std::exception& ex) {
      MCE_WARN("EdmLogCacheI::Expose --> call AdEdmLogAdapter exception error -> "<< ex.what());
      return;
    } catch (...) {
      MCE_WARN("EdmLogCacheI::Expose --> call AdEdmLogAdapter failed");
      return;
    }
  }  
}

/*
 * Click接口供EAdEdmSender服务的response接口调用，负责记录用户点击log
 * 以userID_adgroupID为key，如果num为1则为旧EDM点击log，直接调用AdEdmLog服务记录log.
 * 如果num为3为推左log，直接调用AdEdmLeftLog服务记录log
 */
void EdmLogCacheI::Click(const string& key, const Ice::Current&) {
  string value = Get(key);
  if (!value.empty()){
    ostringstream click_log;

    string edm_click_log_substr;
    string left_click_log_substr;
    string left_cpc_charge_substr;
    int num = Process::ProcessLogStr(value, edm_click_log_substr, left_click_log_substr, left_cpc_charge_substr);

    MyUtil::Date date(time(NULL));
    if ( 1 == num ) {
      click_log << MACHINE_INFO << "," << edm_click_log_substr
        << "," << date.toDateTime() << ",";
      try {
        AdEdmLogAdapter::instance().Click(click_log.str());
      } catch (Ice::Exception& e) { 
        MCE_WARN("EdmLogCacheI::GetClick --> call AdEdmLogAdapter error -> "<< e);
      } catch (std::exception& ex) {
        MCE_WARN("EdmLogCacheI::GetClick --> call AdEdmLogAdapter exception error -> "<< ex.what());
      } catch (...) {
        MCE_WARN("EdmLogCacheI::GetClick --> call AdEdmLogAdapter failed");
      }
    } else if ( 3 == num ) {  //有推左log串
      Process::ProcessLeftClickLog(left_click_log_substr);
      Process::ProcessLeftChargeLog(left_cpc_charge_substr);
    } else {
      MCE_INFO("EdmLogCacheI::GetClick --> invalid edm log strs, num is " << num );
    }

  }// if (value.size() > 0)
}

/*
 * 如果当前时间是凌晨3点，则开始启动cache中逐出策略
 */
bool EdmLogCacheI::IsEvict() {
  time_t now = time(NULL);
  if (now < last_clear_time_) {
    return false;
  }
  MCE_INFO("EdmLogCacheI::IsEvict evict start time: " << now);
  return true;
}

/*
 * 逐出cache中失效数据后，更新下次逐出时间为明天凌晨3点
 */
void EdmLogCacheI::UpdateLastClearTime() {
  last_clear_time_ = GetExpireTime(time(NULL), ONE_DAY);
  MCE_INFO("EdmLogCacheI::UpdateLastClearTime --> next evict time: " << last_clear_time_);
}

/*
 * 从cache中逐出失效数据，最多保留15天
 */
void EdmLogCacheI::Evict() {
  MCE_INFO("EdmLogCacheI::Evict() --> before evict size: " << log_cache_.size());
  time_t now = time(NULL);
  IceUtil::RWRecMutex::WLock lock(log_mutex_);
  list<Entry>::iterator it = log_cache_.begin();
  for (; it != log_cache_.end();) {
    if (now > it->GetTime()) {
      log_cache_.erase(it++);
    } else {
      break;
    }
  }
  MCE_INFO("EdmLogCacheI::Evict() --> last_clear_time: " << last_clear_time_ << " after evict size: " << log_cache_.size());
}

int Process::ProcessLogStr(string& source, string& edm_log, string& left_click_log, string& left_cpc_charge) {
  //解析value串，将edm的clicklog与推左ad的clicklog拆分
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep("|");
  tokenizer tokens(source, sep);
  int num = 0;
  for (tokenizer::iterator tok_iter = tokens.begin(); 
       tok_iter != tokens.end(); ++tok_iter,++num) {
    if (num == 0) {
      edm_log = *tok_iter;
      MCE_DEBUG("EdmClickRequest::ProcessLogStr --> edm_log :" << edm_log);
    } else if ( num == 1) {
      left_click_log = *tok_iter;
      MCE_DEBUG("EdmClickRequest::ProcessLogStr --> left_click_log :" << left_click_log);
    } else if ( num == 2) {
      left_cpc_charge = *tok_iter;
      MCE_DEBUG("EdmClickRequest::ProcessLogStr --> left_charge_log :" << left_cpc_charge);
    }   
  }
  return num;
}

void Process::ProcessLeftClickLog(string& substr) {
  if (substr.empty()) {
    MCE_WARN("Process::ProcessLeftClickLog --> Left Click Log is Empty");
    return;
  }

  size_t time_pos = substr.find("CLICKTIME");
  if ( time_pos == string::npos ) {
    MCE_WARN("Process::ProcessLeftClickLog --> Parse Left Click Log Error, Log:" << substr);
    return;
  }

  MyUtil::Date date(time(NULL));
  substr.replace(time_pos, 9, date.toDateTime() );

  ostringstream left_click_log;
  left_click_log << MACHINE_INFO << "," << substr;
  try {
    AdEdmLogLeftAdapter::instance().Click(left_click_log.str());
  } catch (std::exception& e) {
    MCE_WARN("Process::ProcessLeftClickLog --> print left click log error: " << e.what());
  } catch (...) {
    MCE_WARN("Process::ProcessLeftClickLog --> print left click log error: " << __LINE__);
  }
}

void Process::ProcessLeftChargeLog(string& substr) {
  if (substr.empty()) { 
    MCE_WARN("Process::ProcessLeftChargeLog --> Left Charge Log is Empty");
    return;
  }

  size_t time_pos = substr.find("CLICKTIME");
  if ( time_pos == string::npos ) { 
    MCE_WARN("Process::ProcessLeftChargeLog --> Parse Left Charge Log Error, Log:" << substr);
    return;
  }

  MyUtil::Date date(time(NULL));
  substr.replace(time_pos, 9, date.toDateTime() );
  try {
    AdEdmLogLeftAdapter::instance().CpcCharge(substr);
  } catch (std::exception& e) {
    MCE_WARN("Process::ProcessLeftChargeLog --> print left charge log error: " << e.what());
  } catch (...) {
    MCE_WARN("Process::ProcessLeftChargeLog --> print left charge log error: " << __LINE__);
  }
}

void EvictTask::handle() {
  try{                                                                       
    while (true){
      if (EdmLogCacheI::instance().IsEvict()){
        MCE_INFO("EvictTask::handle --> EdmLogCacheI::evict start");   
        EdmLogCacheI::instance().Evict();
        EdmLogCacheI::instance().UpdateLastClearTime();
        MCE_INFO("EvictTask::handle --> EdmLogCacheI::evict done");    
      }//end of if(IsEvict())
      EdmLogCacheDBManager::instance().EvictFromDB();
      usleep(ONE_MINUTE);
      }// end while (true)                                                    
  } catch (std::exception& e){                                               
    MCE_WARN("EvictTask --> handle ERR LINE: " << __LINE__ << " err: " << e.what());
  } catch (...){
    MCE_WARN("EvictTask --> handle ERR LINE: " << __LINE__);                 
  }

}



