#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "FeedLogger.h"
#include "FeedLogServerI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "client/linux/handler/exception_handler.h"

using namespace std;
using namespace IceUtil;
using namespace xce::feed;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace mysqlpp;
using namespace boost;

bool MyCallback(const char* dump_path, const char* minidump_id, void* context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize(){
  
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedLogServerI::instance(), service.createIdentity("FLS", ""));
  
  string db_host = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedLogServer.DbHost", "10.4.16.98");
  string db_name = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedLogServer.DbName", "feed_sys");
  string db_user = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedLogServer.DbUser", "feed");
  string db_password = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedLogServer.DbPassword", "rrfeed");
  FeedLogServerI::instance().set_db_host(db_host);
  FeedLogServerI::instance().set_db_name(db_name);
  FeedLogServerI::instance().set_db_user(db_user);
  FeedLogServerI::instance().set_db_password(db_password);
  FeedLogServerI::instance().Initialize();

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FeedLogServer.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedLogServer.Interval",5);
  string name = "Controller" + service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedLogServer.Name", "FeedLogServer");
  xce::serverstate::ServerStateSubscriber::instance().initialize(name, &FeedLogServerI::instance(), mod, interval, new FeedLogChannel());
}

void FeedLogServerI::Initialize() {
  stringstream sql;
  sql << "SELECT log_name, log_file_name, buffer_size, wait_seconds, task_level FROM feed_logger_config WHERE valid = 1";
  mysqlpp::StoreQueryResult res;
  try {
    mysqlpp::Connection conn(db_name_.c_str(), db_host_.c_str(), db_user_.c_str(), db_password_.c_str());
    mysqlpp:: Query query(&conn, true, sql.str().c_str());
    res = query.store();
  } catch (Ice::Exception& e) { 
    MCE_WARN("FeedLogServerI::Initialize --> ice error! " << e.what());
    return;
  } catch (std::exception& e) {
    MCE_WARN("FeedLogServerI::Initialize --> std error! " << e.what());
    return;
  }
  if (!res) {
    MCE_WARN("FeedLogServerI::Initialize --> Query Result Error.");
    return;
  }

  MCE_INFO("FeedLogServerI::Initialize --> res.size:"<< res.num_rows());
  
  string log_path = "../log/" + ServiceI::instance().getName() + "/";
  for (size_t i = 0; i < res.num_rows(); i++) {
    mysqlpp::Row row = res.at(i);
    try {
      std::string log_name = lexical_cast<string>(row["log_name"]);
      std::string log_file_name = lexical_cast<string>(row["log_file_name"]);
      size_t buffer_size = lexical_cast<size_t>(row["buffer_size"]);
      size_t wait_seconds = lexical_cast<size_t>(row["wait_seconds"]);
      int task_level = lexical_cast<int>(row["task_level"]);
      FeedLoggerPtr logger = new FeedLogger(log_name, log_path + log_file_name , "INFO", "%m%n", buffer_size, wait_seconds, task_level);
      feed_loggers_[log_name] = logger;
    } catch (...) {
      MCE_WARN("FeedLogServerI::Initialize --> Parser db config error. cast ubc error");
    }
  }
  MCE_INFO("FeedLogServerI::Initialize --> Initialize FeedLogServer finished. There are " << feed_loggers_.size() << " loggers now");
}

FeedLogServerI::~FeedLogServerI() {
  for(map<string, FeedLoggerPtr>::const_iterator iter = feed_loggers_.begin(); iter != feed_loggers_.end(); ++iter) {
    FeedLoggerPtr logger = iter->second;
    logger->Flush();
  }
  feed_loggers_.clear();
}

void FeedLogServerI::PutLog(const string& log_name, const string& log, const Ice::Current& current) {
  FeedLoggerPtr feed_logger = GetLogger(log_name);
  if (!feed_logger) {
    MCE_WARN("FeedLogServerI::PutLog --> No FeedLogger for " << log_name);
  } else {
    feed_logger->PushBack(log);
  }
}

void FeedLogServerI::PutLogBatch(const string& log_name, const StrSeq& logs, const Ice::Current& current) {
  FeedLoggerPtr feed_logger = GetLogger(log_name);
  if (!feed_logger) {
    MCE_WARN("FeedLogServerI::PutLog --> No FeedLogger for " << log_name);
  } else {
    feed_logger->PushBack(logs);
  }
}

void FeedLogServerI::PutLogsMap(const LogsMap& logs_map, const Ice::Current& current) {
  map<string, MyUtil::StrSeq>::const_iterator it = logs_map.begin();
  for( ; it != logs_map.end(); ++it) {
    FeedLoggerPtr feed_logger = GetLogger(it->first);
    if (!feed_logger) {
      MCE_WARN("FeedLogServerI::PutLog --> No FeedLogger for " << it->first);
    } else {
      feed_logger->PushBack(it->second);
    }
  }
}

FeedLoggerPtr FeedLogServerI::GetLogger(const string& log_name) {
  FeedLoggerPtr feed_logger;
  map<string, FeedLoggerPtr>::const_iterator iter = feed_loggers_.find(log_name);
  if (iter != feed_loggers_.end()) {
    feed_logger = iter->second;
  }
  return feed_logger;
}
