/*
 * =====================================================================================
 *
 *       Filename:  DBWriterI.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月17日 10时31分07秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __DB_WRITER_I__
#define __DB_WRITER_I__
#include "DBManager.h"
#include "ServiceI.h"
#include "../share/DBShare.cpp"
#include "QueryRunner.h"
#include "tripod-new/src/cpp/include/TripodClient.h"

namespace xce {
namespace dbmanager {

using namespace std;
//using namespace com::renren::tripod;

class DBWriterI : virtual public DBWriter, public MyUtil::Singleton<DBWriterI> {
public:
  DBWriterI() {}
  void init(const string& port);
  virtual WriteResultPtr write(const string& dbSrc, const string& pattern, const string& sql, const Ice::Current& current = Ice::Current());
  virtual WriteResultPtr writeWithFilter(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace, 
      const string& businessName, const string& filterSql, const string& key, int op = OP_UPDATE, int expireTime = NEVER_EXPIRE_TIME, 
      const Ice::Current& current = Ice::Current());
  virtual WriteResultPtr writeWithKeys(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace, 
      const string& businessName, const MyUtil::StrSeq& keys, int op = OP_UPDATE, int expireTime = NEVER_EXPIRE_TIME, 
      const Ice::Current& current = Ice::Current());
private:
  com::renren::tripod::TripodClient* getTripodClient(const string& nameSpace, const string& businessName); 
  bool opTripodCache(const string& nameSpace, const string& businessName, const MyUtil::StrSeq& keys, int op = OP_UPDATE, 
      int expireTime = NEVER_EXPIRE_TIME);
  string getValueFromMap(const map<string, string>& keyToValue, const string& key);
  map<string, string> processConfigFile();
  vector<string> getDBSources(const string& dbSource);
  //deque<Ice::ObjectPtr> writeInfos_;
  //IceUtil::Mutex mutex_;
  map<string, com::renren::tripod::TripodClient*> tripodClientMap_;
  IceUtil::RWRecMutex tripodClientMapMutex_;
  ZkManagerPtr zkManager_;
};

}
}

#endif
