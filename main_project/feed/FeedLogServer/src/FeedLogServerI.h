/*
 * FeedLogServerServerI.h
 *
 *  Created on: 2012-11-13
 *      Author:rui.qiu
 */

#ifndef __FEED_LOG_SERVER_I_H__
#define __FEED_LOG_SERVER_I_H__

#include <map>
#include <IceUtil/Mutex.h>
#include "FeedLogger.h"
#include "FeedLogServer.h"
#include "ServiceI.h"
#include "Singleton.h"

namespace xce {
namespace feed {

using namespace MyUtil;
using namespace std;
using namespace com::xiaonei::xce;

class FeedLogServerI: virtual public FeedLogServer, public MyUtil::Singleton<FeedLogServerI> {
public:
  void Initialize();
  
  ~FeedLogServerI();

  virtual void PutLog(const string& log_name, const string& log, const Ice::Current& current);
  virtual void PutLogBatch(const string& log_name, const StrSeq& logs, const Ice::Current& current);
  virtual void PutLogsMap(const LogsMap& logs, const Ice::Current& current);
  FeedLoggerPtr GetLogger(const string& log_name);

  string db_host() {
    return db_host_;
  }
  void set_db_host(const string& db_host) {
    db_host_ = db_host;
  }

  string db_name() {
    return db_name_;
  }
  void set_db_name(const string& db_name) {
    db_name_ = db_name;
  }

  string db_user() {
    return db_user_;
  }
  void set_db_user(const string& db_user) {
    db_user_ = db_user;
  }

  string db_password() {
    return db_password_;
  }
  void set_db_password(const string& db_password) {
    db_password_ = db_password;
  }

private:
  std::map<string, FeedLoggerPtr> feed_loggers_;
  string db_host_;
  string db_name_;
  string db_user_;
  string db_password_;
};

}
}
#endif
