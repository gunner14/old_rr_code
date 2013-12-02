#include <Ice/Ice.h>
#include <list>
#include <vector>
#include <map>
#include <string>
#include "TaskManager.h"
#include "Singleton.h"
#include "EdmLogCacheDBManager.h"
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Monitor.h>
#include <IceUtil/Thread.h>

using namespace std;
using namespace xce::ad;
using namespace mysqlpp;
using namespace com::xiaonei::xce;

const static size_t BATCH_SIZE = 10;
const static size_t QUEUE_WARN_SIZE = 100;
const string TABLE_NAME = "edm_log";

/*
 * 将log数据先写入db cache队列中，等待写入db中
 */
void DBEntryQueue::Put(DBEntryPtr ptr) {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  if (QUEUE_WARN_SIZE < entry_queue_.size()){
    MCE_INFO("DBEntryQueue::Put queue size: " << entry_queue_.size());
  }

  entry_queue_.push_back(ptr);
  notify();
}

/*
 * 从db中读取log
 */
vector<DBEntryPtr > DBEntryQueue::Get() {
  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  while (entry_queue_.size() == 0){ 
    wait();
  }
  vector<DBEntryPtr > vec;
  vec.reserve(BATCH_SIZE);
  for (size_t i = 0; (i < BATCH_SIZE) && (entry_queue_.size() > 0); ++i){
    vec.push_back(entry_queue_.front());
    entry_queue_.pop_front();
  }

  return vec;
}

void EdmLogCacheDBManager::Insert(DBEntryPtr entry){
  entry_queue_.Put(entry);  
}

/*
 * EdmLogDBInserter异步读取DBEntryQueue的cache中数据，最多一次读取10条数据，写入db中
 */
void EdmLogCacheDBManager::EdmLogDBInserter::run(){
  while (true){
    vector<DBEntryPtr > vec = queue_.Get();
    Statement sql = GetInsertSql(vec);
    InsertIntoDB(sql);
  }
}

/*
 * 将从db cache中读取的最多10条记录，组装为一条sql
 */
Statement EdmLogCacheDBManager::EdmLogDBInserter::GetInsertSql(const vector<DBEntryPtr >& entry_vec){
  Statement sql;
  sql << " INSERT INTO " << TABLE_NAME << " (uid_aid, log, expire_time) "
      << " VALUES ";
  vector<DBEntryPtr >::const_iterator v_iter = entry_vec.begin();
  if (v_iter != entry_vec.end()){
    sql << " (\"" << (*v_iter)->GetKey() << "\", \"" << (*v_iter)->GetValue()
        << "\", " << (*v_iter)->GetTime() << ")";
    ++v_iter;
  }

  for (; v_iter != entry_vec.end(); ++v_iter){
    sql << ", (\"" << (*v_iter)->GetKey() << "\", \"" << (*v_iter)->GetValue()
        << "\", " << (*v_iter)->GetTime() << ")"; 
  }

  return sql;
}

void EdmLogCacheDBManager::EdmLogDBInserter::InsertIntoDB(Statement& sql) {
  try{
    QueryRunner(db_, CDbWServer, TABLE_NAME).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("DBEntryQueue::InsertIntoDB Ice Exception: " << e);
  } catch (std::exception& e) {
    MCE_WARN("DBEntryQueue::InsertIntoDB error: " << e.what());
  }
}

string EdmLogCacheDBManager::GetFromDB(const string& key) {
  Statement sql;
  sql << " SELECT uid_aid, log FROM " << TABLE_NAME << " WHERE uid_aid = \"" << key << "\"";
  
  string value;
  mysqlpp::StoreQueryResult res; 
  try {
    res = QueryRunner(db_instance_, CDbRServer, TABLE_NAME).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("EdmLogCacheI::GetFromDB Ice Exception: " << e);
  } catch (std::exception& e) {
    MCE_WARN("EdmLogCacheI::GetFromDB error: " << e.what());
  } 

  if (!res) {
    MCE_INFO("EdmLogCacheI::GetFromDB --> StoreQueryResult wrong");
    return value;
  }
  if (res.empty()) {
    MCE_INFO("EdmLogCacheI::GetFromDB --> nothing in db ");
    return value;
  }
    
  try {
    value = lexical_cast<string>((mysqlpp::Null<string, mysqlpp::NullIsBlank>)res.at(0)["log"]);
  } catch (...) {
    MCE_WARN("EdmLogCacheI::GetFromDB --> lexical_cast uid_aid error!");
  }

  /*const char* DB_HOST = "10.2.120.24";
  const char* DB_NAME = "test";
  const char* DB_USER = "root";
  const char* DB_PASSWORD = "123";

  string value;

  stringstream sql;
  sql << " SELECT * FROM " << TABLE_NAME << " WHERE uid_aid = \"" << key << "\"";// max id!!!!!!

  Connection conn(DB_NAME, DB_HOST, DB_USER, DB_PASSWORD);
  Query q(&conn,true,sql.str().c_str());
  mysqlpp::StoreQueryResult res =  q.store();

  if (res.empty()){
    MCE_WARN("EdmLogCacheI::GetFromDB --> nothing in db");
    return value;
  }

  try {
    value = lexical_cast<string>((mysqlpp::Null<string, mysqlpp::NullIsBlank>)res.at(0)["log"]);
  } catch (...) {
    MCE_WARN("EdmLogCacheI::GetFromDB --> lexical_cast uid_aid error!");
  }
  MCE_INFO("EdmLogCacheI::InsertIntoDB --> key: " << key << "  value: " << value);
  */
  return value;
}

/*
 * 从db中定期逐出超过15天的数据
 */
void EdmLogCacheDBManager::EvictFromDB(){
  Statement sql;
  time_t now = time(NULL);
  sql << "DELETE FROM " << TABLE_NAME << " WHERE expire_time < " << now;

  try {
    QueryRunner(db_instance_, CDbWServer, TABLE_NAME).store(sql);

  } catch (Ice::Exception& e) {
    MCE_WARN("EdmLogCacheI::EvictFromDB Ice Error!" << e);
  } catch (std::exception& e) {
    MCE_WARN("EdmLogCacheI::EvictFromDB Error: " << e.what());
  }

  /*const char* DB_HOST = "10.2.118.45";
  const char* DB_NAME = "test";
  const char* DB_USER = "root";
  const char* DB_PASSWORD = "123";

  stringstream sql;
  time_t now = time(NULL);
  sql << "DELETE FROM " << TABLE_NAME << " WHERE expire_time < " << now;

  Connection conn(DB_NAME, DB_HOST, DB_USER, DB_PASSWORD);
  Query q(&conn,true,sql.str().c_str());
  q.store();*/
}

void EdmLogCacheDBManager::Init(){
  db_inserter_.start().detach(); 
}
